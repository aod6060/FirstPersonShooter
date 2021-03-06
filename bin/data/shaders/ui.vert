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
/*
	ui.vert
	
	This file contains vertex shader for the ui component for my 
	first person shooter game
*/

#version 330 core
// Matrix used to transform from camera space to clip space
uniform mat4 Projection;
// Matrix used to transform from world space to camera space
uniform mat4 View;
// Matrix used to transform from local space to world space
uniform mat4 Model;

// This is the layout for vertex
layout(location = 0) in vec3 in_Vertex;

// This is the layout for texCoord0
layout(location = 1) in vec2 in_TexCoord0;

// This goes to ui.frag
out vec2 pass_TexCoord0;


void main() {
	gl_Position = Projection * View * Model * vec4(in_Vertex, 1.0);
	pass_TexCoord0 = in_TexCoord0;
}