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
#include "land.h"


void Land::init(std::string fn, float height) {
	terrain.init(fn, height);

	terrain.getHeights(field);

	this->shape = new btHeightfieldTerrainShape(
		terrain.getWidth(),
		terrain.getHeight(),
		&field[0],
		1.0f,
		0.0,
		terrain.getHeighFactor(),
		1,
		PHY_FLOAT,
		false);


	float offset = terrain.getHeighFactor() * 0.5f;

	btVector3 v(0.0f, offset, 0.0f);


	btDefaultMotionState* ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), v));

	btRigidBody::btRigidBodyConstructionInfo info(
		0,
		ms,
		this->shape);

	this->body = new btRigidBody(info);

	pinfo.theClass = 0;

	pinfo.type = ::PT_TERRAIN;

	this->body->setUserPointer(&pinfo);

	this->body->setFriction(1.5f);
	PhysicsManager::getInstance()->getWorld()->addRigidBody(this->body);

}

void Land::render() {

	glm::mat4 m = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	Renderer::getInstance()->setModel(m);

	terrain.render();
}

void Land::release() {
	PhysicsManager::getInstance()->getWorld()->removeRigidBody(this->body);

	delete body->getMotionState();
	delete body;
	delete shape;

	terrain.release();
}

float Land::getY(int x, int z) {
	return terrain.getY(x, z);
}

Uint32 Land::getWidth() {
	return terrain.getWidth();
}

Uint32 Land::getHeight() {
	return terrain.getHeight();
}
