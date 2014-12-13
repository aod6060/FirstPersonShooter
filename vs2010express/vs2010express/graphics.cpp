#include "system.h"

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
	this->uniforms[name] = glGetUniformLocation(this->pid, name.c_str());
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

// StaticMesh
StaticMesh::StaticMesh() {
	this->vao = 0;
	this->size = 0;
}

void StaticMesh::init(std::string fn) {
	std::vector<glm::vec4> tran;
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec2> t;
	std::vector<BiTriangle> bt;


	std::ifstream in(fn);

	std::string temp;

	std::string cmd;

	StaticMesh::StaticMeshSections sms = StaticMesh::NONE;

	std::cout << "Hello 1" << std::endl;

	while(std::getline(in, temp)) {
		std::stringstream ss(temp);
		std::stringstream ss2(temp);
		ss >> cmd;

		if(cmd == "version") {
			ss >> this->version;
			continue;
		} else if(cmd == "transform") {
			sms = StaticMesh::TRANSFORM;
			continue;
		} else if(cmd == "vertex") {
			sms = StaticMesh::VERTEX;
			continue;
		} else if(cmd == "normal") {
			sms = StaticMesh::NORMAL;
			continue;
		} else if(cmd == "texCoord") {
			sms = StaticMesh::TEXCOORD;
			continue;
		} else if(cmd == "face") {
			sms = StaticMesh::FACE;
			continue;
		} else if(cmd == "end") {
			sms = StaticMesh::NONE;
			continue;
		}


		if(sms == StaticMesh::TRANSFORM) {
			glm::vec4 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;
			ss2 >> tt.w;

			tran.push_back(tt);
		} else if(sms == StaticMesh::VERTEX) {
			glm::vec3 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;

			v.push_back(tt);
		} else if(sms == StaticMesh::NORMAL) {
			glm::vec3 tt;

			ss2 >> tt.x;
			ss2 >> tt.y;
			ss2 >> tt.z;

			n.push_back(tt);
		} else if(sms == StaticMesh::TEXCOORD) {
			glm::vec2 tt;
			ss2 >> tt.x;
			ss2 >> tt.y;
			std::cout << "What" << std::endl;

			t.push_back(tt);
		} else if(sms == StaticMesh::FACE) {
			BiTriangle tt;
			// Vertex-Normal
			ss2 >> tt.t1.v1;
			ss2 >> tt.t1.v2;
			ss2 >> tt.t1.v3;
			// Texture Coord
			ss2 >> tt.t2.v1;
			ss2 >> tt.t2.v2;
			ss2 >> tt.t2.v3;
			bt.push_back(tt);
		}
	}

	std::cout << "Hello 2" << std::endl;
	glm::mat4 mat(
		tran[0],
		tran[1],
		tran[2],
		tran[3]);


	glm::mat4 imat = glm::transpose(glm::inverse(mat));

	glm::mat3 imat3x3 = glm::make_mat3(&imat[0][0]);


	for(int i = 0; i < v.size(); i++) {
		glm::vec4 tv(v[i], 1.0);

		tv = mat * tv;

		v[i] = glm::vec3(tv.x, tv.y, tv.z);

		n[i] = imat3x3 * n[i];
	}

	std::cout << "Hello 4" << std::endl;
	std::vector<glm::vec3> vlist;
	std::vector<glm::vec3> nlist;
	std::vector<glm::vec2> tlist;

	for(int i = 0; i < bt.size(); i++) {
		std::cout << "vertex" << i << std::endl;
		vlist.push_back(v[bt[i].t1.v1]);
		vlist.push_back(v[bt[i].t1.v2]);
		vlist.push_back(v[bt[i].t1.v3]);
		std::cout << "normal" << i << std::endl;
		nlist.push_back(n[bt[i].t1.v1]);
		nlist.push_back(n[bt[i].t1.v2]);
		nlist.push_back(n[bt[i].t1.v3]);
		std::cout << "texCoord" << i << std::endl;
		std::cout << t.size() << std::endl;

		std::cout << bt[i].t2.v1 << ", " << bt[i].t2.v2 << ", " << bt[i].t2.v3 << std::endl;

		tlist.push_back(t[bt[i].t2.v1]);
		tlist.push_back(t[bt[i].t2.v2]);
		tlist.push_back(t[bt[i].t2.v3]);
		std::cout << "end" << i << std::endl;
	}

	std::cout << "Hello 5" << std::endl;
	this->size = vlist.size();

	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);

	glGenBuffers(3, this->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vlist.size() * sizeof(glm::vec3),&vlist[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nlist.size() * sizeof(glm::vec3), &nlist[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tlist.size() * sizeof(glm::vec2), &tlist[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);

	std::cout << "Hello 6" << std::endl;
}

void StaticMesh::render() {
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->size);
	glBindVertexArray(0);
}

void StaticMesh::release() {
	glDeleteBuffers(3, this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

int StaticMesh::getVersion() {
	return this->version;
}


Texture::Texture() {
	this->id = 0;
	this->width = 0;
	this->height = 0;
}

void Texture::init(std::string fn) {
	SDL_Surface* temp;

	temp = IMG_Load(fn.c_str());


	if(temp == 0) {
		std::cout << fn << ": doesn't exist" << std::endl;
		return;
	}

	this->width = temp->w;
	this->height = temp->h;

	GLint format = GL_RGB;

	if(temp->format->BytesPerPixel == 4) {
		format = GL_RGBA;
	}

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		this->width,
		this->height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		temp->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(temp);
}

void Texture::bind(int tt) {
	glActiveTexture(tt);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::release() {
	glDeleteTextures(1, &id);
}