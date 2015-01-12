#include "player.h"

void Player::init(glm::vec3 pos, glm::vec2 rot) {
	cam.init(pos, rot, 45.0f, 1.0f, 1024.0f);

	PhysicsManager* pm = PhysicsManager::getInstance();

	this->shape = new btCapsuleShape(1.0f, 2.0f);

	btScalar mass = 1;
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btDefaultMotionState* ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));

	btRigidBody::btRigidBodyConstructionInfo info(
		mass,
		ms,
		this->shape,
		inertia);

	this->body = new btRigidBody(info);

	// Source https://www.youtube.com/watch?v=R4y7fJpY2_Y
	// For setting up a simple first person 
	// Camera.
	// Thank you Doris Grigore :) for the source code.
	body->setAngularFactor(0.0);
	body->setSleepingThresholds(0, 0);

	this->info.theClass = this;
	this->info.type = PT_PLAYER;

	body->setUserPointer(&this->info);

	pm->getWorld()->addRigidBody(body);
}

void Player::render() {
	btVector3 pos = body->getCenterOfMassPosition();
	cam.setPos(glm::vec3(pos.x(), pos.y(), pos.z()));
	cam.render();
}

void Player::update() {
	WindowManager* wm = WindowManager::getInstance();

	if(wm->getRelativeMouseMode()) {
		int mx, my;

		wm->getMouseCoord(&mx, &my);

		glm::vec2 rot = cam.getRot();

		rot.x += my * 0.1f;
		rot.y += mx * 0.1f;

		cam.setRot(rot);

		float yrad = glm::radians(rot.y);

		btVector3 vel = body->getLinearVelocity();

		vel[0] = 0;
		vel[2] = 0;


		float speed = 8.0f;

		if(wm->keyPress(SDL_SCANCODE_E)) {
			speed = 16.0f;
		}

		if(wm->keyPress(SDL_SCANCODE_W)) {
			vel[0] = btSin(yrad) * speed;
			vel[2] = -btCos(yrad) * speed;
		}

		if(wm->keyPress(SDL_SCANCODE_S)) {
			vel[0] = -btSin(yrad) * speed;
			vel[2] = btCos(yrad) * speed;
		}

		if(wm->keyPress(SDL_SCANCODE_A)) {
			vel[0] += -btCos(yrad) * speed;
			vel[2] += -btSin(yrad) * speed;
		}

		if(wm->keyPress(SDL_SCANCODE_D)) {
			vel[0] += btCos(yrad) * speed;
			vel[2] += btSin(yrad) * speed;
		}

		body->setLinearVelocity(vel);
	}
}

void Player::release() {
	PhysicsManager* pm = PhysicsManager::getInstance();
	pm->getWorld()->removeRigidBody(this->body);
	body->setUserPointer(0);
	info.theClass = 0;
	delete body->getMotionState();
	delete body;
	delete shape;
}

void Player::getViewMatrix(glm::mat4& v) {
	cam.getViewMatrix(v);
}

void Player::getProjectionMatrix(glm::mat4& p) {
	cam.getProjectionMatrix(p);
}

glm::vec3 Player::getPos() {
	// I'm going to need to changed this
	return cam.getPos();
}

glm::vec2 Player::getRot() {
	return cam.getRot();
}

void Player::setPos(glm::vec3 p) {
	cam.setPos(p);
}

void Player::setRot(glm::vec2 r) {
	cam.setRot(r);
}

glm::vec3 Player::getDirection() {
	return cam.getDirection();
}

Camera& Player::getCamera() {
	return this->cam;
}
