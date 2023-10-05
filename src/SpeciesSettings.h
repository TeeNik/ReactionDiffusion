#pragma once

#define MAX_SPECIES 4

struct SpeciesInfo
{
	float moveSpeed = 80.0f;
	float turnSpeed = 30.0f;
	float senseDistance = 20.0f;
	float senseAngle = 30.0f;
	int sensorSize = 1;
	ofColor color{ 255, 255, 255, 255 };
};