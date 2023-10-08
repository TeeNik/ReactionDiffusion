#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxUbo.h"
#include "ofxDropdown.h"
#include "SpeciesInfoUIGroup.h"
#include "SpeciesSettings.h"

#define TEXTURE_WIDTH 1280
#define TEXTURE_HEIGHT 720

#define GUI_WIDTH 200

#define SCREEN_WIDTH (TEXTURE_WIDTH + GUI_WIDTH)
#define SCREEN_HEIGHT TEXTURE_HEIGHT

//If WIDTH and HEIGHT are smaller that texture size, image will be pixelated
#define WIDTH 128
#define HEIGHT 72
#define RADIUS (HEIGHT / 2)

#define NUM_CELLS 250000


enum SpawnMode
{
	Point,
	Random,
	RandomCircle,
	CircleIn
};

struct SimSettings
{
	float EvaporateSpeed = 0.25f;
	float DiffuseSpeed = 10.0f;
	float TrailWeight = 1.0f;
	std::string SpawnMode = "CircleIn";
	int NumOfTeams = 1;

	static std::map<std::string, enum SpawnMode> GetStringToSpawnModeMap();
};

class ofApp : public ofBaseApp{

public:
	virtual void setup() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void exit() override;

	virtual void keyPressed(int key) override;
	virtual void keyReleased(int key) override;

private:
	void setupCells();
	void setupShaders();
	void setupGui();
	void passSpeciesSettingsToShader(ofShader& shader, int speciesIndex, const SpeciesInfo& info);
	void countNumOfTeams();

	void updateSettings();
	void updateUiBySettings();
	void reset();

	void loadPreset();
	void savePreset();

	ofBufferObject mapBuffer;
	ofBufferObject newMapBuffer;

	std::vector<glm::vec4> map;
	std::vector<glm::vec4> newMap;

	ofTexture texture;
	ofxUboShader cellsShader;
	ofShader drawShader;

	ofxPanel gui;

	ofxButton loadPresetButton;
	ofxButton savePresetButton;
	ofxTextField presetNameText;

	ofxFloatSlider evaporationSpeedSlider;
	ofxFloatSlider diffuseSpeedSlider;
	ofxFloatSlider trailWeightSlider;
	ofxDropdown spawnModeList;

	SimSettings simSettings;
	SpeciesInfoUIGroup speciesSettingsGUI[MAX_SPECIES];
	SpeciesInfo speciesSettings[MAX_SPECIES];
};