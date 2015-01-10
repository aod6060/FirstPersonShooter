#include "../system.h"

// AnimatedMesh
void AnimatedMesh::updateVertexArray() {

}

AnimatedMesh::AnimatedMesh() {
	this->vao = 0;
	this->size = 0;
}

void AnimatedMesh::init(std::string fn) {

}

void AnimatedMesh::render() {
	this->updateVertexArray();
}

void AnimatedMesh::release() {
	glDeleteBuffers(3, this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

int AnimatedMesh::getVersion() {
	return this->version;
}