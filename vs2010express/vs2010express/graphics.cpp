#include "system.h"


// Normal
void Normal::addFaceNoraml(glm::vec3 fn) {
	this->fnorml.push_back(fn);
}

void Normal::generate() {
	this->vnormal = glm::vec3(0.0f);

	for(int i = 0; i < fnorml.size(); i++) {
		this->vnormal += fnorml[i];
	}

	this->vnormal = glm::normalize(this->vnormal);
}

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

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

// Font
void Font::_updateBuffer(std::string str) {


	if(this->str != str) {
		this->str = str;

		if(this->str.empty()) {
			this->str = " ";
		}

		SDL_Color c;

		c.r = 255;
		c.g = 255;
		c.b = 255;
		c.a = 255;

		SDL_Surface* temp = ::TTF_RenderText_Blended(this->font, str.c_str(), c);

		this->width = temp->w;
		this->height = temp->h;

		int format = GL_RGB;

		if(temp->format->BytesPerPixel == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(temp);
	}
}

Font::Font() {
	this->font = 0;
	this->id = 0;
	this->str = " ";
}

void Font::init() {

	float vlist[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	float tlist[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->vao);
	
	glBindVertexArray(this->vao);

	glGenBuffers(2, this->vbo);

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

	glGenTextures(1, &this->id);
	this->font = TTF_OpenFont("data/ttf/VeraBd.ttf", 16);
}

void Font::renderString(const char* format, ...) {
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	this->_updateBuffer(buffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glBindVertexArray(this->vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::release() {
	TTF_CloseFont(this->font);
	glDeleteTextures(1, &this->id);
	glDeleteBuffers(2, this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

int Font::getWidth() {
	return this->width;
}

int Font::getHeight() {
	return this->height;
}

// Terrain
Terrain::Terrain() {
	this->width = 0;
	this->height = 0;
}

void Terrain::init(std::string fn) {
	SDL_Surface* surf = 0;
	surf = IMG_Load(fn.c_str());

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
			tempv.y = (this->heights[y * this->width + x] / 255.0f) * 32.0f;
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

		n[tris[i].v1].addFaceNoraml(N);
		n[tris[i].v2].addFaceNoraml(N);
		n[tris[i].v3].addFaceNoraml(N);
	}

	for(int i = 0; i < n.size(); i++) {
		n[i].generate();
		n2[i] = n[i].vnormal;
	}

	glGenVertexArrays(1, &this->vao);

	glBindVertexArray(this->vao);

	glGenBuffers(3, vbo);
	glGenBuffers(1, &this->ibo);


	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, n2.size() * sizeof(glm::vec3), &n2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(glm::vec2), &t[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Triangle), &tris[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Terrain::render() {
	glBindVertexArray(this->vao);

	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Terrain::release() {
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &this->vao);
	heights.clear();
}

// Camera
Camera::Camera() {

}

void Camera::init(glm::vec3 pos, glm::vec2 rot, float fov, float znear, float zfar) {
	this->pos = pos;
	this->rot = rot;

	this->proj = glm::perspective(
					glm::radians(fov),
					WindowManager::getInstance()->getWidthf() / WindowManager::getInstance()->getHeightf(),
					znear,
					zfar);


	this->view = glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));
}

void Camera::render() {
	this->view = glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
				 glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
				 glm::translate(-pos);
}

void Camera::update() {


	WindowManager* wm = WindowManager::getInstance();

	if(wm->getRelativeMouseMode()) {
		int mx, my;

		wm->getMouseCoord(&mx, &my);

		this->rot.x += my * 0.1f;
		this->rot.y += mx * 0.1f;



		float yrad = glm::radians(rot.y);

		if(wm->keyPress(SDL_SCANCODE_W)) {
			pos.x += glm::sin(yrad);
			pos.z -= glm::cos(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_S)) {
			pos.x -= glm::sin(yrad);
			pos.z += glm::cos(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_A)) {
			pos.x -= glm::cos(yrad);
			pos.z -= glm::sin(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_D)) {
			pos.x += glm::cos(yrad);
			pos.z += glm::sin(yrad);
		}

		if(wm->keyPress(SDL_SCANCODE_Z)) {
			pos.y -= 1.0;
		}

		if(wm->keyPress(SDL_SCANCODE_X)) {
			pos.y += 1.0;
		}
	}
}

void Camera::getViewMatrix(glm::mat4& v) {
	v = this->view;
}

void Camera::getProjectionMatrix(glm::mat4& p) {
	p = this->proj;
}

glm::vec3 Camera::getPos() {
	return this->pos;
}

glm::vec2 Camera::getRot() {
	return rot;
}

void Camera::setPos(glm::vec3 p) {
	this->pos = p;
}

void Camera::setRot(glm::vec2 r) {
	this->rot = r;
}
