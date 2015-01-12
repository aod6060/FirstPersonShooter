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
#include "../system.h"

// Cubemap
Cubemap::Cubemap() {
	this->id = 0;
}

void Cubemap::init(
	std::string px,
	std::string nx,
	std::string py,
	std::string ny,
	std::string pz,
	std::string nz
	) {
	std::vector<std::string> images;
	std::vector<int> types;
	// Add images to images vector
	images.push_back(px);
	images.push_back(nx);
	images.push_back(py);
	images.push_back(ny);
	images.push_back(pz);
	images.push_back(nz);
	// Adding Cubemap types to 
	types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	// createing texture
	glGenTextures(1, &this->id);
	// bind cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

	// Run through vectors and load up information
	// to the cubemap
	for(int i = 0; i < images.size(); i++) {
		SDL_Surface* surf = 0; // add sdl surface

		surf = IMG_Load(images[i].c_str()); // Load Image Via SDL_image

		if(surf == 0) { // Check to see if images exists
			continue;
		}

		int format = GL_RGB;

		if(surf->format->BytesPerPixel == 4) {
			format = GL_RGBA;
		}

		glTexImage2D(
			types[i],
			0,
			format,
			surf->w,
			surf->h,
			0,
			format,
			GL_UNSIGNED_BYTE,
			surf->pixels);

		SDL_FreeSurface(surf); // free surface
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// unbind cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::bind(int tt) {
	glActiveTexture(tt);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
}

void Cubemap::unbind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::release() {
	glDeleteTextures(1, &this->id);
}