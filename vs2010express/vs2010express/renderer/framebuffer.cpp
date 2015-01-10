#include "../system.h"

// Framebuffer
Framebuffer::Framebuffer() {
	this->id = 0;
}

void Framebuffer::create() {
	glGenFramebuffers(1, &this->id);
}

void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
	
void Framebuffer::attachTexture(FrameBufferAttachType type, Texture& t) {
	switch(type) {
	case Framebuffer::DEPTH:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, t.getID(), 0);
		break;
	case Framebuffer::COLOR0:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, t.getID(), 0);
		break;
	case Framebuffer::COLOR1:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, t.getID(), 0);
		break;
	case Framebuffer::COLOR2:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, t.getID(), 0);
		break;
	case Framebuffer::COLOR3:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, t.getID(), 0);
		break;
	case Framebuffer::COLOR4:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, t.getID(), 0);
		break;
	case Framebuffer::COLOR5:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, t.getID(), 0);
		break;
	case Framebuffer::COLOR6:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, t.getID(), 0);
		break;
	case Framebuffer::COLOR7:
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, t.getID(), 0);
		break;
	}
}

void Framebuffer::release() {
	glDeleteFramebuffers(1, &this->id);
}
