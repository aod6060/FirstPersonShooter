#include "../system.h"

// Transform
void Transform::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Transform::getPosition() {
	return this->position;
}

void Transform::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
}

glm::vec3 Transform::getRotation() {
	return this->rotation;
}

void Transform::getModel(glm::mat4& m) {
	m = glm::translate(this->position) *
		glm::rotate(glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0)) *
		glm::rotate(glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0)) *
		glm::rotate(glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
}