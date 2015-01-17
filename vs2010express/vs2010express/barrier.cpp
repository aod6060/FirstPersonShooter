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
#include "barrier.h"

void Barrier::init(Uint32 width, Uint32 height) {
	// Values
	this->width = width;
	this->height = height;
	this->halfWidth = this->width / 2;
	this->halfHeight = this->height / 2;

	std::cout << this->halfWidth << ", " << this->halfHeight << std::endl;
	// Create Shape
	this->_westShape = new btStaticPlaneShape(btVector3(1, 0, 0), 0.0f);
	this->_eastShape = new btStaticPlaneShape(btVector3(-1, 0, 0), 0.0f);
	this->_northShape = new btStaticPlaneShape(btVector3(0, 0, 1), 0.0f);
	this->_southShape = new btStaticPlaneShape(btVector3(0, 0, -1), 0.0f);
	this->_bottomShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0.0f);
	this->_topShape = new btStaticPlaneShape(btVector3(0, -1, 0), 0.0f);
	// Create Motion State
	btDefaultMotionState* westMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-this->halfWidth, 0, 0)));
	btDefaultMotionState* eastMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(this->halfWidth, 0, 0)));
	btDefaultMotionState* northMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -this->halfHeight)));
	btDefaultMotionState* southMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, this->halfHeight)));
	btDefaultMotionState* bottomMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -32, 0)));
	btDefaultMotionState* topMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 256, 0)));
	// Create RigidBody Informations
	btRigidBody::btRigidBodyConstructionInfo westInfo(
		0,
		westMS,
		this->_westShape);
	btRigidBody::btRigidBodyConstructionInfo eastInfo(
		0,
		eastMS,
		this->_eastShape);
	btRigidBody::btRigidBodyConstructionInfo northInfo(
		0,
		northMS,
		this->_northShape);
	btRigidBody::btRigidBodyConstructionInfo southInfo(
		0,
		southMS,
		this->_southShape);
	btRigidBody::btRigidBodyConstructionInfo bottomInfo(
		0,
		bottomMS,
		this->_bottomShape);
	btRigidBody::btRigidBodyConstructionInfo topInfo(
		0,
		topMS,
		this->_topShape);

	// Creating physicsinfo struct
	pinfo.theClass = 0;
	pinfo.type = PT_PLANE;

	// Creating RigidBody 
	this->_westBody = new btRigidBody(westInfo);
	this->_eastBody = new btRigidBody(eastInfo);
	this->_northBody = new btRigidBody(northInfo);
	this->_southBody = new btRigidBody(southInfo);
	this->_bottomBody = new btRigidBody(bottomInfo);
	this->_topBody = new btRigidBody(topInfo);

	// Add pinfo to user pointer
	this->_westBody->setUserPointer(&pinfo);
	this->_eastBody->setUserPointer(&pinfo);
	this->_northBody->setUserPointer(&pinfo);
	this->_southBody->setUserPointer(&pinfo);
	this->_bottomBody->setUserPointer(&pinfo);
	this->_topBody->setUserPointer(&pinfo);

	// Grab Physcis Manager
	PhysicsManager* pm = PhysicsManager::getInstance();
	// Registering the rigid bodies with physics manager
	pm->getWorld()->addRigidBody(this->_westBody);
	pm->getWorld()->addRigidBody(this->_eastBody);
	pm->getWorld()->addRigidBody(this->_northBody);
	pm->getWorld()->addRigidBody(this->_southBody);
	pm->getWorld()->addRigidBody(this->_bottomBody);
	pm->getWorld()->addRigidBody(this->_topBody);

	pm = 0;
}

void Barrier::release() {
	// Remove bodies from physics manager
	PhysicsManager* pm = PhysicsManager::getInstance();
	pm->getWorld()->removeRigidBody(this->_westBody);
	pm->getWorld()->removeRigidBody(this->_eastBody);
	pm->getWorld()->removeRigidBody(this->_northBody);
	pm->getWorld()->removeRigidBody(this->_southBody);
	pm->getWorld()->removeRigidBody(this->_bottomBody);
	pm->getWorld()->removeRigidBody(this->_topBody);
	pm = 0;
	// Set user pointers 0
	_westBody->setUserPointer(0);
	_eastBody->setUserPointer(0);
	_northBody->setUserPointer(0);
	_southBody->setUserPointer(0);
	_bottomBody->setUserPointer(0);
	_topBody->setUserPointer(0);
	// Delete Bodies Motions States
	delete _westBody->getMotionState();
	delete _eastBody->getMotionState();
	delete _northBody->getMotionState();
	delete _southBody->getMotionState();
	delete _bottomBody->getMotionState();
	delete _topBody->getMotionState();
	// Delete Bodies
	delete _westBody;
	delete _eastBody;
	delete _northBody;
	delete _southBody;
	delete _bottomBody;
	delete _topBody;
	// Delete Shapes
	delete _westShape;
	delete _eastShape;
	delete _northShape;
	delete _southShape;
	delete _bottomShape;
	delete _topShape;
}