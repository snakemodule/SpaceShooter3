#pragma once
#include "my_pch.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
extern SDL_Window* window;

//The surface contained by the window
extern SDL_Surface* window_surface;

//The window renderer
extern SDL_Renderer* window_renderer;

void SDLInit();

void SDLQuit();