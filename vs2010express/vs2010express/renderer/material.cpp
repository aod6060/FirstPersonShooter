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