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

#ifndef GAME_H_
#define GAME_H_

#include "system.h"
#include "player.h"
#include "land.h"
#include "barrier.h"

class Game : public WindowCallback {
	Font font;

	Land land;
	Barrier barrier;

	//Camera cam;

	Player player;

	StaticMesh enemyStaticMesh;
	Material enemyMaterial;
	Entity enemyEntity;

	StaticMesh handGunStaticMesh;
	Material handGunMaterial;
	Entity handGunEntity;

	StaticMesh handGunAmmoStaticMesh;
	Material handGunAmmoMaterial;
	Entity handGunAmmoEntity;

	StaticMesh healthStaticMesh;
	Material healthMaterial;
	Entity healthEntity;

	StaticMesh crateStaticMesh;
	Material crateMaterial;
	Entity crateEntity;

	Material testMat2;

	// Ligths
	Light light0;

	// Some physics stuff for testing
	// Ground plane
	btCollisionShape* groundShape;
	btRigidBody* groundBody;
	PhysicsInfo pinfo;

	void renderScene();

	void renderUI();

public:

	virtual void init();
	
	virtual void update();

	virtual void release();
};

#endif