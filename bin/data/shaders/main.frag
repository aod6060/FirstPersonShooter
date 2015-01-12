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
#define SHADOW_SIZE 4
#define LIGHT_DIRECTION 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2
#define ENABLED 1
#define DISABLED 0

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

struct Material {
	sampler2D albedo;
	sampler2D normal;
	sampler2D specular;
	sampler2D emissive;
	sampler2D alphaMask;
};

// Uniforms
//uniform sampler2D tex0;

// Lighting
uniform Light lights[LIGHTS_SIZE];

// Material System
uniform Material material;

uniform sampler2D shadow[SHADOW_SIZE];

// outward attributes
out vec4 out_Color;

// pass
in vec2 pass_TexCoord0;
in vec3 pass_Normal;
in vec3 pass_Tangent;
in vec3 pass_Viewer;
in vec3 pass_LightPosition[LIGHTS_SIZE];
in vec3 pass_SpotDirection[LIGHTS_SIZE];
in vec3 pass_SelfPosition;

// Function Declarations
vec3 lightTypes(int i, vec3 color, float dist, vec3 L, vec3 H);
float energyCon(float NdotL, float ec);
vec4 lighting();
vec3 calcNormalMap();
void alphaMasking();

void main() {
	vec4 lightColor = lighting();
	
	// Do Gamma correction
	vec3 gamma = vec3(1.0 / 2.2);
	
	alphaMasking();
	
	
	out_Color = vec4(pow(lightColor.xyz, gamma), 1.0); // Make everything green
}

vec3 lightTypes(int i, vec3 color, float dist, vec3 L, vec3 H) {
	vec3 color2;
	
	if(lights[i].type == LIGHT_DIRECTION) {
		color2 = color;
	} else if(lights[i].type == LIGHT_POINT) {
		float att = clamp(1.0 - dist / lights[i].radius, 0.0, 1.0);
		
		att *= att;
		
		color2 = color * att;
	} else if(lights[i].type == LIGHT_SPOT) {
		vec3 spotDir = normalize(pass_SpotDirection[i]);
		
		float spotEffect = dot(L, spotDir);
		
		spotEffect = pow(spotEffect, lights[i].spotExp);
		
		float att = clamp(spotEffect - dist / lights[i].radius, 0.0, 1.0);
		
		att *= att;
		
		color2 = color * att;
	}
	
	return color2;
}

float energyCon(float NdotL, float ec) {
	float f1 = NdotL + ec;
	
	float f2 = 1 + ec;
	
	f2 = f2 * f2;
	
	return clamp(f1 / f2, 0.0, 1.0);
}

/*
	lighting is the main hub for my lighting and
	material pipeline.
*/
vec4 lighting() {
	vec4 color = vec4(0.0);
	color.w = 1.0;
	
	vec3 N = calcNormalMap();
	vec3 V = normalize(pass_Viewer);
	
	float pi = 3.14;
	
	float visibility = 1.0;
	
	float bias = 0.01;
	
	
	vec3 albedo = texture(material.albedo, pass_TexCoord0).xyz;
	vec3 specularMap = texture(material.specular, pass_TexCoord0).xyz;
	vec3 emissiveMap = texture(material.emissive, pass_TexCoord0).xyz;
	
	for(int i = 0; i < LIGHTS_SIZE; i++) {
	
		if(lights[i].enabled == ENABLED) {
			vec3 L = normalize(pass_LightPosition[i]);
			vec3 H = normalize(L + V);
			
			vec3 ambient = lights[i].ambient * albedo;
			
			float NdotL = max(dot(N, L), 0.0);
			float NdotH = max(dot(N, H), 0.0);
			
			float power = (lights[i].intensity) * 256.0f;
			
			float blinnPhong = ((power + 2.0) / (2.0 * pi)) * pow(NdotH, power);
			// Using Labertion diffuse lighting
			vec3 diffuse = lights[i].diffuse * albedo * NdotL;
			
			vec3 specular = lights[i].specular * albedo * specularMap * blinnPhong;
			
			color += vec4(lightTypes(i,ambient + diffuse + specular, length(L), L, H), 1.0);
		}
	}
	
	vec3 selfLit = emissiveMap * albedo;
	
	color += vec4(selfLit, 1.0);
	
	return color;
}

void alphaMasking() {
	vec4 am = texture(material.alphaMask, pass_TexCoord0);
	
	if(am.r == 0.0) {
		discard;
	}
}

vec3 calcNormalMap() {
	vec3 N = normalize(pass_Normal);
	
	vec3 T = normalize(pass_Tangent);
	
	T = normalize(T - dot(T, N));
	
	vec3 B = cross(T, N);
	
	vec3 normal = texture(material.normal, pass_TexCoord0).xyz;
	
	normal = 2.0 * normal - vec3(1.0);
	
	vec3 NN;
	
	mat3 TBN = mat3(T, B, N);
	
	NN = TBN * normal;
	
	NN = normalize(NN);
	
	return NN;
}