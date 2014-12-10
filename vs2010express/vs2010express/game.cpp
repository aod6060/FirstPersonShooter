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


}

void Game::release() {

}