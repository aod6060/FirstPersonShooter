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
#ifndef RENDERER_H
#define RENDERER_H

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

	void createDepthBias();

	void createShadow();

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

	//void setDepthMatrix(glm::mat4 depthvp);

	void setDepthMatrices(glm::mat4* depthvp);
};

#endif