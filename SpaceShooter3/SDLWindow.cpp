#include "my_pch.h"

#include "SDLWindow.h"

//The window we'll be rendering to
SDL_Window* window = nullptr;

//The surface contained by the window
SDL_Surface* window_surface = nullptr;

//The window renderer
extern SDL_Renderer* window_renderer = nullptr;


void SDLInit() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            //Create renderer for window
            window_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (window_renderer == nullptr)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());                
                
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(window_renderer, 0xFF, 0xFF, 0xFF, 0xFF);                
            }

            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                assert(false);
            }
            else
            {
                //Get window surface
                window_surface = SDL_GetWindowSurface(window);
            }
            
            //Update the surface
            SDL_UpdateWindowSurface(window);            
        }
    }
}

void SDLQuit()
{
    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();
}
