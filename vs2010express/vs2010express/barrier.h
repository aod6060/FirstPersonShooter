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
#ifndef BARRIER_H
#define BARRIER_H


#include "system.h"

// This class is used to setup a barrier 
// Around map
class Barrier {
	// Collision Shape
	btCollisionShape* _westShape;
	btCollisionShape* _eastShape;
	btCollisionShape* _southShape;
	btCollisionShape* _northShape;
	btCollisionShape* _topShape;
	btCollisionShape* _bottomShape;
	// Rigid Bodies
	btRigidBody* _westBody;
	btRigidBody* _eastBody;
	btRigidBody* _southBody;
	btRigidBody* _northBody;
	btRigidBody* _topBody;
	btRigidBody* _bottomBody;


	Uint32 width;
	Uint32 height;

	float halfWidth;
	float halfHeight;

	PhysicsInfo pinfo;

public:

	void init(Uint32 width, Uint32 height);


	void release();

};

#endif