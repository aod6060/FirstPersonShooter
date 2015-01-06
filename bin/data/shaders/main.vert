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

#version 330 core

// Defines
#define LIGHTS_SIZE 8
#define LIGHT_DIRECTION 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2
#define ENABLED 1
#define DISABLED 0

// Structures

// Light
struct Light {
	int enabled; // checks to see if the light is enabled
	int type; // This is the light type
	vec3 position; // Position of the light
	vec3 ambient; // Ambient Light Color
	vec3 diffuse; // diffuse term
	vec3 specular; // Specular Term
	float radius; // For attention
	vec3 spotDirection; // Spot light ra
	float spotExp;
	float intensity;
};

// Uniforms
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 Normal;
uniform mat4 TextureMatrix;
uniform mat4 DepthBias;
uniform vec3 CameraPosition;
uniform vec3 SelfPosition;
// Lighting
uniform Light lights[LIGHTS_SIZE];

// Attributes
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec2 in_TexCoord0;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;

// passes
out vec2 pass_TexCoord0;
out vec3 pass_Normal;
out vec3 pass_Tangent;
out vec3 pass_Viewer;
out vec3 pass_LightPosition[LIGHTS_SIZE];
out vec3 pass_SpotDirection[LIGHTS_SIZE];
out vec3 pass_SelfPosition;
out vec4 pass_ShadowCoord;

// Function Declareations
void setLight(int i, vec3 posW);
void lighting();
void shadow();

void main() {
	// Project vertex to clip space
	gl_Position = Projection * View * Model * vec4(in_Vertex, 1.0);
	
	pass_TexCoord0 = (TextureMatrix * vec4(in_TexCoord0, 1.0, 0.0)).xy;
	
	lighting();
	
	shadow();
}


void setLight(int i, vec3 posW) {
	if(lights[i].type == LIGHT_DIRECTION) {
		pass_LightPosition[i] = normalize(lights[i].position);
	} else if(lights[i].type == LIGHT_POINT) {
		pass_LightPosition[i] = normalize(posW - lights[i].position);
	} else if(lights[i].type == LIGHT_SPOT) {
		pass_LightPosition[i] = normalize(posW - lights[i].position);
		pass_SpotDirection[i] = normalize(lights[i].spotDirection);
	}
}

void lighting() {
	pass_Normal = normalize(mat3(Normal) * in_Normal);
	pass_Tangent = normalize(mat3(Normal) * in_Tangent);
	
	vec3 posW = (Model * vec4(in_Vertex, 1.0)).xyz;
	
	pass_Viewer = normalize(posW - CameraPosition);
	
	pass_SelfPosition = normalize(posW - SelfPosition);
	
	for(int i = 0; i < LIGHTS_SIZE; i++) {
		if(lights[i].enabled == ENABLED) {
			setLight(i, posW);
		}
	}
}

void shadow() {
	pass_ShadowCoord = DepthBias * Model * vec4(in_Vertex, 1.0);
}