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

glm::mat4 lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up) {
	glm::vec3 f = glm::normalize(target - eye);
	glm::vec3 u = glm::normalize(up);
	glm::vec3 s = glm::normalize(glm::cross(f, u));

	u = glm::cross(s, f);

	glm::mat4 result;

	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;
	result[3][0] = -glm::dot(s, eye);
	result[3][1] = -glm::dot(u, eye);
	result[3][2] = glm::dot(f, eye);

	return result;
}

void Game::init() {

	this->test_index = 0;

	// Init Renderer
	Renderer::getInstance()->init();

	this->font.init();


	this->test.init("data/mesh/cube.smesh");
	player.init("data/mesh/player.smesh");

	testMat.setAlbedoFilename("data/texture/grass0.png");
	testMat.setNormalFilename("data/texture/grass0_normal.png");
	testMat.setSpecularFilename("data/texture/grass0_specular.png");

	testMat.init();

	testMat2.setAlbedoFilename("data/texture/grass1.png");
	testMat2.setNormalFilename("data/texture/grass1_normal.png");
	testMat2.setSpecularFilename("data/texture/grass1_specular.png");
	testMat2.init();

	playerMat.setAlbedoFilename("data/texture/player.png");
	playerMat.init();

	this->sphere.init("data/mesh/sphere.smesh");

	testMat3.setAlbedoFilename("data/texture/dirt0.png");
	testMat3.setNormalFilename("data/texture/dirt0_normal.png");
	testMat3.setSpecularFilename("data/texture/dirt0_specular.png");
	testMat3.init();

	monkey.init("data/mesh/monkey.smesh");

	sand.setAlbedoFilename("data/texture/sand0.png");
	sand.setNormalFilename("data/texture/sand0_normal.png");
	sand.setSpecularFilename("data/texture/sand0_specular.png");
	sand.init();

	silver.setAlbedoFilename("data/texture/silver.png");
	silver.setSpecularFilename("data/texture/roughness.png");
	silver.setNormalFilename("data/texture/roughness_normal.png");
	silver.init();

	gold.setAlbedoFilename("data/texture/gold.png");
	gold.setSpecularFilename("data/texture/roughness.png");
	gold.setNormalFilename("data/texture/roughness_normal.png");

	gold.init();

	copper.setAlbedoFilename("data/texture/copper.png");
	copper.setSpecularFilename("data/texture/roughness.png");
	copper.setNormalFilename("data/texture/roughness_normal.png");

	copper.init();

	emTest.setAlbedoFilename("data/texture/test.png");
	emTest.setEmissiveFilename("data/texture/test_emissive.png");
	emTest.init();

	alphaTest.setAlbedoFilename("data/texture/test.png");
	alphaTest.setAlphaMaskFilename("data/texture/alphaTest.png");
	alphaTest.init();

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
	light0.intensity = 0.3f;

	light1.enabled = 0;
	light1.type = Renderer::POINT;
	light1.position = glm::vec3(3.0f, 5.0f, 20.0f);
	light1.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.specular = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.radius = 3.0;
	light1.intensity = 0.1f;

	light2.enabled = 0;
	light2.type = Renderer::POINT;
	light2.position = glm::vec3(-3.0f, 5.0f, 20.0f);
	light2.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.specular = glm::vec3(0.0f, 1.0f, 0.0f);
	light2.radius = 3.0;
	light2.intensity = 0.1f;

	light3.enabled = 0;
	light3.type = Renderer::POINT;
	light3.position = glm::vec3(0.0f, 5.0f, 17.0f);
	light3.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.specular = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.radius = 3.0;
	light3.intensity = 0.1f;

	light4.enabled = 0;
	light4.type = Renderer::SPOT;
	light4.position = glm::vec3(0.0f, 5.0f, 8.0f);
	light4.diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.specular = glm::vec3(1.0f, 0.0f, 1.0f);
	light4.spotDirection = glm::vec3(0.0f, -0.5f, -0.5f);
	light4.spotExp = 1.0f;
	light4.radius = 3.0;
	light4.intensity = 0.1f;


	ds.init();
	testTex.init("data/texture/tile0.png");

	depthShader.init("data/shaders/shadow.vert",
					 "data/shaders/shadow.frag");

	depthShader.bind();
	depthShader.createUniform("Projection");
	depthShader.createUniform("View");
	depthShader.createUniform("Model");
	depthShader.unbind();

	depthTexture.init(1024, 1024, Texture::DEPTH);

	depthFramebuffer.create();

	depthFramebuffer.bind();

	depthTexture.bind();

	depthFramebuffer.attachTexture(Framebuffer::DEPTH, depthTexture);

	depthTexture.unbind();

	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "There something wrong with the frame buffer" << std::endl;
	}

	depthFramebuffer.unbind();
}

