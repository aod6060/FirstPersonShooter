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

#include "game.h"

struct Sphere {
	glm::vec3 center;
	float radius;
};

bool physicsCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj0, int id0, int index0, const btCollisionObjectWrapper* obj1, int id1, int index1) {
	
	PhysicsInfo* info0 = (PhysicsInfo*)obj0->getCollisionObject()->getUserPointer();

	if(info0->type == PT_ENTITY) {
	}

	return false;
}

void Game::init() {
	// Init Renderer
	Renderer::getInstance()->init();
	PhysicsManager::getInstance();

	::gContactAddedCallback = physicsCallback;

	this->font.init();

	testMat2.setAlbedoFilename("data/texture/grass1.png");
	testMat2.setNormalFilename("data/texture/grass1_normal.png");
	testMat2.setSpecularFilename("data/texture/grass1_specular.png");
	testMat2.init();

	land.init("data/heightmap/heightmap1.png", 32.0f);
	barrier.init(land.getWidth(), land.getHeight());

	player.init(
		glm::vec3(20.0f, land.getY(20, -40) + 2.0f, -40.0f), 
		glm::vec2(0.0f, 180.0f));


	enemyStaticMesh.init("data/mesh/enemy.smesh", true);
	enemyMaterial.setAlbedoFilename("data/texture/enemy.png");
	enemyMaterial.init();
	enemyEntity.init(glm::vec3(0.0f, land.getY(0, 0), 0.0f), enemyStaticMesh);

	handGunStaticMesh.init("data/mesh/hand_gun.smesh");
	handGunMaterial.setAlbedoFilename("data/texture/hand_gun.png");
	handGunMaterial.setNormalFilename("data/texture/hand_gun_normal.png");
	handGunMaterial.setSpecularFilename("data/texture/hand_gun_specular.png");
	handGunMaterial.init();
	handGunEntity.init(glm::vec3(5.0f, land.getY(5, 0), 0.0f), handGunStaticMesh);

	handGunAmmoStaticMesh.init("data/mesh/hand_gun_ammo.smesh");
	handGunAmmoMaterial.setAlbedoFilename("data/texture/hand_gun_ammo.png");
	handGunAmmoMaterial.setNormalFilename("data/texture/hand_gun_ammo_normal.png");
	handGunAmmoMaterial.setSpecularFilename("data/texture/hand_gun_ammo_specular.png");
	handGunAmmoMaterial.init();
	handGunAmmoEntity.init(glm::vec3(10.0f, land.getY(10, 0) + 3.0, 0.0f), handGunAmmoStaticMesh);

	healthStaticMesh.init("data/mesh/health.smesh");
	healthMaterial.setAlbedoFilename("data/texture/health.png");
	healthMaterial.setNormalFilename("data/texture/health_normal.png");
	healthMaterial.setSpecularFilename("data/texture/health_specular.png");
	healthMaterial.init();
	healthEntity.init(glm::vec3(15.0f, land.getY(15, 0), 0.0f), healthStaticMesh);

	crateStaticMesh.init("data/mesh/crate.smesh", true);
	crateMaterial.setAlbedoFilename("data/texture/crate.png");
	crateMaterial.setNormalFilename("data/texture/crate_normal.png");
	crateMaterial.setSpecularFilename("data/texture/crate_specular.png");
	crateMaterial.init();
	crateEntity.init(glm::vec3(20.0f, land.getY(20, 0), 0.0f), crateStaticMesh);

	light0.enabled = 1;
	light0.type = Renderer::DIRECTION;
	light0.position = glm::vec3(0.0, -0.5, 0.5);
	light0.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light0.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light0.intensity = 0.3f;

	PhysicsManager::getInstance()->getWorld()->setGravity(btVector3(0.0, -10.0f, 0.0f));

	this->groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);

	btDefaultMotionState* ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -20, 0)));

	btRigidBody::btRigidBodyConstructionInfo info(
		0,
		ms,
		this->groundShape);

	this->groundBody = new btRigidBody(
		info);

	pinfo.theClass = 0;
	pinfo.type = PT_PLANE;

	this->groundBody->setUserPointer(&pinfo);

	//PhysicsManager::getInstance()->getWorld()->addRigidBody(this->groundBody);
}

void Game::renderScene() {
	// Implement this now
	Renderer* rend = Renderer::getInstance();

	glm::mat4 m;

	rend->startShader(Renderer::SCENE);

	player.render();

	rend->setCamera(player.getCamera());

	rend->setLight(Renderer::LIGHT0, this->light0);

	testMat2.bind();
	land.render();
	testMat2.unbind();

	enemyEntity.render(enemyStaticMesh, enemyMaterial);
	handGunEntity.render(handGunStaticMesh, handGunMaterial);
	handGunAmmoEntity.render(handGunAmmoStaticMesh, handGunAmmoMaterial);
	healthEntity.render(healthStaticMesh, healthMaterial);
	crateEntity.render(crateStaticMesh, crateMaterial);

	rend->endShader(Renderer::SCENE);
}

void Game::renderUI() {
	glm::mat4 p, v, m;
	Renderer* rend = Renderer::getInstance();
	WindowManager* wm = WindowManager::getInstance();

	rend->startShader(Renderer::UI);

	p = glm::ortho(0.0f, wm->getWidthf(), wm->getHeightf(), 0.0f);
	v = glm::mat4(1.0f);
	m = glm::translate(glm::vec3(32.0f, 32.0f, 0.0f)) * glm::scale(glm::vec3((float)font.getWidth(), (float)font.getHeight(), 0.0f));

	rend->setProjection(p);
	rend->setView(v);

	rend->setModel(m);

	glm::vec3 cp = player.getPos();

	font.renderString("Position: [%f, %f, %f]", cp.x, cp.y, cp.z);
	
	rend->endShader(Renderer::UI);
}

void Game::update() {

	WindowManager* wm = WindowManager::getInstance();
	Renderer* rend = Renderer::getInstance();

	glm::mat4 p, v, m;

	if(wm->keyHit(SDL_SCANCODE_ESCAPE)) {
		wm->quit();
	}

	if(wm->keyHit(SDL_SCANCODE_TAB)) {
		if(wm->getRelativeMouseMode() == SDL_TRUE) {
			wm->setRelativeMouseMode(SDL_FALSE);
		} else {
			wm->setRelativeMouseMode(SDL_TRUE);
		}
	}

	PhysicsManager::getInstance()->getWorld()->stepSimulation(1.0 / 60.0, 10);

	player.update();

	glViewport(0, 0, wm->getWidth(), wm->getHeight());
	glClearColor(0.5, 0.6, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->renderScene();

	this->renderUI();

	rend = 0;
}

void Game::release() {
	PhysicsManager::getInstance()->getWorld()->removeRigidBody(this->groundBody);
	delete groundBody->getMotionState();
	delete groundBody;
	delete groundShape;

	crateEntity.release();
	crateMaterial.release();
	crateStaticMesh.release();
	
	healthEntity.release();
	healthMaterial.release();
	healthStaticMesh.release();

	handGunAmmoEntity.release();
	handGunAmmoMaterial.release();
	handGunAmmoStaticMesh.release();
	
	handGunEntity.release();
	handGunMaterial.release();
	handGunStaticMesh.release();
	
	enemyEntity.release();
	enemyMaterial.release();
	enemyStaticMesh.release();
	
	player.release();
	barrier.release();
	land.release();
	font.release();
	testMat2.release();
	PhysicsManager::release();
	Renderer::getInstance()->release();
}