#include "SpeciesInfoUIGroup.h"
#include "SpeciesSettings.h"

void SpeciesInfoUIGroup::setup(ofxPanel& panel, const SpeciesInfo& info, int speciesTeam)
{
	panel.add(group.setup("Species " + std::to_string(speciesTeam)));
	group.add(isOnToggle.setup("Is On", speciesTeam == 0));
	group.add(speedSlider.setup("Speed", info.moveSpeed, 0, 100));
	group.add(turnSpeedSlider.setup("Turn Speed", info.turnSpeed, -100, 100));
	group.add(senseDistanceSlider.setup("Sense Distance", info.senseDistance, 1, 100));
	group.add(senseAngleSlider.setup("Sense Angle", info.senseAngle, 0.0f, 360.0f));
	group.add(sensorSizeSlider.setup("Sensor Size", info.sensorSize, 0, 4));
	group.add(colorSlider.setup("Color", info.color, 0, 255));
}

void SpeciesInfoUIGroup::updateInfo(SpeciesInfo& info)
{
	info.moveSpeed = speedSlider;
	info.turnSpeed = turnSpeedSlider;
	info.senseDistance = senseDistanceSlider;
	info.senseAngle = senseAngleSlider;
	info.sensorSize = sensorSizeSlider;
	info.color = colorSlider;
}

void SpeciesInfoUIGroup::loadInfo(const SpeciesInfo& info)
{
	speedSlider = info.moveSpeed;
	turnSpeedSlider = info.turnSpeed;
	senseDistanceSlider = info.senseDistance;
	senseAngleSlider = info.senseAngle;
	sensorSizeSlider = info.sensorSize;
	colorSlider = info.color;
}

bool SpeciesInfoUIGroup::isActive()
{
	return isOnToggle;
}
