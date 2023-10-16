#include "ofApp.h"
#include "PresetManager.h"

std::map<std::string, SpawnMode> SimSettings::GetStringToDisplayModeMap()
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
	ofSetFrameRate(2*60);
	ofBackground(ofColor::black);

	setupGui();
	setupCells();
	setupShaders();
}

void ofApp::setupCells()
{
	map.resize(WIDTH * HEIGHT);
	newMap.resize(WIDTH * HEIGHT);

	int x = WIDTH / 2;
	int y = HEIGHT / 2;

	ofImage image;
	image.load("circle_big.jpg");
	//image.load("circle.jpg");


	int a = 0;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (image.getWidth() <= j || image.getHeight() <= i)
			{
				map[a].r = 1;
			}
			else
			{
				ofColor color = image.getColor(j, i);

				map[a].r = 1;
				map[a].g = (color.r > 200) ? 1 : 0;

				//if (color.r > 200)
				//{
				//	map[a].g = 1;
				//}
				//else
				//{
				//	map[a].r = 1;
				//}
			}
			//map[a] = glm::vec4(float(i) / HEIGHT, float(j) / WIDTH, 0, 1);
			++a;
		}
		//if (ofRandom(1) > .5f)
		//{
		//	map[i].x = .1f;
		//}
		//else
		//{
		//	map[i].y = .1f;
		//}

		//map[i] = glm::vec4(1, 0, 0, 1);
		//newMap[i] = glm::vec4(0, 0.5, 0.5, 1);
	}
}

void ofApp::setupShaders()
{
	drawShader.setupShaderFromFile(GL_COMPUTE_SHADER, "DrawShader.glsl");
	drawShader.linkProgram();

	cellsShader.setupShaderFromFile(GL_COMPUTE_SHADER, "CellsShader.glsl");
	cellsShader.linkProgram();

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

	gui.add(feedRateSlider.setup("Feed Rate", simSettings.FeedRate, 0.0f, 0.1f));
	gui.add(killRateSlider.setup("Kill Rate", simSettings.KillRate, 0.0f, 0.1f));
	gui.add(diffuseRateASlider.setup("Diffuse Rate A", simSettings.DiffuseRateA, 0.0f, 1.0f));
	gui.add(diffuseRateBSlider.setup("Diffuse Rate B", simSettings.DiffuseRateB, 0.0f, 1.0f));
	gui.add(diffuseRadiusSlider.setup("Diffuse Radius", simSettings.DiffuseRadius, 0, 10));

	gui.add(displayModeList.setup("Display Mode"));
	displayModeList.setDropDownPosition(ofxDropdown_<std::basic_string<char>>::DD_LEFT);
	displayModeList.disableMultipleSelection();
	displayModeList.enableCollapseOnSelection();
	for (auto& item : SimSettings::GetStringToDisplayModeMap())
	{
		displayModeList.add(item.first);
	}
	displayModeList.setSelectedValueByIndex(0, false);

	ofSetWindowTitle("Reaction Diffusion");
	gui.setPosition(ofGetWidth() - GUI_WIDTH, 10);
}

void ofApp::updateSettings()
{
	simSettings.FeedRate = feedRateSlider;
	simSettings.KillRate = killRateSlider;
	simSettings.DiffuseRateA = diffuseRateASlider;
	simSettings.DiffuseRateB = diffuseRateBSlider;
	simSettings.DiffuseRadius = diffuseRadiusSlider;
	simSettings.DisplayMode = displayModeList.selectedValue;
}

void ofApp::updateUiBySettings()
{
	feedRateSlider = simSettings.FeedRate;
	killRateSlider = simSettings.KillRate;
	diffuseRateASlider = simSettings.DiffuseRateA;
	diffuseRateBSlider = simSettings.DiffuseRateB;
	diffuseRadiusSlider = simSettings.DiffuseRadius;
	displayModeList.setSelectedValueByName(simSettings.DisplayMode, false);
}

void ofApp::reset()
{
	setupCells();
	mapBuffer.updateData(map);
	newMapBuffer.updateData(newMap);
}

void ofApp::loadPreset()
{
	PresetManager::loadPreset(presetNameText, simSettings);
	updateUiBySettings();
}

void ofApp::savePreset()
{
	PresetManager::savePreset(presetNameText, simSettings);
}

//--------------------------------------------------------------
void ofApp::update()
{
	updateSettings();

	static bool once = true;

	//if (once)
	{
		once = false;

		cellsShader.begin();
		cellsShader.setUniform1i("width", WIDTH);
		cellsShader.setUniform1i("height", HEIGHT);
		cellsShader.setUniform1i("numOfCells", NUM_CELLS);
		cellsShader.setUniform1f("time", ofGetElapsedTimef());
		cellsShader.setUniform1f("deltaTime", ofGetLastFrameTime());

		cellsShader.setUniform1f("feedRate", simSettings.FeedRate);
		cellsShader.setUniform1f("killRate", simSettings.KillRate);
		cellsShader.setUniform1f("diffuseRateA", simSettings.DiffuseRateA);
		cellsShader.setUniform1f("diffuseRateB", simSettings.DiffuseRateB);
		cellsShader.setUniform1i("diffuseRadius", simSettings.DiffuseRadius);

		int numGroupsX = ceil(WIDTH / 8.0f);
		int numGroupsY = ceil(HEIGHT / 8.0f);

		cellsShader.dispatchCompute(numGroupsX, numGroupsY, 1);
		cellsShader.end();

	}

	drawShader.begin();
	drawShader.setUniform1i("width", WIDTH);
	drawShader.setUniform1i("height", HEIGHT);
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