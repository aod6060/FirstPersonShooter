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

	this->test.init("data/mesh/cube.smesh");

	this->testTex.init("data/texture/grass0.png");

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

	if(wm->keyHit(SDL_SCANCODE_LEFT)) {
		std::cout << "Hitting Left Key" << std::endl;
	}

	if(wm->keyPress(SDL_SCANCODE_RIGHT)) {
		std::cout << "Pressing Right key" << std::endl;
	}

	if(wm->mouseButtonHit(SDL_BUTTON_LEFT)) {
		int x, y;
		wm->getMouseCoord(&x, &y);
		std::cout << "Hitting Right Button [" << x << ", " << y << "]" << std::endl;
	}

	if(wm->mouseButtonPress(SDL_BUTTON_RIGHT)) {
		int x, y;
		wm->getMouseCoord(&x, &y);
		std::cout << "Pressing Right Button [" << x << ", " << y << "]" << std::endl;
	}


	this->yrot += 1.0f / (1000 / 60);

	if(yrot > 360.0f) {
		this->yrot -= 360.0f;
	}

	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.bind();

	glm::mat4 p = glm::perspective(
		glm::radians(45.0f), 
		wm->getWidthf() / wm->getHeight(),
		1.0f,
		1000.0f);

	glm::mat4 v = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 m = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f)) *
				  glm::rotate(yrot, glm::vec3(1.0f, 1.0f, 1.0f));


	shader.setUniformMatrix4f("Projection", p);

	shader.setUniformMatrix4f("View", v);

	shader.setUniformMatrix4f("Model", m);

	testTex.bind();

	this->test.render();

	testTex.unbind();

	shader.unbind();
}

void Game::release() {
	testTex.release();
	test.release();
	this->shader.release();
}