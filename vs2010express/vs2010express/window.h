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

#ifndef WINDOW_H_
#define WINDOW_H_


/*
	WindowCallback

	This is a simple window callback.
*/
class WindowCallback {
public:

	virtual void init() = 0;
	
	virtual void update() = 0;

	virtual void release() = 0;
};

/*
	WindowConfig

	This is a simple configuration for the window
*/
struct WindowConfig {
	std::string caption;
	Uint32 width;
	Uint32 height;
	bool vsync;
	WindowCallback* callback;

	WindowConfig(
		std::string caption, 
		Uint32 width, 
		Uint32 height,
		bool vsync,
		WindowCallback* callback = 0) {
			this->caption = caption;
			this->width = width;
			this->height = height;
			this->vsync = vsync;
			this->callback = callback;
	}
};


/*
	WindowManager

	This manager is used to manage the window and inputs. It wraps
	up SDL realed stuff. Note its a singleton class.
*/
class WindowManager {
	static WindowManager* instance;

	WindowConfig* conf;

	bool running;

	SDL_Window* window;

	SDL_GLContext context;

	// Keyboard
	const Uint8* keys;
	bool hits[SDL_NUM_SCANCODES];

	// Mouse
	Uint32 buttons;
	bool buttonHits[3];
	int mx, my;

	// Keyboard
	void initKeyboard();
	void updateKeyboard();

	// Mouse
	void initMouse();
	void updateMouse();

	void _release();

	// This class is a singleton
	WindowManager() {}
	WindowManager(WindowManager&) {}
	WindowManager& operator = (WindowManager&) {}


public:

	static WindowManager* getInstance();

	static void release();


	void init(WindowConfig* conf);

	void mainLoop();

	Uint32 getWidth();

	Uint32 getHeight();

	float getWidthf();

	float getHeightf();

	void quit();

	// Keyboard Section
	bool keyPress(int key);

	bool keyHit(int key);

	// Mouse Section
	void getMouseCoord(int* mx, int* my);

	bool mouseButtonPress(int b);

	bool mouseButtonHit(int b);

	void setRelativeMouseMode(SDL_bool b);

	SDL_bool getRelativeMouseMode();

};

#endif