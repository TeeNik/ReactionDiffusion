#pragma once
#include <string>

#include "ofxJSONElement.h"

struct SpeciesInfo;
struct SimSettings;
using std::string;

#define PRESET_DIR "presets/"

class PresetManager
{
public:
	static void savePreset(const string& presetName, const SimSettings& simSettings, const SpeciesInfo* speciesSettings);
	static void loadPreset(const string& presetName, SimSettings& simSettings, SpeciesInfo* speciesSettings);

};

