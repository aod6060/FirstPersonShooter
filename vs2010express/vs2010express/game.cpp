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

	glEnable(GL_DEPTH_TEST);
	this->shader.init("data/shaders/main.vert", "data/shaders/main.frag");

	this->shader.bind();
	this->shader.createUniform("Projection");
	this->shader.createUniform("View");
	this->shader.createUniform("Model");
	this->shader.createUniform("tex0");
	this->shader.setUniform1i("tex0", 0);
	this->shader.unbind();

	this->ui.init("data/shaders/ui.vert", "data/shaders/ui.frag");

	this->ui.bind();
	this->ui.createUniform("Projection");
	this->ui.createUniform("View");
	this->ui.createUniform("Model");
	this->ui.createUniform("tex0");
	this->ui.setUniform1i("tex0", 0);
	this->ui.unbind();

	this->font.init();


	this->test.init("data/mesh/cube.smesh");

	this->testTex.init("data/texture/grass0.png");
	testTex2.init("data/texture/grass1.png");

	this->terrain.init("data/heightmap/heightmap1.png");

	cam.init(
		glm::vec3(0.0f, 2.0f, 0.0f), 
		glm::vec2(0.0f, 0.0f),
		45.0f,
		1.0f, 
		1000.0f);


	this->yrot = 0.0f;
}
	
void Game::update() {

	WindowManager* wm = WindowManager::getInstance();

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

	this->yrot += 1.0f / (1000 / 60);

	if(yrot > 360.0f) {
		this->yrot -= 360.0f;
	}

	cam.update();

	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.bind();

	/*
	glm::mat4 p = glm::perspective(
		glm::radians(45.0f), 
		wm->getWidthf() / wm->getHeight(),
		1.0f,
		1000.0f);

	glm::mat4 v = glm::rotate(glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f)) *
		          glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));
	*/

	glm::mat4 p, v;

	cam.render();

	cam.getViewMatrix(v);
	cam.getProjectionMatrix(p);

	glm::mat4 m = glm::translate(glm::vec3(0.0f, 2.0f, -5.0f)) *
				  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


	shader.setUniformMatrix4f("Projection", p);

	shader.setUniformMatrix4f("View", v);

	shader.setUniformMatrix4f("Model", m);

	testTex.bind();


	this->test.render();

	testTex.unbind();

	m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	shader.setUniformMatrix4f("Model", m);

	testTex2.bind();

	this->terrain.render();

	testTex2.unbind();

	shader.unbind();


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	ui.bind();

	p = glm::ortho(0.0f, wm->getWidthf(), wm->getHeightf(), 0.0f);
	v = glm::mat4(1.0f);
	m = glm::translate(glm::vec3(32.0f, 32.0f, 0.0f)) * glm::scale(glm::vec3((float)font.getWidth(), (float)font.getHeight(), 0.0f));


	ui.setUniformMatrix4f("Projection", p);
	ui.setUniformMatrix4f("View", v);
	ui.setUniformMatrix4f("Model", m);

	font.renderString("Hello World");

	ui.unbind();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void Game::release() {
	terrain.release();
	font.release();
	testTex2.release();
	testTex.release();
	test.release();
	this->ui.release();
	this->shader.release();
}