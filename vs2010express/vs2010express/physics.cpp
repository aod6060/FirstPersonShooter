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

PhysicsManager* PhysicsManager::instance = 0;

void PhysicsManager::_init() {
	this->broadphase = new btDbvtBroadphase();
	this->collisionConfig = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfig);
	this->solver = new btSequentialImpulseConstraintSolver;
	this->world = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfig);

	std::cout << "Init Bullet Physics Engine" << std::endl;
}

btBroadphaseInterface* PhysicsManager::getBroadphase() {
	return this->broadphase;
}

btDefaultCollisionConfiguration* PhysicsManager::getConfig() {
	return this->collisionConfig;
}

btCollisionDispatcher* PhysicsManager::getDispatcher() {
	return this->dispatcher;
}

btSequentialImpulseConstraintSolver* PhysicsManager::getSolver() {
	return this->solver;
}

btDiscreteDynamicsWorld* PhysicsManager::getWorld() {
	return this->world;
}

void PhysicsManager::_release() {
	delete this->world;
	delete this->solver;
	delete this->dispatcher;
	delete this->collisionConfig;
	delete this->broadphase;

	std::cout << "Release Bullet Physics Engine" << std::endl;
}

PhysicsManager* PhysicsManager::getInstance() {
	if(PhysicsManager::instance == 0) {
		PhysicsManager::instance = new PhysicsManager();
		PhysicsManager::instance->_init();
	}

	return PhysicsManager::instance;
}

void PhysicsManager::release() {
	if(PhysicsManager::instance != 0) {
		PhysicsManager::instance->_release();
		delete PhysicsManager::instance;
		PhysicsManager::instance = 0;
	}
}