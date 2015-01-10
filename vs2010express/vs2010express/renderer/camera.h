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
#ifndef CAMERA_H
#define CAMERA_H

/*
	Camera

	This is a basic camera that will be used for the project
*/

class Camera {
	glm::vec3 pos;
	glm::vec2 rot;

	glm::mat4 view;
	glm::mat4 proj;


public:

	Camera();

	void init(glm::vec3 pos, glm::vec2 rot, float fov, float znear, float zfar);

	void render();

	void update();

	void getViewMatrix(glm::mat4& v);

	void getProjectionMatrix(glm::mat4& p);

	glm::vec3 getPos();

	glm::vec2 getRot();

	void setPos(glm::vec3 p);

	void setRot(glm::vec2 r);

	glm::vec3 getDirection();

};

#endif