#version 440

layout(std140, binding = 0) buffer mapBuffer { vec4 map[]; };
layout(std140, binding = 1) buffer newMapBuffer { vec4 newMap[]; };

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

uniform int width;
uniform int height; 
uniform float deltaTime; 
uniform float time;

uniform float feedRate;
uniform float killRate;
uniform float diffuseRateA;
uniform float diffuseRateB;
uniform int diffuseRadius;

const float PI = 3.1415;

float hash01(uint n) 
{
	n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U;
    return float( n & uint(0x7fffffffU))/float(0x7fffffff);
}

uint hash(uint state)
{
    state ^= 2747636419u;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    return state;
}

float scaleToRange01(uint state)
{
    return state / 4294967295.0;
}

void main()
{
	int i, j;
	i = int(gl_GlobalInvocationID.x);
	j = int(gl_GlobalInvocationID.y);

	//int idx = i + j * width;
	int idx = i;
	ivec2 id = ivec2(i, j);

	vec2 currentValues = map[idx].xy;
	float a = currentValues.x;
	float b = currentValues.y;

	vec2 sum = vec2(0,0);
	float weightSum;
	for (int offsetY = -diffuseRadius; offsetY <= diffuseRadius; offsetY ++) {
		for (int offsetX = -diffuseRadius; offsetX <= diffuseRadius; offsetX ++) {
			if (offsetX == 0 && offsetY == 0) {
				continue;
			}
			ivec2 samplePos = id.xy + ivec2(offsetX, offsetY);
			samplePos = min(ivec2(width-1, height-1), max(ivec2(0,0), samplePos));
			float sqrDst = float(offsetX * offsetX + offsetY * offsetY);
			
			if (sqrDst <= diffuseRadius * diffuseRadius) {
				float weight = 1.0 / sqrt(sqrDst);
				sum += map[samplePos.x + samplePos.y * width].xy * weight;
				weightSum += weight;
			}
		}
	}

	vec2 diffuseStrength = sum / weightSum - currentValues;

	//newMap[idx] = vec4(sum.x, sum.y, 0, 1);
	//return;


	float deltaA = (diffuseRateA * diffuseStrength.x - a * b * b + feedRate * (1-a));
	float newA = a + deltaA;

	float deltaB = (diffuseRateB * diffuseStrength.y + a * b * b - (killRate + feedRate) * b);
	float newB = b + deltaB;

	newMap[idx] = vec4(max(0, newA), min(newB, 1), deltaA, deltaB);
	//newMap[idx] = map[idx];
}