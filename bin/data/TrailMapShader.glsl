#version 440

struct Trail
{
	vec4 value;
};

layout(std140, binding = 1) buffer trailMapBuffer { Trail trailMap[]; };
layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

uniform int width;
uniform int height; 
uniform float evaporateSpeed; 
uniform float diffuseSpeed; 
uniform float deltaTime; 

void main()
{
	int j = int(gl_GlobalInvocationID.x) / width;
	int i = int(gl_GlobalInvocationID.x) - j * width;

	int idx = i + j * width;

	vec4 currValue = trailMap[idx].value;

	vec4 sum = vec4(0.0f);
	for (int offsetX = -1; offsetX <= 1; ++offsetX)
	{
		for (int offsetY = -1; offsetY <= 1; ++offsetY)
		{
			int x = i + offsetX;
			int y = j + offsetY;
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				sum += trailMap[x + y * width].value;
			}
		}
	}
	vec4 blur = vec4(sum / 9);
	vec4 diffusedValue = mix(currValue, blur, diffuseSpeed * deltaTime);
	vec4 evaporatedValue = max(vec4(0.0f), diffusedValue - evaporateSpeed * deltaTime);
	trailMap[idx].value = evaporatedValue;
}