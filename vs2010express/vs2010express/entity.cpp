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


void Entity::init(glm::vec3 pos) {
	PhysicsManager* pm = PhysicsManager::getInstance();
	this->shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	btScalar mass = 1;
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btDefaultMotionState* ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),btVector3(pos.x, pos.y, pos.z)));

	btRigidBody::btRigidBodyConstructionInfo info(
		mass,
		ms,
		this->shape,
		inertia);

	this->body = new btRigidBody(info);

	this->info.theClass = this;
	this->info.type = PT_ENTITY;

	this->body->setUserPointer(&this->info);

	pm->getWorld()->addRigidBody(body);

	this->prePos = pos;
}

void Entity::init(glm::vec3 pos, StaticMesh& mesh) {
	PhysicsManager* pm = PhysicsManager::getInstance();


	//this->shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	this->shape = new btConvexHullShape();
	
	std::vector<glm::vec3> v;

	mesh.getVerticies(v);

	for(int i = 0; i < v.size(); i++) {
		((btConvexHullShape*)shape)->addPoint(btVector3(
			v[i].x,
			v[i].y,
			v[i].z));
	}

	btScalar mass = 1;
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btDefaultMotionState* ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),btVector3(pos.x, pos.y, pos.z)));

	btRigidBody::btRigidBodyConstructionInfo info(
		mass,
		ms,
		this->shape,
		inertia);

	this->body = new btRigidBody(info);

	this->info.theClass = this;
	this->info.type = PT_ENTITY;

	this->body->setUserPointer(&this->info);

	pm->getWorld()->addRigidBody(body);

	this->prePos = pos;
}

void Entity::render(StaticMesh& mesh, Material& material) {
	glm::mat4 m;

	Renderer* rend = Renderer::getInstance();

	body->getCenterOfMassTransform().getOpenGLMatrix(&m[0][0]);

	rend->setModel(m);

	material.bind();
	mesh.render();
	material.unbind();
}


void Entity::release() {
	PhysicsManager* pm = PhysicsManager::getInstance();
	pm->getWorld()->removeRigidBody(this->body);
	this->body->setUserPointer(0);
	this->info.theClass = 0;
	delete this->body->getMotionState();
	delete this->body;
	delete this->shape;
}

void Entity::reset() {
	btTransform tran = body->getCenterOfMassTransform();

	tran.setOrigin(btVector3(prePos.x, prePos.y, prePos.z));

	body->setCenterOfMassTransform(tran);

	body->forceActivationState(1);
}