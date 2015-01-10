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

struct Sphere {
	glm::vec3 center;
	float radius;
};

struct Frustrum {
	float neard;
	float fard;
	float fov;
	float ration;
	glm::vec3 point[8];
};

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
	depthShader.createUniform("TextureMatrix");
	glm::mat4 tm = glm::scale(glm::vec3(-1.0f, 1.0f, 1.0f)) * glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	depthShader.setUniformMatrix4f("TextureMatrix", tm);
	depthShader.createUniform("alphaMask");
	depthShader.setUniform1i("alphaMask", 4);
	depthShader.unbind();


	for(int i = 0; i < 4; i++) {
		depthTexture[i].init(1024, 1024, Texture::DEPTH);

		depthFramebuffer[i].create();

		depthFramebuffer[i].bind();

		depthTexture[i].bind();

		depthFramebuffer[i].attachTexture(Framebuffer::DEPTH, depthTexture[i]);

		depthTexture[i].unbind();

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "There something wrong with the frame buffer" << std::endl;
		}

		glDrawBuffer(GL_NONE);

		depthFramebuffer[i].unbind();
	}

	/*
	depthTexture.bind();

	depthFramebuffer.attachTexture(Framebuffer::DEPTH, depthTexture);

	depthTexture.unbind();
	*/
}

void Game::renderShadow() {

	float sp[] = {
		8.0f,
		32.0f,
		128.0f,
		256.0f
	};


	//depthFramebuffer.bind();

	for(int i = 0; i < 4; i++) {

		depthFramebuffer[i].bind();
		//depthFramebuffer.attachTexture(Framebuffer::DEPTH, depthTexture[i]);

		glm::mat4 v = glm::lookAt(
			cam.getPos(),
			cam.getPos() + light0.position,
			glm::vec3(0.0f, 1.0f, 0.0f));

		Sphere bs;

		bs.radius = sp[i];

		glm::vec4 c = v * glm::vec4(cam.getPos(), 1.0);

		glm::vec3 center = glm::vec3(c.x, c.y, c.z);

		glm::vec3 min = center - glm::vec3(bs.radius);
		glm::vec3 max = center + glm::vec3(bs.radius);

		glm::mat4 p = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z*2);

		glm::mat4 m;

		glViewport(0, 0, 1024, 1024);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		depthShader.bind();

		depthShader.setUniformMatrix4f("Projection", p);
		depthShader.setUniformMatrix4f("View", v);

		m = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f)) *
					  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


		depthShader.setUniformMatrix4f("Model", m);

		testMat.bind();
		this->test.render();
		testMat.unbind();

		m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

		depthShader.setUniformMatrix4f("Model", m);

		testMat2.bind();
		this->terrain.render();
		testMat2.unbind();

		m = glm::translate(glm::vec3(0.0f, 0.0f, 5.0f));

		depthShader.setUniformMatrix4f("Model", m);

		playerMat.bind();
		this->player.render();
		playerMat.unbind();

		m = glm::translate(glm::vec3(0.0f, 0.0f, 18.0f));

		depthShader.setUniformMatrix4f("Model", m);

		testMat3.bind();
		this->sphere.render();
		testMat3.bind();

		m = glm::translate(glm::vec3(18.0f, 0.0f, 18.0f));

		depthShader.setUniformMatrix4f("Model", m);

		sand.bind();
		this->monkey.render();
		sand.unbind();

		glm::vec3 loc(25.0f, 30.0f, -20);
	
		silver.bind();
		for(int i = 0; i < 10; i++) {
			this->renderSphere(loc, depthShader);
			loc.z += 5.0f;
		}
		silver.unbind();

		loc.x = 30.0;
		loc.z = -20.0;

		gold.bind();
		for(int i = 0; i < 10; i++) {
			this->renderSphere(loc, depthShader);
			loc.z += 5.0f;
		}
		gold.unbind();

		loc.x = 20.0;
		loc.z = -20.0;

		copper.bind();

		for(int i = 0; i < 10; i++) {
			this->renderSphere(loc, depthShader);
			loc.z += 5.0f;
		}
		copper.unbind();

		m = glm::translate(glm::vec3(0.0f, 2.0f, 30.0f));

		depthShader.setUniformMatrix4f("Model", m);

		emTest.bind();
		test.render();
		emTest.unbind();

		m = glm::translate(glm::vec3(8.0f, 1.0f, 30.0f));

		depthShader.setUniformMatrix4f("Model", m);

		alphaTest.bind();
	
		test.render();

		alphaTest.unbind();

		depthShader.unbind();
		depthFramebuffer[i].unbind();

		this->shadowMatrix[i] = p * v;
	}

	//depthFramebuffer.unbind();
}

void Game::renderScene() {
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

	//rend->setDepthMatrix(bias * shadowMatrix[1]);

	for(int i = 0; i < 4; i++) {
		shadowMatrix[i] = bias * shadowMatrix[i];
	}

	rend->setDepthMatrices(shadowMatrix);

	depthTexture[0].bind(GL_TEXTURE5);
	depthTexture[1].bind(GL_TEXTURE6);
	depthTexture[2].bind(GL_TEXTURE7);
	depthTexture[3].bind(GL_TEXTURE8);

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

	glm::vec3 loc(25.0f, 30.0f, -20);
	
	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, silver);
		loc.z += 5.0f;
	}

	loc.x = 30.0;
	loc.z = -20.0;

	for(int i = 0; i < 10; i++) {
		this->renderSphere(loc, copper);
		loc.z += 5.0f;
	}

	loc.x = 20.0;
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


	m = glm::translate(glm::vec3(8.0f, 1.0f, 30.0f));

	rend->setModel(m);

	alphaTest.bind();
	test.render();
	alphaTest.unbind();

	depthTexture[3].unbind();
	depthTexture[2].unbind();
	depthTexture[1].unbind();
	depthTexture[0].unbind();

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


	m = glm::translate(glm::vec3(wm->getWidth() - 128.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(128.0f, 128.0f, 0.0));

	rend->setModel(m);

	depthTexture[0].bind();

	ds.render();

	depthTexture[0].unbind();

	m = glm::translate(glm::vec3(wm->getWidth() - 128.0f, 128.0f, 0.0f)) *
		glm::scale(glm::vec3(128.0f, 128.0f, 0.0));

	rend->setModel(m);

	depthTexture[1].bind();

	ds.render();

	depthTexture[1].unbind();

	m = glm::translate(glm::vec3(wm->getWidth() - 128.0f, 256.0f, 0.0f)) *
		glm::scale(glm::vec3(128.0f, 128.0f, 0.0));

	rend->setModel(m);

	depthTexture[2].bind();

	ds.render();

	depthTexture[2].unbind();

	m = glm::translate(glm::vec3(wm->getWidth() - 128.0f, 384.0f, 0.0f)) *
		glm::scale(glm::vec3(128.0f, 128.0f, 0.0));

	rend->setModel(m);

	depthTexture[3].bind();

	ds.render();

	depthTexture[3].unbind();

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

	//glm::mat4 depthvp = this->renderShadow();

	this->renderShadow();

	glViewport(0, 0, wm->getWidth(), wm->getHeight());
	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->renderScene();

	this->renderUI();

	rend = 0;
}

void Game::release() {
	for(int i = 0; i < 4; i++) {
		depthFramebuffer[i].release();
	}

	for(int i = 0; i < 4; i++) {
		depthTexture[i].release();
	}

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