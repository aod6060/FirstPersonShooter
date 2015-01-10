#include "../system.h"

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