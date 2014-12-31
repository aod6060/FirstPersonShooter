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

	this->testTex.init("data/texture/grass0.png");
	testTex2.init("data/texture/grass1.png");
	playerTex.init("data/texture/player.png");
	
	this->sphere.init("data/mesh/sphere.smesh");
	testTex3.init("data/texture/dirt0.png");

	monkey.init("data/mesh/monkey.smesh");
	sand.init("data/texture/sand0.png");

	silver.init("data/texture/silver.png");
	gold.init("data/texture/gold.png");
	copper.init("data/texture/copper.png");
	/*
	reflectMap.init(
		"data/skybox/land2/east.png",
		"data/skybox/land2/west.png",
		"data/skybox/land2/up.png",
		"data/skybox/land2/down.png",
		"data/skybox/land2/south.png",
		"data/skybox/land2/north.png");
		*/

	normal_map.init("data/texture/dirt0_normal.png");
	this->roughness_map.init("data/texture/dirt0_roughness.png");

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
	light0.roughness = 1.0;

	light1.enabled = 0;
	light1.type = Renderer::POINT;
	light1.position = glm::vec3(3.0f, 5.0f, 20.0f);
	light1.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.specular = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.roughness = 0.0;
	light1.radius = 3.0;

	light2.enabled = 0;
	light2.type = Renderer::POINT;
	light2.position = glm::vec3(-3.0f, 5.0f, 20.0f);
	light2.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.specular = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.roughness = 0.0;
	light2.radius = 3.0;
	
	light3.enabled = 0;
	light3.type = Renderer::POINT;
	light3.position = glm::vec3(0.0f, 5.0f, 17.0f);
	light3.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.specular = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.roughness = 0.0;
	light3.radius = 3.0;
	
	light4.enabled = 0;
	light4.type = Renderer::SPOT;
	light4.position = glm::vec3(0.0f, 5.0f, 8.0f);
	light4.diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.specular = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.spotDirection = glm::vec3(0.0f, -0.5f, -0.5f);
	light4.spotExp = 1.0f;
	light4.roughness = 0.0;
	light4.radius = 3.0;
	
	def.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	def.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	def.emission = glm::vec3(0.0);
	def.roughness = 1.0f;
	def.reflectIndex = 1.0f;
	def.energyConserve = 0.3f;


	float ri = 1.0f;
	float r = 1.0f;
	float ec = 0.0f;
	float metal = 0.0f;

	for(int i = 0; i < 10; i++) {
		this->createMaterial(
			this->tests[i],
			glm::vec3(1.0),
			glm::vec3(1.0),
			glm::vec3(0.0),
			ri,
			r,
			ec,
			metal);

		ri -= 0.1;
		r -= 0.1;
		ec += 0.1;
		metal += 0.1;
	}
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

	rend->setMaterial(this->tests[this->test_index]);

	//this->testTex3.bind();

	//reflectMap.bind(GL_TEXTURE1);
	/*
	normal_map.bind(GL_TEXTURE1);
	roughness_map.bind(GL_TEXTURE2);
	*/
	m = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f)) *
				  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


	rend->setModel(m);

	testTex.bind();

	this->test.render();

	testTex.unbind();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	rend->setModel(m);

	testTex2.bind();

	this->terrain.render();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 5.0f));

	rend->setModel(m);

	testTex2.unbind();

	playerTex.bind();

	this->player.render();

	playerTex.unbind();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 18.0f));

	rend->setModel(m);

	testTex3.bind();
	this->sphere.render();
	testTex3.unbind();

	m = glm::translate(glm::vec3(18.0f, 0.0f, 18.0f));

	rend->setModel(m);

	sand.bind();
	this->monkey.render();
	sand.unbind();

	glm::vec3 loc(0.0f, 30.0f, -20);
	
	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, this->tests[i], silver);
		loc.z += 5.0f;
	}

	loc.x = 5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, this->tests[i], copper);
		loc.z += 5.0f;
	}

	loc.x = -5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, this->tests[i], gold);
		loc.z += 5.0f;
	}

	/*
	normal_map.unbind();
	roughness_map.unbind();
	testTex3.unbind();
	*/

	//reflectMap.unbind();

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
	playerTex.release();
	copper.release();
	gold.release();
	silver.release();
	normal_map.release();
	roughness_map.release();
	//reflectMap.release();
	sand.release();
	testTex3.release();
	testTex2.release();
	testTex.release();
	monkey.release();
	sphere.release();
	test.release();
	player.release();
	Renderer::getInstance()->release();
}

void Game::renderSphere(glm::vec3 location, Material& material, Texture& tex) {
	Renderer* rend = Renderer::getInstance();
	
	glm::mat4 m = glm::translate(location);

	rend->setMaterial(material);

	rend->setModel(m);

	tex.bind();

	sphere.render();

	tex.unbind();
}

void Game::createMaterial(Material& m, glm::vec3 d, glm::vec3 s, glm::vec3 e, float ri, float r, float ec, float metal) {
	m.diffuse = d;
	m.specular = s;
	m.emission = e;
	m.reflectIndex = ri;
	m.roughness = r;
	m.energyConserve = ec;
	m.metal = metal;
}
