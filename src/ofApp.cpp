#include "ofApp.h"
#include "PresetManager.h"

std::map<std::string, SpawnMode> SimSettings::GetStringToSpawnModeMap()
{
	static std::map<std::string, enum SpawnMode> StringToSpawnModeMap = {
	{"Point", Point},
	{"Random", Random},
	{"RandomCircle", RandomCircle},
	{"CircleIn", CircleIn}
	};
	return StringToSpawnModeMap;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofBackground(ofColor::black);

	setupGui();
	setupCells();
	setupShaders();
}

void ofApp::setupCells()
{
	map.resize(WIDTH * HEIGHT);
	newMap.resize(WIDTH * HEIGHT);

	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		map[i] = glm::vec4(1, 0, 0, 1);
		newMap[i] = glm::vec4(0, 0.5, 0.5, 1);
	}
}

void ofApp::setupShaders()
{
	drawShader.setupShaderFromFile(GL_COMPUTE_SHADER, "DrawShader.glsl");
	drawShader.linkProgram();

	//cellsShader.setupShaderFromFile(GL_COMPUTE_SHADER, "CellsShader.glsl");
	//cellsShader.linkProgram();

	mapBuffer.allocate(map, GL_DYNAMIC_DRAW);
	mapBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	newMapBuffer.allocate(newMap, GL_DYNAMIC_DRAW);
	newMapBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	texture.allocate(WIDTH, HEIGHT, GL_RGBA8);
	texture.bindAsImage(2, GL_WRITE_ONLY);
	texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
}

void ofApp::setupGui()
{
	gui.setup();

	loadPresetButton.addListener(this, &ofApp::loadPreset);
	savePresetButton.addListener(this, &ofApp::savePreset);

	gui.add(loadPresetButton.setup("Load Preset"));
	gui.add(savePresetButton.setup("Save Preset"));
	gui.add(presetNameText.setup("Preset Name", "preset"));

	gui.add(evaporationSpeedSlider.setup("Evaporation Speed", simSettings.EvaporateSpeed, 0.0f, 2));
	gui.add(diffuseSpeedSlider.setup("Diffuse", simSettings.DiffuseSpeed, 0, 50));
	gui.add(trailWeightSlider.setup("Trail Weight", simSettings.TrailWeight, 0, 20));

	gui.add(spawnModeList.setup("Spawn Mode"));
	spawnModeList.setDropDownPosition(ofxDropdown_<std::basic_string<char>>::DD_LEFT);
	spawnModeList.disableMultipleSelection();
	spawnModeList.enableCollapseOnSelection();
	for (auto& item : SimSettings::GetStringToSpawnModeMap())
	{
		spawnModeList.add(item.first);
	}
	spawnModeList.setSelectedValueByIndex(0, false);

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		speciesSettingsGUI[i].setup(gui, speciesSettings[i], i);
	}

	ofSetWindowTitle("Slime Mold");
	gui.setPosition(ofGetWidth() - GUI_WIDTH, 10);
}

void ofApp::updateSettings()
{
	simSettings.EvaporateSpeed = evaporationSpeedSlider;
	simSettings.DiffuseSpeed = diffuseSpeedSlider;
	simSettings.TrailWeight = trailWeightSlider;
	simSettings.SpawnMode = spawnModeList.selectedValue;

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		speciesSettingsGUI[i].updateInfo(speciesSettings[i]);
	}
}

void ofApp::updateUiBySettings()
{
	evaporationSpeedSlider = simSettings.EvaporateSpeed;
	diffuseSpeedSlider = simSettings.DiffuseSpeed;
	trailWeightSlider = simSettings.TrailWeight;
	spawnModeList.setSelectedValueByName(simSettings.SpawnMode, false);

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		speciesSettingsGUI[i].loadInfo(speciesSettings[i]);
	}
}

void ofApp::reset()
{
	setupCells();
	mapBuffer.updateData(map);
	newMapBuffer.updateData(newMap);
}

void ofApp::loadPreset()
{
	PresetManager::loadPreset(presetNameText, simSettings, speciesSettings);
	updateUiBySettings();
}

void ofApp::savePreset()
{
	PresetManager::savePreset(presetNameText, simSettings, speciesSettings);
}

void ofApp::passSpeciesSettingsToShader(ofShader& shader, int speciesIndex, const SpeciesInfo& info)
{
	const std::string name = "speciesSettings[" + std::to_string(speciesIndex) + "].";
	shader.setUniform1f(name + "moveSpeed", info.moveSpeed);
	shader.setUniform1f(name + "turnSpeed", info.turnSpeed);
	shader.setUniform1f(name + "senseDistance", info.senseDistance);
	shader.setUniform1f(name + "senseAngle", info.senseAngle);
	shader.setUniform1i(name + "sensorSize", info.sensorSize);
	shader.setUniform4f(name + "color", info.color);
}

void ofApp::countNumOfTeams()
{
	int num = 0;
	for (auto& species : speciesSettingsGUI)
	{
		if (species.isActive())
		{
			++num;
		}
	}
	simSettings.NumOfTeams = num;
}

//--------------------------------------------------------------
void ofApp::update()
{
	updateSettings();

	//cellsShader.begin();
	//cellsShader.setUniform1i("width", WIDTH);
	//cellsShader.setUniform1i("height", HEIGHT);
	//cellsShader.setUniform1i("numOfCells", NUM_CELLS);
	//cellsShader.setUniform1f("time", ofGetElapsedTimef());
	//cellsShader.setUniform1f("deltaTime", ofGetLastFrameTime());
	//cellsShader.setUniform1f("trailWeight", simSettings.TrailWeight);
	//
	//for (int i = 0; i < MAX_SPECIES; ++i)
	//{
	//	passSpeciesSettingsToShader(cellsShader, i, speciesSettings[i]);
	//}
	//
	//cellsShader.dispatchCompute((map.size() + 1024 - 1) / 1024, 1, 1);
	//cellsShader.end();

	drawShader.begin();
	drawShader.setUniform1i("width", WIDTH);
	drawShader.setUniform1i("height", HEIGHT);

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		passSpeciesSettingsToShader(drawShader, i, speciesSettings[i]);
	}

	drawShader.dispatchCompute(WIDTH / 1, HEIGHT / 1, 1);
	drawShader.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	texture.draw(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	gui.draw();
}

void ofApp::exit()
{
	loadPresetButton.removeListener(this, &ofApp::loadPreset);
	savePresetButton.removeListener(this, &ofApp::savePreset);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 'r')
	{
		reset();
	}
}