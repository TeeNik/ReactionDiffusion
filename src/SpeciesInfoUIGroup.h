#pragma once
#include "ofxGui.h"

struct SpeciesInfo;

class SpeciesInfoUIGroup
{
public:
	void setup(ofxPanel& panel, const SpeciesInfo& info, int speciesTeam);
	void updateInfo(SpeciesInfo& info);
	void loadInfo(const SpeciesInfo& info);
	bool isActive();

private:
	ofxGuiGroup group;
	ofxToggle isOnToggle;
	ofxFloatSlider speedSlider;
	ofxFloatSlider turnSpeedSlider;
	ofxFloatSlider senseDistanceSlider;
	ofxFloatSlider senseAngleSlider;
	ofxIntSlider sensorSizeSlider;
	ofxColorSlider colorSlider;
	ofColor color;
};

