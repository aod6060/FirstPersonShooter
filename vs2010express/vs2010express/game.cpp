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

#include "game.h"

void Game::init() {

	this->test_index = 0;

	// Init Renderer
	Renderer::getInstance()->init();

	this->font.init();


	this->test.init("data/mesh/cube.smesh");
	player.init("data/mesh/player.smesh");

	testMat.init("data/texture/grass0.png",
				 "data/texture/grass0_normal.png",
				 "data/texture/grass0_specular.png");

	testMat2.init("data/texture/grass1.png",
				  "data/texture/grass1_normal.png",
				  "data/texture/grass1_specular.png");

	playerMat.init("data/texture/player.png");
	
	this->sphere.init("data/mesh/sphere.smesh");

	testMat3.init("data/texture/dirt0.png",
				  "data/texture/dirt0_normal.png",
				  "data/texture/dirt0_specular.png");

	monkey.init("data/mesh/monkey.smesh");

	sand.init("data/texture/sand0.png",
			  "data/texture/sand0_normal.png",
			  "data/texture/sand0_specular.png");

	silver.init("data/texture/silver.png",
				"data/texture/def_normal.png",
				"data/texture/roughness.png");

	gold.init("data/texture/gold.png",
			  "data/texture/def_normal.png",
			  "data/texture/roughness.png");

	copper.init("data/texture/copper.png",				
				"data/texture/def_normal.png",
				"data/texture/roughness.png");

	
	emTest.init("data/texture/test.png",
				"data/texture/def_normal.png",
				"data/texture/def_specular.png",
				"data/texture/test_emissive.png");

	alphaTest.init("data/texture/test.png",
				   "data/texture/def_normal.png",
				   "data/texture/def_specular.png",
				   "data/texture/def_emissive.png",
				   "data/texture/alphaTest.png");

	this->terrain.init("data/heightmap/heightmap1.png");

	cam.init(
		glm::vec3(0.0f, 2.0f, 0.0f), 
		glm::vec2(0.0f, 0.0f),
		45.0f,
		1.0f, 
		1000.0f);


	this->yrot = 0.0f;

	/*
	light0.enabled = 1;
	light0.type = Renderer::SPOT;
	light0.position = glm::vec3(40.0, 40.0, 0.0);
	light0.ambient = glm::vec3(0.1, 0.1, 0.1);
	light0.diffuse = glm::vec3(1.0, 1.0, 1.0);
	light0.specular = glm::vec3(1.0, 1.0, 1.0);
	light0.roughness = 1.0f;
	light0.radius = 1.5;
	light0.spotDirection = glm::vec3(-0.5, -0.5, -0.5);
	light0.spotExp = 4.0f;
	*/

	
	light0.enabled = 1;
	light0.type = Renderer::DIRECTION;
	light0.position = glm::vec3(-0.5, -0.5, 0.0);
	light0.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light0.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light0.intensity = 0.1f;

	light1.enabled = 0;
	light1.type = Renderer::POINT;
	light1.position = glm::vec3(3.0f, 5.0f, 20.0f);
	light1.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.specular = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.radius = 3.0;
	light1.intensity = 1.0f;

	light2.enabled = 0;
	light2.type = Renderer::POINT;
	light2.position = glm::vec3(-3.0f, 5.0f, 20.0f);
	light2.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.specular = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.radius = 3.0;
	light2.intensity = 1.0f;

	light3.enabled = 0;
	light3.type = Renderer::POINT;
	light3.position = glm::vec3(0.0f, 5.0f, 17.0f);
	light3.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.specular = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.radius = 3.0;
	light3.intensity = 1.0f;

	light4.enabled = 0;
	light4.type = Renderer::SPOT;
	light4.position = glm::vec3(0.0f, 5.0f, 8.0f);
	light4.diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.specular = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.spotDirection = glm::vec3(0.0f, -0.5f, -0.5f);
	light4.spotExp = 1.0f;
	light4.radius = 3.0;
	light4.intensity = 1.0f;
}
	
