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
			pos.x += glm::sin(yrad);
			pos.z -= glm::cos(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_S)) {
			pos.x -= glm::sin(yrad);
			pos.z += glm::cos(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_A)) {
			pos.x -= glm::cos(yrad);
			pos.z -= glm::sin(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_D)) {
			pos.x += glm::cos(yrad);
			pos.z += glm::sin(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_Z)) {
			pos.y -= 1.0;
		}

		if(wm->keyPress(SDL_SCANCODE_X)) {
			pos.y += 1.0;
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