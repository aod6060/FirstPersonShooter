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
};

// Sampler main sampler for project
uniform sampler2D tex0;

// Lights
uniform Light lights[LIGHT_SIZE];

// output color to framebuffer
out vec4 out_Color;

in vec2 pass_TexCoord0;
//in vec4 pass_Color;
in vec3 pass_Normal;
in vec3 pass_LightDir[LIGHT_SIZE];
in vec3 pass_Viewer;

// This is the beckman equation,
// This is far more advace Normal Distribution than blinn-phong
float beckman(vec3 h, vec3 n, float m) {
	float NdotHV = dot(n, h);
	float m2 = m * m;
	float NdotHV2 = NdotHV * NdotHV;
	float NdotHV4 = NdotHV * NdotHV * NdotHV * NdotHV;
	
	float dim = 3.14 * m2 * NdotHV4;
	
	float num = exp((NdotHV2 - 1) / (m2 * NdotHV2));
	
	return num / dim;
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
	float NdotL = dot(n, l);
	float NdotHV = dot(n, h);
	float NdotV = dot(n, v);
	float LdotHV = dot(l, h);
	float VdotHV = dot(v, h);
	
	// Calculate demenator
	float dem = 4 * NdotL * NdotV;
	
	// Calculate Fresnel Term n = 0.0 for the moment
	float f0 = (1.0 - rn) / (1.0 + rn);
	f0 = f0 * f0;
	//float f0 = 0.5;
	
	float p0 = 1.0 - LdotHV;
	
	p0 = p0 * p0 * p0 * p0 * p0;
	
	float F = f0 + (1.0 - f0)*p0;
	
	// Calculate Normal Distribution or D term, using blinn-phong
	float a = (2.0 / pow(rough, 2.0)) - 2.0; // Shineness Term
	
	//float D = ((a + 2.0) / (2.0*3.14)) * pow(NdotHV, a);
	
	// Beckman
	float D = beckman(h, n, rough);
	
	// Calculate Geometry term, using implicit for simplicity
	//float G = NdotL * NdotV;
	
	float g0 = min((2*NdotHV*NdotV)/VdotHV, (2*NdotHV*NdotL)/VdotHV);
	float G = min(1, g0);
	
	// Catting all terms
	float num = F * G * D;
	
	
	return num / dem;
}

vec4 doLights2(int i, vec4 color, float dist, vec4 lightColor) {
	vec4 color2;
	
	if(lights[i].type == LIGHT_DIRECTIONAL) {
		color2 = clamp(color + lightColor, 0.0, 1.0);
	} else if(lights[i].type == LIGHT_POINT) {
	
		float fscale = lights[i].range - dist;
		
		fscale /= lights[i].range;
		
		if(lights[i].range - dist > 0.0) {
			color2 = clamp(color * fscale + lightColor, 0.0, 1.0);
		}
	}
	
	return color2;
}

vec4 doLights(int i, vec3 N, vec3 V, vec4 albedo, vec4 lightColor) {
	vec4 color = lightColor;
	
	if(lights[i].enabled == LIGHT_ENABLED) {
		vec3 L = normalize(pass_LightDir[i]);
		float NdotL = max(dot(N, L), 0.0);
		float mf = microfacet(L, V, N, 0.1, 1.0);
		
		vec3 diffuse = lights[i].diffuse * albedo.xyz * NdotL * (1.0 - mf);
		vec3 specular = lights[i].specular * albedo.xyz * mf;
		
		color = doLights2(i, vec4(specular+diffuse, 1.0), length(pass_LightDir[i]), lightColor);
	}
	
	return color;
}

void main() {
	// Setting to just white output for now
	vec4 color;
	vec4 albedo = texture(tex0, pass_TexCoord0);
	
	vec3 N = normalize(pass_Normal);
	vec3 V = normalize(pass_Viewer);
	
	//float mf = microfacet(L, V, N, 1.0, 2.0);
	
	//vec3 diffuse = vec3(1.0) * albedo.xyz * NdotL;
	//vec3 specular = vec3(1.0) * albedo.xyz * mf;
	
	color = vec4(1.0);
	
	/*
	if(light0.enabled == LIGHT_ENABLED) {
		vec3 L = normalize(pass_LightDir);
		float NdotL = max(dot(N, L), 0.0);
		float mf = microfacet(L, V, N, 1.0, 2.0);
		
		vec3 diffuse = light0.diffuse * albedo.xyz * NdotL * (1.0 - mf);
		vec3 specular = light0.specular * albedo.xyz * mf;
		
		color *= vec4(specular, 1.0) + vec4(diffuse, 1.0);
	}
	*/
	
	vec4 lightColor;
	
	for(int i = 0; i < LIGHT_SIZE; i++) {
		/*
		if(lights[i].enabled == LIGHT_ENABLED) {
			vec3 L = normalize(pass_LightDir[i]);
			float NdotL = max(dot(N, L), 0.0);
			float mf = microfacet(L, V, N, 1.0, 2.0);
			
			vec3 diffuse = lights[i].diffuse * albedo.xyz * NdotL * (1.0 - mf);
			vec3 specular = lights[i].specular * albedo.xyz * mf;
			
			color += vec4(specular, 1.0) + vec4(diffuse, 1.0);
		}
		*/
		
		lightColor = doLights(i, N, V, albedo, lightColor);
	}
	
	//color = vec4(specular, 1.0) + vec4(diffuse, 1.0);
	
	out_Color = color * lightColor;
}