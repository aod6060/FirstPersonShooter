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
