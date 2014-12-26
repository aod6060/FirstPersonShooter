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
	main.frag
	
	This scripting file contains code for the main fragment shader
	for my first person shooter game.
*/

#version 330 core

#define LIGHT_SIZE 8
#define LIGHT_ENABLED 1
#define LIGHT_DISABLE 0
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct Light {
	int enabled;
	int type;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	float range; // This is for point light
	vec3 spotDirection; // This is for spotLight
	float spotExp;
	float spotCutOff;
	// Attenuation
	float attenuation;
};

/*
	Material
*/
struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float roughness;
	float energyConserve;
	float reflectIndex;
	float metal;
};

// Sampler main sampler for project
uniform sampler2D tex0;
// Reflective Sampler
uniform samplerCube reflectMap;
// Lights
uniform Light lights[LIGHT_SIZE];
// Material
uniform Material material;

// output color to framebuffer
out vec4 out_Color;

in vec2 pass_TexCoord0;
//in vec4 pass_Color;
in vec3 pass_Normal;
in vec3 pass_LightDir[LIGHT_SIZE];
in vec3 pass_SpotDirection[LIGHT_SIZE];
in vec3 pass_Viewer;
// This is for reflectMap
in vec3 pass_pos_eye;
in vec3 pass_n_eye;

// This is used to calculate the fresnel term
float fresnel(vec3 v, vec3 h, float rn) {
	// Calculate Fresnel Term n = 0.0 for the moment
	float VdotH = dot(v, h);
	
	float f0 = (1.0 - rn) / (1.0 + rn);
	
	f0 = f0 * f0;
	
	float p0 = VdotH * VdotH * VdotH * VdotH * VdotH;
	
	return f0 + (1.0 - f0) * p0;
}

float geometry(vec3 l, vec3 v, vec3 h, vec3 n) {
	// Cook Torrence Optimize
	float NdotH = dot(n, h);
	float NdotV = dot(n, v);
	float NdotL = dot(n, l);
	float VdotH = dot(v, h);
	
	float f1 = 2 * NdotH * NdotV;
	float f2 = 2 * NdotH * NdotL;
	
	float iVdotH = 1.0 / VdotH;
	
	float v1 = f1 * iVdotH;
	float v2 = f2 * iVdotH;

	float g = min(1, min(v1, v2));
	
	return g;
}

// This is far more advace Normal Distribution than blinn-phong
float distr(vec3 h, vec3 n, float m) {

	float NdotHV = dot(n, h);
	
	float alpha = m * m;
	
	float NdotHV2 = NdotHV * NdotHV;
	
	float NdotHV4 = NdotHV2 * NdotHV2;
	
	float dim = 3.14 * pow(alpha, 2) * NdotHV4;
	
	float num = exp((NdotHV2 - 1) / (alpha*alpha * NdotHV2));
	
	float d = num / dim;
	
	return d;
}

// Microfacet Lighting
// Specular Function
float microfacet(vec3 l, vec3 v, vec3 n, float rough, float rn) {

	/*
		fmicrofacet(l, v) = (F(l, h)*G(l, v, h)*D(h)) / (4*dot(n, l)*dot(n, v))

		F is fresnel term
		
		G is geometry term
		
		D is Normal Distribution
	*/
	// Calculate Half Vector
	vec3 h = normalize(l + v);
	// Calculating Dot Products
	
	// Calculate demenator
	float NdotL = dot(n, l);
	float NdotV = dot(n, v);
	
	float dem = 4.0 * NdotL * NdotV;
	
	float F = fresnel(v, h, rn);
	
	//float D = ((a + 2.0) / (2.0*3.14)) * pow(NdotHV, a);
	
	// Distribution
	float D = distr(h, n, rough);
	
	// Calculate Geometry term, using implicit for simplicity
	//float G = NdotL * NdotV;
	float G = geometry(l, v, h, n);
	
	// Catting all terms
	float num = F * G * D;
	
	
	return num / dem;
}

/*
	diffuse(vec3 N, vec3 L, float energyConverasion
*/
float diffuse(vec3 N, vec3 L, float energyConversion) {

	float NdotL = dot(N, L);
	
	float f1 = NdotL + energyConversion;
	
	float f2 = 1 + energyConversion;
	
	f2 = f2 * f2;

	return clamp(f1 / f2, 0.0f, 1.0f);
}

float fresnelReflection(vec3 v, vec3 l, float rn) {
	vec3 h = normalize(l + v);
	
	float F = fresnel(v, h, rn);
	
	return F;
}

vec4 doLights2(int i, vec4 color, float dist, vec3 L) {
	vec4 color2;
	// Directional Light Calculation
	if(lights[i].type == LIGHT_DIRECTIONAL) {
		color2 = clamp(color, 0.0, 1.0);
		
	// Point Light Calculation
	} else if(lights[i].type == LIGHT_POINT) {
		float att = 1.0 / (1.0 + lights[i].attenuation * pow(dist, 2));
		
		att *= att;
		
		//if(lights[i].range - dist > 0.0) {
			color2 = clamp(color * att, 0.0, 1.0);
		//}
		
	// Spot Light Calculation
	} else if(lights[i].type == LIGHT_SPOT) {
		vec3 _L = normalize(pass_LightDir[i]);
		
		vec3 spotDir = normalize(pass_SpotDirection[i]);
		float spotEffect = dot(_L, spotDir);
		spotEffect = pow(spotEffect, lights[i].spotExp);
		
		/*
			http://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
			Thanks for simplification
		*/
		float att = spotEffect / (1.0 + lights[i].attenuation * pow(dist, 2));
		
		color2 = clamp(color * att, 0.0, 1.0);
	}
	
	return color2;
}

vec4 doLights(int i, vec3 N, vec3 V, vec4 albedo) {
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	if(lights[i].enabled == LIGHT_ENABLED) {
		vec3 L = normalize(pass_LightDir[i]);
		
		float mf = microfacet(L, V, N, material.roughness, material.reflectIndex);
		
		vec3 diffuse = lights[i].diffuse * material.diffuse * albedo.xyz * diffuse(N, L, material.energyConserve);
		vec3 specular = lights[i].specular * material.specular * albedo.xyz * mf;
		vec3 emission = material.emission;
		
		color = doLights2(i, vec4(diffuse + specular + emission, 1.0), length(pass_LightDir[i]), L);
	}
	
	return color;
}

// This takes care of the reflect map
vec4 getReflectMap() {
	vec3 incident_eye = normalize(pass_pos_eye);
	vec3 normal = normalize(pass_n_eye);
	
	vec3 reflected = reflect(incident_eye, normal);
	
	return texture(reflectMap, reflected);
}

void main() {
	// Setting to just white output for now
	vec4 color;
	vec3 N = normalize(pass_Normal);
	vec3 V = normalize(pass_Viewer);
	vec4 albedo = texture(tex0, pass_TexCoord0);
	
	color = vec4(1.0);
	
	vec4 lightColors[LIGHT_SIZE];
	
	for(int i = 0; i < LIGHT_SIZE; i++) {
		lightColors[i] = doLights(i, N, V, albedo);
	}
	
	vec4 finalLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	for(int i = 0; i < LIGHT_SIZE; i++) {
		finalLightColor = clamp(lightColors[i] + finalLightColor, 0.0, 1.0);
	}
	
	vec3 gamma = vec3(1.0/2.2);
	
	out_Color = vec4(pow(color.xyz * finalLightColor.xyz, gamma), 1.0);
}