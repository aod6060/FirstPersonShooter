#include "../system.h"

// Shader
Shader::Shader() {
	this->fid = 0;
	this->vid = 0;
	this->pid = 0;
}

// This simply grabs file content from the shader file
std::string Shader::getFileContent(std::string fn) {
	std::ifstream in(fn);
	std::stringstream ss;
	std::string temp;
	while(std::getline(in, temp)) {
		ss << temp << std::endl;
	}
	return ss.str();
}

// This checks to see if a shader has errors
bool Shader::printShaderError(GLuint id) {
	int len;
	int ch;
	char c[256];
	bool b = false;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

	if(len > 0) {
		glGetShaderInfoLog(id, len, 0, c);
		std::cout << c << std::endl;
		b = true;
	}

	return b;
}

// inits the vertex and fragment shader
// for now I'm not worried about 
// the geometrey shader
void Shader::init(std::string vfn, std::string ffn) {
	this->vid = glCreateShader(GL_VERTEX_SHADER);
	this->fid = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vs = this->getFileContent(vfn);
	std::string fs = this->getFileContent(ffn);
	const char* vsc = vs.c_str();
	const char* fsc = fs.c_str();
	glShaderSource(this->vid, 1, &vsc, 0);
	glShaderSource(this->fid, 1, &fsc, 0);
	glCompileShader(this->vid);
	this->printShaderError(this->vid);
	glCompileShader(this->fid);
	this->printShaderError(this->fid);
	this->pid = glCreateProgram();
	glAttachShader(this->pid, this->vid);
	glAttachShader(this->pid, this->fid);
	glLinkProgram(this->pid);
}

// This binds the shader
void Shader::bind() {
	glUseProgram(this->pid);
}

// This unbinds the shader
void Shader::unbind() {
	glUseProgram(0);
}

// This destroyes the shader
void Shader::release() {
	glDetachShader(this->pid, this->vid);
	glDetachShader(this->pid, this->fid);
	glDeleteProgram(this->pid);
	glDeleteShader(this->fid);
	glDeleteShader(this->vid);
}

// This creates a link to a uniform variable
// located in the shader
void Shader::createUniform(std::string name) {
	int value = glGetUniformLocation(this->pid, name.c_str());
	std::cout << "Uniform ID: " << value << " Name: " << name << std::endl;
	this->uniforms[name] = value;
}

// This sets a single uniform integer
void Shader::setUniform1i(std::string name, int i) {
	glUniform1i(this->uniforms[name], i);
}

// This sets a double uniform integer
void Shader::setUniform2i(std::string name, const glm::ivec2& v) {
	glUniform2i(this->uniforms[name], v.x, v.y);
}

// This sets a triple uniform integer
void Shader::setUniform3i(std::string name, const glm::ivec3& v) {
	glUniform3i(this->uniforms[name], v.x, v.y, v.z);
}

// This sets a quataple uniform integer
void Shader::setUniform4i(std::string name, const glm::ivec4& v) {
	glUniform4i(this->uniforms[name], v.x, v.y, v.z, v.w);
}

// This sets a single uniform float
void Shader::setUniform1f(std::string name, float i) {
	glUniform1f(this->uniforms[name], i);
}

// This sets a double uniform float
void Shader::setUniform2f(std::string name, const glm::vec2& v) {
	glUniform2f(this->uniforms[name], v.x, v.y);
}

// This sets a triple uniform float
void Shader::setUniform3f(std::string name, const glm::vec3& v) {
	glUniform3f(this->uniforms[name], v.x, v.y, v.z);
}

// This sets a quataple uniform float
void Shader::setUniform4f(std::string name, const glm::vec4& v) {
	glUniform4f(this->uniforms[name], v.x, v.y, v.z, v.w);
}

// This sets a uniform 2 dimensional matrix
void Shader::setUniformMatrix2f(std::string name, const glm::mat2& m) {
	glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}
	
// This sets a uniform 3 dimensional matrix
void Shader::setUniformMatrix3f(std::string name, const glm::mat3& m) {
	glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}
	
// This sets a uniform 4 dimensional matrix
void Shader::setUniformMatrix4f(std::string name, const glm::mat4& m) {
	glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}