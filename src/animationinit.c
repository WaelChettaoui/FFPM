#include "animationinit.h"

bool AnimationInit_start(SDL_Renderer *renderer, int width, int height)
{
    AnimationInit animationInit = {0};
    animationInit.renderer  = renderer;
    animationInit.width     = width;
    animationInit.height    = height;

    // Loat animation assets
    if(!AnimationInit_load(&animationInit))
        return false;


    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 60);

    // Initialize start frame
    int frame = SDL_getFramecount(&fpsmanager);

    // Event loop exit flag
    bool quit = false;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // Handle animation events
            quit = AnimationInit_handleEvent(&animationInit, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            AnimationInit_update(&animationInit, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }

        // Render animation
        AnimationInit_render(&animationInit);
        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);
        SDL_RenderClear(renderer);

    }

    AnimationInit_destroy(&animationInit);

    return true;
}

bool AnimationInit_load(AnimationInit *animationInit)
{
    // Load the ocean image
    if(!Background_load(animationInit->renderer, &animationInit->background, OCEAN_IMG))
        return false;

    // Background move direction
    animationInit->background.direction = DIRECTION_DOWN;

    // Background speed in pixel/second
    animationInit->background.speed = 0;

    return true;
}

void AnimationInit_destroy(AnimationInit *animationInit)
{
    Background_destroy(&animationInit->background);
}

bool AnimationInit_handleEvent(AnimationInit *animationInit, SDL_Event *e)
{
    bool quit = false;

    // User requests quit
    if(e->type == SDL_QUIT)
    {
        quit = true;
    }
    else if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_RETURN:
            quit = true;
            break;

        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:

            break;
        }
    }
    else if(e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:

            break;
        }
    }

    return quit;
}


void AnimationInit_update(AnimationInit *animationInit, int framerate)
{
    // Move ocean
    Background_move(&animationInit->background, framerate);
}

void AnimationInit_render(AnimationInit *animationInit)
{
    // Render background
    Background_render(animationInit->renderer, &animationInit->background, animationInit->width, animationInit->height);

}

