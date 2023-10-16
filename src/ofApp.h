#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxUbo.h"
#include "ofxDropdown.h"

#define TEXTURE_WIDTH 1280
#define TEXTURE_HEIGHT 720

#define GUI_WIDTH 200

#define SCREEN_WIDTH (TEXTURE_WIDTH + GUI_WIDTH)
#define SCREEN_HEIGHT TEXTURE_HEIGHT

//If WIDTH and HEIGHT are smaller that texture size, image will be pixelated
#define WIDTH 1280
#define HEIGHT 720
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
	float FeedRate = 0.0545f;
	float KillRate = 0.062f;
	float DiffuseRateA = 1.0f;
	float DiffuseRateB = 0.5f;
	int DiffuseRadius = 6;
	std::string DisplayMode = "CircleIn";

	static std::map<std::string, enum SpawnMode> GetStringToDisplayModeMap();
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

	ofxFloatSlider feedRateSlider;
	ofxFloatSlider killRateSlider;
	ofxFloatSlider diffuseRateASlider;
	ofxFloatSlider diffuseRateBSlider;
	ofxIntSlider diffuseRadiusSlider;
	ofxDropdown displayModeList;

	SimSettings simSettings;
};