#include "animationcredits.h"

bool AnimationCredits_start(SDL_Renderer *renderer, int width, int height)
{
    AnimationCredits animationCredits = {0};
    animationCredits.renderer  = renderer;
    animationCredits.width     = width;
    animationCredits.height    = height;

    // Loat animation assets
    if(!AnimationCredits_load(&animationCredits))
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
            quit = AnimationCredits_handleEvent(&animationCredits, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            AnimationCredits_update(&animationCredits, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }

        // Render animation
        AnimationCredits_render(&animationCredits);
        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);
        SDL_RenderClear(renderer);

    }

    AnimationCredits_destroy(&animationCredits);

    return true;
}

bool AnimationCredits_load(AnimationCredits *animationCredits)
{
    // Load the ocean image
    if(!Background_load(animationCredits->renderer, &animationCredits->background, OCEAN_IMG))
        return false;

    // Background move direction
    animationCredits->background.direction = DIRECTION_DOWN;

    // Background speed in pixel/second
    animationCredits->background.speed = 0;

    return true;
}

void AnimationCredits_destroy(AnimationCredits *animationCredits)
{
    Background_destroy(&animationCredits->background);
}

bool AnimationCredits_handleEvent(AnimationCredits *animationCredits, SDL_Event *e)
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


void AnimationCredits_update(AnimationCredits *animationCredits, int framerate)
{
    // Move ocean
    Background_move(&animationCredits->background, framerate);
}

void AnimationCredits_render(AnimationCredits *animationCredits)
{
    // Render background
    Background_render(animationCredits->renderer, &animationCredits->background, animationCredits->width, animationCredits->height);

}

