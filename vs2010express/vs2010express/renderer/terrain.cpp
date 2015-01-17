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
#include "../system.h"

// Terrain
Terrain::Terrain() {
	this->width = 0;
	this->height = 0;
}

void Terrain::init(std::string fn, float hs) {
	SDL_Surface* surf = 0;
	surf = IMG_Load(fn.c_str());

	this->heightFactor = hs;

	if(surf == 0) {
		std::cout << fn << ": doesn't load" << std::endl;
		return;
	}


	this->width = surf->w;
	this->height = surf->h;


	SDL_Color* colors = (SDL_Color*)surf->pixels;


	
	for(int y = 0; y < this->height; y++) {
		for(int x = 0; x < this->width; x++) {
			heights.push_back(colors[y * width + x].r);
		}
	}

	SDL_FreeSurface(surf);

	std::vector<glm::vec3> v;
	std::vector<Normal> n;
	std::vector<glm::vec3> n2;
	std::vector<glm::vec2> t;
	std::vector<Triangle> tris;

	v.resize(this->width * this->height);
	t.resize(this->width * this->height);
	n.resize(this->width * this->height);
	n2.resize(this->width * this->height);

	int halfWidth = width / 2;
	int halfHeight = height / 2;

	for(int y = 0; y < this->height; y++) {
		for(int x = 0; x < this->width; x++) {
			glm::vec3 tempv;
			glm::vec2 tempt;

			tempv.x = x - halfWidth;
			tempv.y = (this->heights[y * this->width + x] / 255.0f) * this->heightFactor;
			tempv.z = y - halfHeight;

			tempt.x = x * 0.5;
			tempt.y = y * 0.5;

			v[y * height + x] = tempv;
			t[y * height + x] = tempt;

		}
	}

	for(int y = 0; y < this->height - 1; y++) {
		for(int x = 0; x < this->width - 1; x++) {
			Triangle t1;
			Triangle t2;

			t1.v1 = y * this->width + x;
			t1.v2 = y * this->width + (x + 1);
			t1.v3 = (y + 1) * this->width + x;

			t2.v1 = (y + 1) * this->width + x;
			t2.v2 = y * this->width + (x + 1);
			t2.v3 = (y + 1) * this->width + (x + 1);

			tris.push_back(t1);
			tris.push_back(t2);
		}
	}

	this->count = tris.size() * 3;

	for(int i = 0; i < tris.size(); i++) {
		
		glm::vec3 U;
		glm::vec3 V;

		glm::vec3 N;

		U = v[tris[i].v2] - v[tris[i].v1];
		V = v[tris[i].v3] - v[tris[i].v1];

		N = glm::cross(U, V);

		N = glm::normalize(N);

		n2[tris[i].v1] += N;
		n2[tris[i].v2] += N;
		n2[tris[i].v3] += N;
	}

	for(int i = 0; i < n2.size(); i++) {
		n2[i] = glm::normalize(n2[i]);
	}

	std::vector<glm::vec3> tangent;
	std::vector<glm::vec3> biTangent;

	for(int i = 0; i < n.size(); i++) {
		glm::vec3 T, C1, C2;

		C1 = glm::cross(n2[i], glm::vec3(0.0f, 0.0f, 1.0f));
		C2 = glm::cross(n2[i], glm::vec3(0.0f, 1.0f, 0.0f));

		if(glm::length(C1) > glm::length(C2)) {
			T = C1;
		} else {
			T = C2;
		}

		T = glm::normalize(T);

		tangent.push_back(T);
	}

	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);

	glGenBuffers(4, vbo);
	glGenBuffers(1, &this->ibo);


	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(glm::vec2), &t[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, n2.size() * sizeof(glm::vec3), &n2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(glm::vec3), &tangent[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Triangle), &tris[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Terrain::render() {
	glBindVertexArray(this->vao);

	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Terrain::release() {
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(4, vbo);
	glDeleteVertexArrays(1, &this->vao);
	heights.clear();
}

void Terrain::getHeights(std::vector<float>& hs) {
	hs.resize(this->width * this->height);

	for(int i = 0; i < heights.size(); i++) {
		hs[i] = (heights[i] / 255.0f) * this->heightFactor;
	}
}

float Terrain::getY(int x, int y) {
	float _y = 0.0f;

	x += this->width / 2;
	y += this->height / 2;


	//std::cout << x << ", " << y << std::endl;

	_y = (heights[y * this->width + x] / 255.0f) * this->heightFactor;

	return _y;
}

Uint32 Terrain::getWidth() {
	return this->width;
}

Uint32 Terrain::getHeight() {
	return this->height;
}

float Terrain::getHeighFactor() {
	return this->heightFactor;
}