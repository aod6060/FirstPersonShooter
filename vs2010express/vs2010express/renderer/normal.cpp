#include "../system.h"


// Normal
void Normal::addFaceNoraml(glm::vec3 fn) {
	this->fnorml.push_back(fn);
}

void Normal::generate() {
	this->vnormal = glm::vec3(0.0f);

	for(int i = 0; i < fnorml.size(); i++) {
		this->vnormal += fnorml[i];
	}

	this->vnormal = glm::normalize(this->vnormal);
}

