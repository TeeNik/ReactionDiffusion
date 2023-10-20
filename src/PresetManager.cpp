#include "PresetManager.h"
#include "ofFileUtils.h"
#include "ofApp.h"

void PresetManager::savePreset(const string& presetName, const SimSettings& simSettings)
{
	const string filePath = PRESET_DIR + presetName + ".json";
	ofFile file(filePath);

	Json::Value ev;
	ofxJSONElement json;


	if (!file.exists()) {
		std::cout << "The file " + filePath + " is missing\n";
		file.create();
	}
	else
	{
		std::cout << "The file " + filePath + " is found\n";
	}

	json["FeedRate"] = simSettings.FeedRate;
	json["KillRate"] = simSettings.KillRate;
	json["DiffuseRateA"] = simSettings.DiffuseRateA;
	json["DiffuseRateB"] = simSettings.DiffuseRateB;
	json["DiffuseRadius"] = simSettings.DiffuseRadius;
	json["StepsPerFrame"] = simSettings.StepsPerFrame;
	json["DisplayMode"] = simSettings.DisplayMode;

	bool result = json.save(filePath);
}

void PresetManager::loadPreset(const string& presetName, SimSettings& simSettings)
{
	const string filePath = PRESET_DIR + presetName + ".json";
	ofFile file(filePath);

	if (!file.exists()) {
		std::cout << "Preset " + presetName + " is missing\n";
		return;
	}

	file.close();

	ofxJSONElement json;
	json.open(filePath);

	simSettings.FeedRate = json["FeedRate"].asFloat();
	simSettings.KillRate = json["KillRate"].asFloat();
	simSettings.DiffuseRateA = json["DiffuseRateA"].asFloat();
	simSettings.DiffuseRateB = json["DiffuseRateB"].asFloat();
	simSettings.DiffuseRadius = json["DiffuseRadius"].asInt();
	simSettings.StepsPerFrame = json["StepsPerFrame"].asInt();
	simSettings.DisplayMode = json["DisplayMode"].asString();
}
