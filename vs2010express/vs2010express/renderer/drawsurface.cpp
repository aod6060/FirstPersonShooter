#include "../system.h"

// DrawSurface
void DrawSurface::init() {
	GLfloat vlist[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	GLfloat tlist[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vlist), vlist, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tlist), tlist, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void DrawSurface::render() {
	glBindVertexArray(this->vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void DrawSurface::release() {
	glDeleteBuffers(2, this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}