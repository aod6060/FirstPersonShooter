#include "../system.h"

// Renderer
Renderer* Renderer::instance = 0;

void Renderer::_release() {
	scene.release();
	ui.release();
}

Renderer* Renderer::getInstance() {

	if(Renderer::instance == 0) {
		Renderer::instance = new Renderer();
	}

	return Renderer::instance;
}

void Renderer::release() {
	Renderer::instance->_release();
	delete Renderer::instance;
	Renderer::instance = 0;
}

void Renderer::init() {
	// GL Enables
	glEnable(GL_DEPTH_TEST);

	// Init Shaders

	// Create Scene Shader
	scene.init("data/shaders/main.vert", "data/shaders/main.frag");
	scene.bind();
	scene.createUniform("Projection");
	scene.createUniform("View");
	scene.createUniform("Model");
	scene.createUniform("Normal");
	scene.createUniform("TextureMatrix");
	glm::mat4 tm = glm::scale(glm::vec3(-1.0f, 1.0f, 1.0f)) * glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	scene.setUniformMatrix4f("TextureMatrix", tm);
	scene.createUniform("CameraPosition");
	scene.createUniform("SelfPosition");
	for(int i = 0; i < 8; i++) {
		this->createLight(i);
	}

	this->createMaterial();
	this->createDepthBias();
	this->createShadow();

	scene.unbind();

	// Create UI Shader
	ui.init("data/shaders/ui.vert", "data/shaders/ui.frag");
	ui.bind();
	ui.createUniform("Projection");
	ui.createUniform("View");
	ui.createUniform("Model");
	ui.createUniform("tex0");
	ui.setUniform1i("tex0", 0);
	ui.unbind();

	this->shaderType = Renderer::SCENE;
}

void Renderer::startShader(ShaderTypes s) {
	this->shaderType = s;
	if(this->shaderType == Renderer::SCENE) {
		scene.bind();
	} else if(this->shaderType == Renderer::UI) {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ui.bind();
	}
}

