#include "../system.h"

Texture::Texture() {
	this->id = 0;
	this->width = 0;
	this->height = 0;
}

void Texture::init(std::string fn, bool mipmaps) {
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

	int filter = GL_LINEAR;

	if(mipmaps) {
		filter = GL_LINEAR_MIPMAP_LINEAR;
	}

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
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

void Texture::init(int width, int height, Texture::TextureFormatType format) {
	this->width = width;
	this->height = height;

	int glformat = 0;
	int glformat2 = 0;
	int type = 0;

	if(format == Texture::RGB) {
		glformat = GL_RGB;
		glformat2 = GL_RGB;
		type = GL_UNSIGNED_BYTE;
	} else if(format == Texture::RGBA) {
		glformat = GL_RGBA;
		glformat2 = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	} else if(format == Texture::DEPTH) {
		glformat = GL_DEPTH_COMPONENT;
		glformat2 = GL_DEPTH_COMPONENT16;
		type = GL_FLOAT;
	}

	glGenTextures(1, &this->id);

	glBindTexture(GL_TEXTURE_2D, this->id);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		glformat2,
		this->width,
		this->height,
		0,
		glformat,
		type,
		0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(int tt) {
	glActiveTexture(tt);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() {
	return this->id;
}

void Texture::release() {
	glDeleteTextures(1, &id);
}
