#include "animationquit.h"

bool AnimationQuit_start(SDL_Renderer *renderer, int width, int height)
{
    AnimationQuit animationQuit = {0};
    animationQuit.renderer  = renderer;
    animationQuit.width     = width;
    animationQuit.height    = height;

    // Loat animation assets
    if(!AnimationQuit_load(&animationQuit))
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
            quit = AnimationQuit_handleEvent(&animationQuit, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            AnimationQuit_update(&animationQuit, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }

        // Render animation
        AnimationQuit_render(&animationQuit);
        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);
        SDL_RenderClear(renderer);

    }

    AnimationQuit_destroy(&animationQuit);

    return true;
}

bool AnimationQuit_load(AnimationQuit *animationQuit)
{
    // Load the ocean image
    if(!Background_load(animationQuit->renderer, &animationQuit->background, OCEAN_IMG))
        return false;

    // Background move direction
    animationQuit->background.direction = DIRECTION_DOWN;

    // Background speed in pixel/second
    animationQuit->background.speed = 0;

    return true;
}

void AnimationQuit_destroy(AnimationQuit *animationQuit)
{
    Background_destroy(&animationQuit->background);
}

bool AnimationQuit_handleEvent(AnimationQuit *animationQuit, SDL_Event *e)
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
        case SDLK_ESCAPE:
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


void AnimationQuit_update(AnimationQuit *animationQuit, int framerate)
{
    // Move ocean
    Background_move(&animationQuit->background, framerate);
}

void AnimationQuit_render(AnimationQuit *animationQuit)
{
    // Render background
    Background_render(animationQuit->renderer, &animationQuit->background, animationQuit->width, animationQuit->height);

}

