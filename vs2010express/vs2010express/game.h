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

#ifndef GAME_H_
#define GAME_H_

#include "system.h"


class Game : public WindowCallback {
	Font font;

	Terrain terrain;

	Camera cam;

	StaticMesh test;
	StaticMesh sphere;
	StaticMesh player;
	StaticMesh monkey;

	Material testMat;
	Material testMat2;
	Material playerMat;
	Material testMat3;
	Material sand;
	Material silver;
	Material gold;
	Material copper;
	Material emTest;
	Material alphaTest;

	// Ligths
	Light light0;
	Light light1;
	Light light2;
	Light light3;
	Light light4;

	// DrawSurface
	DrawSurface ds;
	Texture testTex;

	Shader depthShader;
	Texture depthTexture;
	Framebuffer depthFramebuffer;

	float yrot;

	void renderSphere(glm::vec3 location, Material& material);

	void renderSphere(glm::vec3 location, Shader& shader);

	void createMaterial(Material& m, glm::vec3 d, glm::vec3 s, glm::vec3 e, float ri, float r, float ec, float metal);

	int test_index;


	glm::mat4 renderShadow();

	void renderScene(glm::mat4& depthvp);

	void renderUI();

public:

	virtual void init();
	
	virtual void update();

	virtual void release();
};

#endif