glm::mat4 Game::renderShadow() {
	// Implement this in a minute

	float num = 128.0f;

	glm::mat4 p = glm::ortho(-num, num, -num, num, -num, num * 2.0f);
	glm::mat4 v = glm::translate(glm::vec3(cam.getPos().x, 0.0f, cam.getPos().z)) * glm::lookAt(
		-light0.position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 m;

	depthFramebuffer.bind();
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	depthShader.bind();

	depthShader.setUniformMatrix4f("Projection", p);
	depthShader.setUniformMatrix4f("View", v);

	m = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f)) *
				  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


	depthShader.setUniformMatrix4f("Model", m);

	this->test.render();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	depthShader.setUniformMatrix4f("Model", m);

	this->terrain.render();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 5.0f));

	depthShader.setUniformMatrix4f("Model", m);

	this->player.render();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 18.0f));

	depthShader.setUniformMatrix4f("Model", m);

	this->sphere.render();

	m = glm::translate(glm::vec3(18.0f, 0.0f, 18.0f));

	depthShader.setUniformMatrix4f("Model", m);

	this->monkey.render();

	glm::vec3 loc(0.0f, 30.0f, -20);
	
	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, depthShader);
		loc.z += 5.0f;
	}

	loc.x = 5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, depthShader);
		loc.z += 5.0f;
	}

	loc.x = -5.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, depthShader);
		loc.z += 5.0f;
	}

	m = glm::translate(glm::vec3(0.0f, 2.0f, 30.0f));

	depthShader.setUniformMatrix4f("Model", m);

	test.render();

	m = glm::translate(glm::vec3(4.0f, 2.0f, 30.0f));

	depthShader.setUniformMatrix4f("Model", m);

	test.render();

	depthShader.unbind();

	depthFramebuffer.unbind();

	return p * v;
}

void Game::renderScene(glm::mat4& depthvp) {
	// Implement this now
	Renderer* rend = Renderer::getInstance();

	glm::mat4 m;

	rend->startShader(Renderer::SCENE);

	glm::mat4 bias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);
	rend->setDepthMatrix(bias * depthvp);

	depthTexture.bind(GL_TEXTURE5);

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

	depthTexture.unbind();

	rend->endShader(Renderer::SCENE);
}

void Game::renderUI() {
	glm::mat4 p, v, m;
	Renderer* rend = Renderer::getInstance();
	WindowManager* wm = WindowManager::getInstance();

	rend->startShader(Renderer::UI);

	p = glm::ortho(0.0f, wm->getWidthf(), wm->getHeightf(), 0.0f);
	v = glm::mat4(1.0f);
	m = glm::translate(glm::vec3(32.0f, 32.0f, 0.0f)) * glm::scale(glm::vec3((float)font.getWidth(), (float)font.getHeight(), 0.0f));

	rend->setProjection(p);
	rend->setView(v);

	rend->setModel(m);

	glm::vec3 cp = cam.getPos();

	font.renderString("Position: [%f, %f, %f]", cp.x, cp.y, cp.z);


	m = glm::translate(glm::vec3(wm->getWidth() - 256.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(256.0f, 256.0f, 0.0));

	rend->setModel(m);

	depthTexture.bind();

	ds.render();

	depthTexture.unbind();

	rend->endShader(Renderer::UI);
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

	glm::mat4 depthvp = this->renderShadow();

	glViewport(0, 0, wm->getWidth(), wm->getHeight());
	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->renderScene(depthvp);

	this->renderUI();

	rend = 0;
}

void Game::release() {
	depthFramebuffer.release();
	depthTexture.release();
	depthShader.release();
	testTex.release();
	ds.release();
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

void Game::renderSphere(glm::vec3 location, Shader& shader) {
	glm::mat4 m = glm::translate(location);

	shader.setUniformMatrix4f("Model", m);

	sphere.render();
}