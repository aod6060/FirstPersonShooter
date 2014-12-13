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

#ifndef GRAPHICS_H_
#define GRAPHICS_H_


/*
	Triangle
*/
struct Triangle {
	int v1;
	int v2;
	int v3;
};

/*
	BiTriangle
*/
struct BiTriangle {
	Triangle t1;
	Triangle t2;
};
/*
	Shader

	This is a the basic shader class 
	for this project. I'm going to 
	implement a better one later.
*/
class Shader {
	GLuint vid;
	GLuint fid;
	GLuint pid;

	std::map<std::string, GLuint> uniforms;
	// This simply grabs file content from the shader file
	std::string getFileContent(std::string fn);

	// This checks to see if a shader has errors
	bool printShaderError(GLuint id);

public:

	Shader();

	// inits the vertex and fragment shader
	// for now I'm not worried about 
	// the geometrey shader
	void init(std::string vfn, std::string ffn);

	// This binds the shader
	void bind();

	// This unbinds the shader
	void unbind();

	// This destroyes the shader
	void release();

	// This creates a link to a uniform variable
	// located in the shader
	void createUniform(std::string name);

	// This sets a single uniform integer
	void setUniform1i(std::string name, int i);

	// This sets a double uniform integer
	void setUniform2i(std::string name, const glm::ivec2& v);

	// This sets a triple uniform integer
	void setUniform3i(std::string name, const glm::ivec3& v);

	// This sets a quataple uniform integer
	void setUniform4i(std::string name, const glm::ivec4& v);

	// This sets a single uniform float
	void setUniform1f(std::string name, float i);

	// This sets a double uniform float
	void setUniform2f(std::string name, const glm::vec2& v);

	// This sets a triple uniform float
	void setUniform3f(std::string name, const glm::vec3& v);

	// This sets a quataple uniform float
	void setUniform4f(std::string name, const glm::vec4& v);

	// This sets a uniform 2 dimensional matrix
	void setUniformMatrix2f(std::string name, const glm::mat2& m);
	
	// This sets a uniform 3 dimensional matrix
	void setUniformMatrix3f(std::string name, const glm::mat3& m);
	
	// This sets a uniform 4 dimensional matrix
	void setUniformMatrix4f(std::string name, const glm::mat4& m);
};

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
	GLuint vbo[3];

	GLuint size;

	int version;

	enum StaticMeshSections {
		NONE = 0,
		TRANSFORM,
		VERTEX,
		NORMAL,
		TEXCOORD,
		FACE
	};

public:

	StaticMesh();

	void init(std::string fn);

	void render();

	void release();

	int getVersion();

};

/*
	Texture

	This is a simple 2D texture wrapper
*/
class Texture {
	GLuint id;
	GLuint width, height;

public:

	Texture();

	void init(std::string fn);

	void bind(int tt = GL_TEXTURE0);

	void unbind();

	void release();
};

#endif