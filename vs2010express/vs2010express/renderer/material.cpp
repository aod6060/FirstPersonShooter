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

// Material
Material::Material() {
	this->albedofn = "data/texture/def_albedo.png";
	this->normalfn = "data/texture/def_normal.png";
	this->specularfn = "data/texture/def_specular.png";
	this->emissivefn = "data/texture/def_emissive.png";
	this->alphaMaskfn = "data/texture/def_alphaMask.png";
}

void Material::init() {
	albedo.init(albedofn);
	normal.init(normalfn);
	specular.init(specularfn);
	emissive.init(emissivefn, false);
	alphaMask.init(alphaMaskfn, false);
}

void Material::bind() {
	albedo.bind(GL_TEXTURE0);
	normal.bind(GL_TEXTURE1);
	specular.bind(GL_TEXTURE2);
	emissive.bind(GL_TEXTURE3);
	alphaMask.bind(GL_TEXTURE4);
}

void Material::unbind() {
	alphaMask.unbind();
	emissive.unbind();
	specular.unbind();
	normal.unbind();
	albedo.unbind();
}

void Material::release() {
	alphaMask.release();
	emissive.release();
	specular.release();
	normal.release();
	albedo.release();
}

void Material::setAlbedoFilename(std::string fn) {
	this->albedofn = fn;
}

void Material::setNormalFilename(std::string fn) {
	this->normalfn = fn;
}

void Material::setSpecularFilename(std::string fn) {
	this->specularfn = fn;
}

void Material::setEmissiveFilename(std::string fn) {
	this->emissivefn = fn;
}

void Material::setAlphaMaskFilename(std::string fn) {
	this->alphaMaskfn = fn;
}