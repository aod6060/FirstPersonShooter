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
#include "../system.h"

// Camera
Camera::Camera() {

}

void Camera::init(glm::vec3 pos, glm::vec2 rot, float fov, float znear, float zfar) {
	this->pos = pos;
	this->rot = rot;

	this->proj = glm::perspective(
					glm::radians(fov),
					WindowManager::getInstance()->getWidthf() / WindowManager::getInstance()->getHeightf(),
					znear,
					zfar);


	this->view = glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));
}

void Camera::render() {
	this->view = glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
				 glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
				 glm::translate(-pos);
}

void Camera::update() {


	WindowManager* wm = WindowManager::getInstance();

	if(wm->getRelativeMouseMode()) {
		int mx, my;

		wm->getMouseCoord(&mx, &my);

		this->rot.x += my * 0.1f;
		this->rot.y += mx * 0.1f;

		float yrad = glm::radians(rot.y);

		if(wm->keyPress(SDL_SCANCODE_W)) {
			pos.x += glm::sin(yrad) * 0.5;
			pos.z -= glm::cos(yrad) * 0.5;
		}

		if(wm->keyPress(SDL_SCANCODE_S)) {
			pos.x -= glm::sin(yrad) * 0.5;
			pos.z += glm::cos(yrad) * 0.5;
		}

		if(wm->keyPress(SDL_SCANCODE_A)) {
			pos.x -= glm::cos(yrad) * 0.5;
			pos.z -= glm::sin(yrad) * 0.5;
		}

		if(wm->keyPress(SDL_SCANCODE_D)) {
			pos.x += glm::cos(yrad) * 0.5;
			pos.z += glm::sin(yrad) * 0.5;
		}

		if(wm->keyPress(SDL_SCANCODE_Z)) {
			pos.y -= 0.5;
		}

		if(wm->keyPress(SDL_SCANCODE_X)) {
			pos.y += 0.5;
		}
	}
}

void Camera::getViewMatrix(glm::mat4& v) {
	v = this->view;
}

void Camera::getProjectionMatrix(glm::mat4& p) {
	p = this->proj;
}

glm::vec3 Camera::getPos() {
	return this->pos;
}

glm::vec2 Camera::getRot() {
	return rot;
}

void Camera::setPos(glm::vec3 p) {
	this->pos = p;
}

void Camera::setRot(glm::vec2 r) {
	this->rot = r;
}

glm::vec3 Camera::getDirection() {
	return glm::vec3(view[0][2], view[1][2], view[2][2]);
}