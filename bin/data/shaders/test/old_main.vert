/*
The MIT License (MIT)

Copyright (c) 2014 Frederick Cook <a.o.d.6060@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/*
	main.vert
	
	This scripting file contiain the vertex shader code
	for my first person shooter game.
*/

#version 330 core

#define LIGHT_SIZE 8
#define LIGHT_ENABLED 1
#define LIGHT_DISABLE 0
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

/*
	Light Structure
*/
struct Light {
	int enabled;
	int type;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	float range; // This is for point light
	vec3 spotDirection;
	float spotExp;
	float spotCutOff;
	// Attenuation
	float attenuation;
};
// Matrix used to transform from camera space to clip space
uniform mat4 Projection;
// Matrix used to transform from world space to camera space
uniform mat4 View;
// Matrix used to transform from local space to world space
uniform mat4 Model;
// Matrix for normals
uniform mat4 Normal;
// Fix for texture problems
uniform mat4 TextureMatrix;
// this is the camea coord (note, this helps with specular)
uniform vec3 CameraPosition;

// Lights
uniform Light lights[LIGHT_SIZE];

// Vertex Attribute
layout(location = 0) in vec3 in_Vertex;
// Normal Attribute
layout(location = 1) in vec3 in_Normal;
// TexCoord Attribute
layout(location = 2) in vec2 in_TexCoord0;
// Tangent Space Attribute
layout(location = 3) in vec3 in_Tangent;
// BiTangent Space Attribute
layout(location = 4) in vec3 in_BiTangent;

// More stuff to come

out vec2 pass_TexCoord0;
//out vec4 pass_Color;
out vec3 pass_Normal;
out vec3 pass_Tangent;
//out vec3 pass_BiTangent;
out vec3 pass_LightDir[LIGHT_SIZE];
out vec3 pass_SpotDirection[LIGHT_SIZE];
out vec3 pass_Viewer;

void setLights(int i, vec3 posW) {
	if(lights[i].enabled == LIGHT_ENABLED) {
		if(lights[i].type == LIGHT_DIRECTIONAL) {
			pass_LightDir[i] = normalize(lights[i].position);
		} else if(lights[i].type == LIGHT_POINT) {
			pass_LightDir[i] = normalize(posW - lights[i].position);
		} else if(lights[i].type == LIGHT_SPOT) {
			// Implement This later
			pass_LightDir[i] = normalize(posW - lights[i].position);
			pass_SpotDirection[i] = normalize(lights[i].spotDirection);
		}
	}
}

// This is were the lights will be calculated
void lighting() {
	vec3 posW = (Model * vec4(in_Vertex, 1.0)).xyz;
	
	pass_Normal = mat3(Normal) * in_Normal;
	pass_Tangent = mat3(Normal) * in_Tangent;
	//pass_BiTangent = mat3(Normal) * in_BiTangent;
	
	//vec3 N = normalize(mat3(Normal) * in_Normal);
	
	vec3 V = normalize(posW - CameraPosition);
	
	pass_Viewer = V;
	
	for(int i = 0; i < LIGHT_SIZE; i++) {
		setLights(i, posW);
	}
}

/*
// this is were the reflection map is calculated
void reflectMap() {
	pass_pos_eye = vec3(View * Model * vec4(in_Vertex, 1.0));
	pass_n_eye = vec3(transpose(inverse(View * Model)) * vec4(in_Normal, 0.0));
}
*/

void main() {
	pass_TexCoord0 = (TextureMatrix * vec4(in_TexCoord0, 1.0, 1.0)).xy;
	gl_Position = Projection * View * Model * vec4(in_Vertex, 1.0);
	
	lighting();
	//reflectMap();
}