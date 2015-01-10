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
#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

class AnimatedMesh {
	GLuint vao;
	GLuint vbo[4];

	GLuint size;

	int version;

	struct AnimatedMeshBone {
		int id;
		std::string name;
	};

	// This is for bones (note: I'm not real sure what I need this for)
	std::vector<AnimatedMesh::AnimatedMeshBone> bones;

	// Vertex Section
	std::vector<int> boneIDs; // This is for the animation part
	std::vector<glm::vec3> vertex; // This is the default vertices
	std::vector<glm::vec3> normals; // This is the default normals

	// TexCoord Section
	std::vector<glm::vec2> texCoords;

	// Face
	std::vector<BiTriangle> btris;

	enum AnimatedMeshSection {
		NONE = 0,
		BONE,
		TRANSFORM,
		VERTEX,
		TEXCOORD,
		FACE
	};


	void updateVertexArray();
public:

	AnimatedMesh();

	void init(std::string fn);

	void render();

	void release();

	int getVersion();

};

#endif