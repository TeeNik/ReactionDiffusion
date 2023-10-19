#version 440

layout(std140, binding = 0) buffer mapBuffer { vec4 map[]; };
layout(std140, binding = 1) buffer newMapBuffer { vec4 newMap[]; };
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform int width;
uniform int height; 

void main()
{
	int i, j;
	i = int(gl_GlobalInvocationID.x);
	j = int(gl_GlobalInvocationID.y);

	int idx = i + j * width;
	map[idx] = newMap[idx];
}