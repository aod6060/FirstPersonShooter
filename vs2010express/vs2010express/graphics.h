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
	Normal
*/
struct Normal {
	glm::vec3 vnormal;
	std::vector<glm::vec3> fnorml;

	void addFaceNoraml(glm::vec3 fn);

	void generate();

};

/*
	Light
*/
struct Light {
	int enabled;
	int type;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float radius;
	glm::vec3 spotDirection;
	float spotExp;
	float intensity;
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
	GLuint vbo[4];

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

/*
	Texture

	This is a simple 2D texture wrapper
*/
class Texture {
	GLuint id;
	GLuint width, height;

public:

	enum TextureFormatType {
		RGB = 0,
		RGBA,
		DEPTH,
		TFT_SIZE
	};

	Texture();

	void init(std::string fn, bool mipmaps = true);

	void init(int width, int height, TextureFormatType format);

	void bind(int tt = GL_TEXTURE0);

	void unbind();

	GLuint getID();

	void release();
};

/*
	Cubemap

	This is a simple cube map loader.
*/
class Cubemap {
	GLuint id;
public:
	Cubemap();

	void init(
		std::string px,
		std::string nx,
		std::string py,
		std::string ny,
		std::string pz,
		std::string nz
		);

	void bind(int tt = GL_TEXTURE0);

	void unbind();

	void release();

};

/*
	Framebuffer - This class wraps up the framebuffer.
*/
class Framebuffer {
	GLuint id;

	int width;
	int height;

public:

	enum FrameBufferAttachType {
		DEPTH = 0,
		COLOR0,
		COLOR1,
		COLOR2,
		COLOR3,
		COLOR4,
		COLOR5,
		COLOR6,
		COLOR7,
		FBAT_SIZE
	};

	Framebuffer();

	void create();

	void bind();

	void unbind();
	
	void attachTexture(FrameBufferAttachType type, Texture& t);

	void release();
};

/*
	Font

	This is the font class.
*/
class Font {
	GLuint id;
	GLuint width, height;
	GLuint vao;
	GLuint vbo[2];

	std::string str;

	TTF_Font* font;

	void _updateBuffer(std::string str);
public:

	Font();

	void init();

	void renderString(const char* format, ...);

	void release();

	int getWidth();

	int getHeight();
};

/*
	DrawSurface
*/
class DrawSurface {
	GLuint vao;
	GLuint vbo[2];
public:

	void init();

	void render();

	void release();
};

/*
	Terrain

	This is the terrain render class
*/
class Terrain {
	std::vector<Uint8> heights;

	Uint32 width;
	Uint32 height;


	GLuint vao;

	GLuint vbo[4];
	GLuint ibo;

	Uint32 count;

public:

	Terrain();

	void init(std::string fn);

	void render();

	void release();
};

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


class Material {
	Texture albedo;
	Texture normal;
	Texture specular;
	Texture emissive;
	Texture alphaMask;

	std::string albedofn;
	std::string normalfn;
	std::string specularfn;
	std::string emissivefn;
	std::string alphaMaskfn;

public:

	Material();

	void init();

	void bind();

	void unbind();

	void release();

	void setAlbedoFilename(std::string fn);

	void setNormalFilename(std::string fn);

	void setSpecularFilename(std::string fn);

	void setEmissiveFilename(std::string fn);

	void setAlphaMaskFilename(std::string fn);
};

/*
	Transform - This class is used for transforming 
	objects. 
*/
class Transform {
	glm::vec3 position;
	glm::vec3 rotation;

public:

	void setPosition(glm::vec3 position);

	glm::vec3 getPosition();

	void setRotation(glm::vec3 rotation);

	glm::vec3 getRotation();

	void getModel(glm::mat4& m);

};

/*
	This is the renderer for the game engine.

	This pretty much wrapps the shader code for

	the project.
*/
class Renderer {
	static Renderer* instance;

	Shader scene;
	Shader ui;

	Renderer() {}
	Renderer(Renderer&) {}
	Renderer& operator = (Renderer&) {}

	void _release();
public:
	/*
		This is for choosing which shader
		to use
	*/
	enum ShaderTypes {
		SCENE = 0,
		UI,
		SHADER_TYPE_SIZE
	};

	enum LightType {
		DIRECTION = 0,
		POINT,
		SPOT,
		LIGHT_TYPE_SIZE
	};

	enum Lights {
		LIGHT0 = 0,
		LIGHT1,
		LIGHT2,
		LIGHT3,
		LIGHT4,
		LIGHT5,
		LIGHT6,
		LIGHT7,
		LIGHT_SIZE
	};

private:

	ShaderTypes shaderType;

	Lights lights;

	void createLight(int i);

	void createMaterial();

public:


	static Renderer* getInstance();

	static void release();

	void init();

	void startShader(ShaderTypes s);

	void endShader(ShaderTypes s);

	void setProjection(const glm::mat4& p);

	void setView(const glm::mat4& p);

	void setModel(const glm::mat4& w);

	void setCamera(Camera& cam);

	void setLight(Renderer::Lights lts, Light& l);

	void setDepthMatrix(glm::mat4 depthvp);

};

#endif