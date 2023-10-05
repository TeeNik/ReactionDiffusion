#include "PresetManager.h"
#include <ofFileUtils.h>

#include "ofApp.h"

void PresetManager::savePreset(const string& presetName, const SimSettings& simSettings, const SpeciesInfo* speciesSettings)
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

	json["EvaporateSpeed"] = simSettings.EvaporateSpeed;
	json["DiffuseSpeed"] = simSettings.DiffuseSpeed;
	json["TrailWeight"] = simSettings.TrailWeight;
	json["NumOfTeams"] = simSettings.NumOfTeams;
	json["SpawnMode"] = simSettings.SpawnMode;

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		const ::string speciesName = "Species " + std::to_string(i);

		json[speciesName]["moveSpeed"] = speciesSettings[i].moveSpeed;
		json[speciesName]["turnSpeed"] = speciesSettings[i].turnSpeed;
		json[speciesName]["senseDistance"] = speciesSettings[i].senseDistance;
		json[speciesName]["senseAngle"] = speciesSettings[i].senseAngle;
		json[speciesName]["sensorSize"] = speciesSettings[i].sensorSize;
		json[speciesName]["color"]["r"] = speciesSettings[i].color.r;
		json[speciesName]["color"]["g"] = speciesSettings[i].color.g;
		json[speciesName]["color"]["b"] = speciesSettings[i].color.b;
		json[speciesName]["color"]["a"] = speciesSettings[i].color.a;
	}

	bool result = json.save(filePath);
}

void PresetManager::loadPreset(const string& presetName, SimSettings& simSettings, SpeciesInfo* speciesSettings)
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

	simSettings.EvaporateSpeed = json["EvaporateSpeed"].asFloat();
	simSettings.DiffuseSpeed = json["DiffuseSpeed"].asFloat();
	simSettings.TrailWeight = json["TrailWeight"].asFloat();
	simSettings.NumOfTeams = json["NumOfTeams"].asInt();
	simSettings.SpawnMode = json["SpawnMode"].asString();

	for (int i = 0; i < MAX_SPECIES; ++i)
	{
		std::string speciesName = "Species " + std::to_string(i);

		speciesSettings[i].moveSpeed = json[speciesName]["moveSpeed"].asFloat();
		speciesSettings[i].turnSpeed = json[speciesName]["turnSpeed"].asFloat();
		speciesSettings[i].senseDistance = json[speciesName]["senseDistance"].asFloat();
		speciesSettings[i].senseAngle = json[speciesName]["senseAngle"].asFloat();
		speciesSettings[i].sensorSize = json[speciesName]["sensorSize"].asInt();
		speciesSettings[i].color.r = json[speciesName]["color"]["r"].asInt();
		speciesSettings[i].color.g = json[speciesName]["color"]["g"].asInt();
		speciesSettings[i].color.b = json[speciesName]["color"]["b"].asInt();
		speciesSettings[i].color.a = json[speciesName]["color"]["a"].asInt();
	}
}
