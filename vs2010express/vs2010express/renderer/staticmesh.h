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
#ifndef STATICMESH_H
#define STATICMESH_H

/*
	StaticMesh

	This is a simple static mesh loader. It loads a custom
	format that is specified in the script folder. The reason
	I'm using a custom format is to change it up for the game
	This will also be appied to the animated format when
	I derive it.
*/
class StaticMesh {
	GLuint vao;
	GLuint vbo[4];

	GLuint size;

	int version;
	
	std::vector<glm::vec3> verticies;

	enum StaticMeshSections {
		NONE = 0,
		TRANSFORM,
		VERTEX,
		FACE_NORMAL,
		TEXCOORD,
		FACE
	};

public:

	StaticMesh();

	void init(std::string fn, bool facens = false);

	void render();

	void release();

	int getVersion();

	void getVerticies(std::vector<glm::vec3>& v);

};

#endif