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

#include "system.h"


WindowManager* WindowManager::instance = 0;


void WindowManager::_release() {
	// Release SDL
	if(this->conf) {
		if(this->conf->callback) {
			this->conf->callback->release();
		}
		this->conf = 0;
	}
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

WindowManager* WindowManager::getInstance() {
	if(WindowManager::instance == 0) {
		WindowManager::instance = new WindowManager();
	}

	return WindowManager::instance;
}

void WindowManager::release() {
	WindowManager::instance->_release();
	delete WindowManager::instance;
	WindowManager::instance = 0;
}


void WindowManager::init(WindowConfig* conf) {
	this->conf = conf;
	this->running = true;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->window = SDL_CreateWindow(
		conf->caption.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		conf->width,
		conf->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


	this->context = SDL_GL_CreateContext(this->window);

	if(conf->vsync) {
		SDL_GL_SetSwapInterval(1);
	} else {
		SDL_GL_SetSwapInterval(0);
	}

	// Init other subsystems
	this->initKeyboard();
	this->initMouse();

	glewInit();

	if(this->conf) {
		if(this->conf->callback) {
			this->conf->callback->init();
		}
	}
}

void WindowManager::mainLoop() {
	SDL_Event ev;

	while(this->running) {
		while(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_QUIT) {
				this->running = false;
			}
		}

		this->updateKeyboard();
		this->updateMouse();

		if(this->conf) {
			if(this->conf->callback) {
				this->conf->callback->update();
			}
		}

		SDL_GL_SwapWindow(this->window);
	}
}

Uint32 WindowManager::getWidth() {
	return conf->width;
}

Uint32 WindowManager::getHeight() {
	return conf->height;
}

float WindowManager::getWidthf() {
	return conf->width;
}

float WindowManager::getHeightf() {
	return conf->height;
}

void WindowManager::quit() {
	this->running = false;
}

// Keyboard Section
void WindowManager::initKeyboard() {
	for(int i = 0; i < SDL_NUM_SCANCODES; i++) {
		this->hits[i] = false;
	}
}

void WindowManager::updateKeyboard() {
	this->keys = SDL_GetKeyboardState(0);
}

bool WindowManager::keyPress(int key) {
	return this->keys[key];
}

bool WindowManager::keyHit(int key) {
	if(this->keyPress(key) && !hits[key]) {
		hits[key] = true;
		return true;
	}

	if(!this->keyPress(key) && hits[key]) {
		hits[key] = false;
	}

	return false;
}

// Mouse Section
void WindowManager::initMouse() {
	for(int i = 0; i < 3; i++) {
		this->buttonHits[i] = false;
	}
}

void WindowManager::updateMouse() {
	if(SDL_GetRelativeMouseMode()) {
		this->buttons = SDL_GetRelativeMouseState(&this->mx, &this->my);
	} else {
		this->buttons = SDL_GetMouseState(&this->mx, &this->my);
	}
}

void WindowManager::getMouseCoord(int* mx, int* my) {
	*mx = this->mx;
	*my = this->my;
}

bool WindowManager::mouseButtonPress(int b) {
	return this->buttons & SDL_BUTTON(b);
}

bool WindowManager::mouseButtonHit(int b) {
	if(this->mouseButtonPress(b) && !this->buttonHits[b-1]) {
		this->buttonHits[b-1] = true;
		return true;
	}

	if(!this->mouseButtonPress(b) && this->buttonHits[b-1]) {
		this->buttonHits[b-1] = false;
	}

	return false;
}

void WindowManager::setRelativeMouseMode(SDL_bool b) {
	SDL_SetRelativeMouseMode(b);
}

SDL_bool WindowManager::getRelativeMouseMode() {
	return SDL_GetRelativeMouseMode();
}
