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
#ifndef PHYSICS_H
#define PHYSICS_H

// Finally I'm going to work with bullet physics.
enum PhysicsType {
	PT_PLANE = 0,
	PT_ENTITY,
	PT_PLAYER,
	PT_SIZE
};

struct PhysicsInfo {
	PhysicsType type;
	void* theClass;
};

// PhysicsManager
class PhysicsManager {

	static PhysicsManager* instance;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;


	PhysicsManager() {}
	PhysicsManager(PhysicsManager&) {}
	PhysicsManager& operator = (PhysicsManager&) {}

	void _init();

	void _release();

public:

	btBroadphaseInterface* getBroadphase();
	btDefaultCollisionConfiguration* getConfig();
	btCollisionDispatcher* getDispatcher();
	btSequentialImpulseConstraintSolver* getSolver();
	btDiscreteDynamicsWorld* getWorld();
	
	static PhysicsManager* getInstance();

	static void release();
};

#endif