void Game::update() {

	WindowManager* wm = WindowManager::getInstance();
	Renderer* rend = Renderer::getInstance();

	glm::mat4 p, v, m;

	if(wm->keyHit(SDL_SCANCODE_ESCAPE)) {
		wm->quit();
	}

	if(wm->keyHit(SDL_SCANCODE_TAB)) {
		if(wm->getRelativeMouseMode() == SDL_TRUE) {
			wm->setRelativeMouseMode(SDL_FALSE);
		} else {
			wm->setRelativeMouseMode(SDL_TRUE);
		}
	}

	if(wm->mouseButtonHit(SDL_BUTTON_LEFT)) {
		this->test_index++;


		if(this->test_index > 9) {
			this->test_index = 0;
		}
	}

	if(wm->keyHit(SDL_SCANCODE_1)) {
		light0.enabled = !light0.enabled;
	}

	if(wm->keyHit(SDL_SCANCODE_2)) {
		light1.enabled = !light1.enabled;
	}

	if(wm->keyHit(SDL_SCANCODE_3)) {
		light2.enabled = !light2.enabled;
	}

	if(wm->keyHit(SDL_SCANCODE_4)) {
		light3.enabled = !light3.enabled;
	}

	if(wm->keyHit(SDL_SCANCODE_5)) {
		light4.enabled = !light4.enabled;
	}

	this->yrot += 1.0f / (1000 / 60);

	if(yrot > 360.0f) {
		this->yrot -= 360.0f;
	}

	cam.update();

	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rend->startShader(Renderer::SCENE);

	rend->setCamera(cam);

	rend->setLight(Renderer::LIGHT0, this->light0);
	rend->setLight(Renderer::LIGHT1, this->light1);
	rend->setLight(Renderer::LIGHT2, this->light2);
	rend->setLight(Renderer::LIGHT3, this->light3);
	rend->setLight(Renderer::LIGHT4, this->light4);

	m = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f)) *
				  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


	rend->setModel(m);

	testMat.bind();

	this->test.render();

	testMat.unbind();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	rend->setModel(m);

	testMat2.bind();

	this->terrain.render();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 5.0f));

	rend->setModel(m);

	testMat2.unbind();

	playerMat.bind();

	this->player.render();

	playerMat.unbind();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 18.0f));

	rend->setModel(m);

	testMat3.bind();
	this->sphere.render();
	testMat3.unbind();

	m = glm::translate(glm::vec3(18.0f, 0.0f, 18.0f));

	rend->setModel(m);

	sand.bind();
	this->monkey.render();
	sand.unbind();

	glm::vec3 loc(0.0f, 30.0f, -20);
	
	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, silver);
		loc.z += 5.0f;
	}

	loc.x = 5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, copper);
		loc.z += 5.0f;
	}

	loc.x = -5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, gold);
		loc.z += 5.0f;
	}

	m = glm::translate(glm::vec3(0.0f, 2.0f, 30.0f));

	rend->setModel(m);

	emTest.bind();
	test.render();
	emTest.unbind();


	m = glm::translate(glm::vec3(4.0f, 2.0f, 30.0f));

	rend->setModel(m);

	alphaTest.bind();
	test.render();
	alphaTest.unbind();

	rend->endShader(Renderer::SCENE);

	rend->startShader(Renderer::UI);

	p = glm::ortho(0.0f, wm->getWidthf(), wm->getHeightf(), 0.0f);
	v = glm::mat4(1.0f);
	m = glm::translate(glm::vec3(32.0f, 32.0f, 0.0f)) * glm::scale(glm::vec3((float)font.getWidth(), (float)font.getHeight(), 0.0f));

	rend->setProjection(p);
	rend->setView(v);
	rend->setModel(m);

	font.renderString("Hello World");

	rend->endShader(Renderer::UI);

	rend = 0;
}

void Game::release() {
	terrain.release();
	font.release();
	playerMat.release();
	copper.release();
	gold.release();
	silver.release();
	sand.release();
	testMat3.release();
	testMat2.release();
	testMat.release();
	monkey.release();
	sphere.release();
	test.release();
	player.release();
	Renderer::getInstance()->release();
}

void Game::renderSphere(glm::vec3 location, Material& material) {
	Renderer* rend = Renderer::getInstance();
	
	glm::mat4 m = glm::translate(location);

	rend->setModel(m);

	material.bind();

	sphere.render();

	material.unbind();
}