void Renderer::endShader(ShaderTypes s) {
	this->shaderType = s;
	if(this->shaderType == Renderer::SCENE) {
		scene.unbind();
	} else if(this->shaderType == Renderer::UI) {
		ui.unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

void Renderer::setProjection(const glm::mat4& p) {
	if(this->shaderType == Renderer::SCENE) {
		scene.setUniformMatrix4f("Projection", p);
	} else if(this->shaderType == Renderer::UI) {
		ui.setUniformMatrix4f("Projection", p);
	}
}

void Renderer::setView(const glm::mat4& p) {
	if(this->shaderType == Renderer::SCENE) {
		scene.setUniformMatrix4f("View", p);
	} else if(this->shaderType == Renderer::UI) {
		ui.setUniformMatrix4f("View", p);
	}
}

void Renderer::setModel(const glm::mat4& w) {
	if(this->shaderType == Renderer::SCENE) {
		glm::mat4 n = glm::transpose(glm::inverse(w));
		scene.setUniformMatrix4f("Model", w);
		scene.setUniformMatrix4f("Normal", n);
		scene.setUniform3f("SelfPosition", glm::vec3(w[0][3], w[1][3], w[2][3]));
	} else if(this->shaderType == Renderer::UI) {
		ui.setUniformMatrix4f("Model", w);
	}
}

void Renderer::setCamera(Camera& cam) {
	if(this->shaderType == Renderer::SCENE) {
		glm::mat4 p, v;

		glm::vec3 pos;

		cam.render();

		pos = cam.getPos();

		cam.getProjectionMatrix(p);

		cam.getViewMatrix(v);

		scene.setUniformMatrix4f("Projection", p);

		scene.setUniformMatrix4f("View", v);

		scene.setUniform3f("CameraPosition", pos);
	} // UI doesn't make any since
}

void Renderer::setLight(Renderer::Lights lts, Light& l) {

	if(this->shaderType == Renderer::SCENE) {
		std::stringstream ss;
		std::stringstream ss2;
		ss << "lights[" << lts << "]";
		ss2 << ss.str() << ".enabled";
		scene.setUniform1i(ss2.str(), l.enabled);
		ss2.str("");
		ss2 << ss.str() << ".type";
		scene.setUniform1i(ss2.str(), l.type);
		ss2.str("");
		ss2 << ss.str() << ".position";
		scene.setUniform3f(ss2.str(), l.position);
		ss2.str("");
		ss2 << ss.str() << ".ambient";
		scene.setUniform3f(ss2.str(), l.ambient);
		ss2.str("");
		ss2 << ss.str() << ".diffuse";
		scene.setUniform3f(ss2.str(), l.diffuse);
		ss2.str("");
		ss2 << ss.str() << ".specular";
		scene.setUniform3f(ss2.str(), l.specular);
		ss2.str("");
		ss2 << ss.str() << ".radius";
		scene.setUniform1f(ss2.str(), l.radius);
		ss2.str("");
		ss2 << ss.str() << ".spotDirection";
		scene.setUniform3f(ss2.str(), l.spotDirection);
		ss2.str("");
		ss2 << ss.str() << ".spotExp";
		scene.setUniform1f(ss2.str(), l.spotExp);
		ss2.str("");
		ss2 << ss.str() << ".intensity";
		scene.setUniform1f(ss2.str(), l.intensity);
		ss2.str("");
	}
}

void Renderer::createLight(int i) {

	std::stringstream ss;
	std::stringstream ss2;

	ss << "lights[" << i << "]";
	ss2 << ss.str() << ".enabled";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".type";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".position";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".ambient";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".diffuse";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".specular";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".radius";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".spotDirection";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".spotExp";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << ".intensity";
	scene.createUniform(ss2.str());
	ss2.str("");
}

void Renderer::createMaterial() {
	std::stringstream ss;
	std::stringstream ss2;
	ss << "material";
	ss2 << ss.str() << ".albedo";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 0);
	ss2.str("");
	ss2 << ss.str() << ".normal";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 1);
	ss2.str("");
	ss2 << ss.str() << ".specular";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 2);
	ss2.str("");
	ss2 << ss.str() << ".emissive";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 3);
	ss2.str("");
	ss2 << ss.str() << ".alphaMask";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 4);
	ss2.str("");
}

/*
void Renderer::setDepthMatrix(glm::mat4 depthvp) {
	this->scene.setUniformMatrix4f("DepthBias", depthvp);
}
*/

void Renderer::setDepthMatrices(glm::mat4* depthvp) {
	std::stringstream ss;
	std::stringstream ss2;

	ss << "DepthBias[";

	ss2 << ss.str() << "0]";
	scene.setUniformMatrix4f(ss2.str(), depthvp[0]);
	ss2.str("");
	ss2 << ss.str() << "1]";
	scene.setUniformMatrix4f(ss2.str(), depthvp[1]);
	ss2.str("");
	ss2 << ss.str() << "2]";
	scene.setUniformMatrix4f(ss2.str(), depthvp[2]);
	ss2.str("");
	ss2 << ss.str() << "3]";
	scene.setUniformMatrix4f(ss2.str(), depthvp[3]);
	ss2.str("");

}

void Renderer::createDepthBias() {
	std::stringstream ss;
	std::stringstream ss2;

	ss << "DepthBias[";
	ss2 << ss.str() << "0]";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << "1]";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << "2]";
	scene.createUniform(ss2.str());
	ss2.str("");
	ss2 << ss.str() << "3]";
	scene.createUniform(ss2.str());
	ss2.str("");
}

void Renderer::createShadow() {
	std::stringstream ss;
	std::stringstream ss2;

	ss << "shadow[";
	ss2 << ss.str() << "0]";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 5);
	ss2.str("");
	ss2 << ss.str() << "1]";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 6);
	ss2.str("");
	ss2 << ss.str() << "2]";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 7);
	ss2.str("");
	ss2 << ss.str() << "3]";
	scene.createUniform(ss2.str());
	scene.setUniform1i(ss2.str(), 8);
	ss2.str("");
}
