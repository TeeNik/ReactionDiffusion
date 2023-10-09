#version 440

layout(std140, binding = 0) buffer mapBuffer { vec4 map[]; };
layout(std140, binding = 1) buffer newMapBuffer { vec4 newMap[]; };
layout(rgba8, binding = 2) uniform writeonly image2D texture;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform int width;
uniform int height; 
uniform vec4 cellColor;

void main()
{
	int i, j;
	i = int(gl_GlobalInvocationID.x);
	j = int(gl_GlobalInvocationID.y);

	int idx = i + j * width;
	map[idx] = newMap[idx];

	vec4 color = newMap[idx];
	color.a = 1.0f;

	//vec4 color = vec4(0);
	//for (uint i = 0; i < 4; ++i) {
	//	vec4 mask = vec4(i==0, i==1, i==2,i==3);
	//	color += speciesSettings[i].color * dot(map, mask); 
	//}

    imageStore(texture, ivec2(i, j), color);
}