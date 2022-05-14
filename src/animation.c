/*
 * Copyright (c) 2018 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "animation.h"
#include "Dot.h"
#include "utils.h"
#include "SDL_log.h"
#include <SDL_ttf.h>

/*void Display_text(SDL_Renderer* renderer, char* text)
{
    TTF_Font* Sans = TTF_OpenFont("arial.ttf", 50); //this opens a font style and sets a size

    SDL_Color White = {0, 0, 0, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "You died", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}
*/

bool Animation_start(SDL_Renderer *renderer, int width, int height)
{
    Animation animation = {0};
    animation.renderer  = renderer;
    animation.width     = width;
    animation.height    = height;

    // Loat animation assets
    if(!Animation_load(&animation))
        return false;


    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 60);

    // Initialize start frame
    int frame = SDL_getFramecount(&fpsmanager);


    // Event loop exit flag
    bool quit = false;
    animation.gameover = false;
    // Event loop
    while(!quit && !animation.gameover)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // Handle animation events
            quit = Animation_handleEvent(&animation, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            Animation_update(&animation, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }

        // Render animation
        Animation_render(&animation); 
        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
        SDL_SetRenderDrawColor(renderer, 255 , 255, 255, 255);
        SDL_RenderClear(renderer);

    }
    //SDL_Delay(5000);
    Animation_destroy(&animation);

    return true;
}


bool Animation_load(Animation *animation)
{
    animation->player.x = animation->width/2;
    animation->player.y = animation->height/2;
    animation->player.rad= 50;
    animation->player.color.R = 255;
    animation->player.color.G = 0;
    animation->player.color.B = 0;
    animation->player.color.A = 255;
    animation->player.speed = 400;

    for(int i=0; i<3; i++)

        {
            animation->enemies[i].x = rand() % animation->width;
            animation->enemies[i].y = rand() % animation->height;
            animation->enemies[i].rad = 100 + (50 * i);
            animation->enemies[i].color.R = 0;
            animation->enemies[i].color.G = 0;
            animation->enemies[i].color.B = 255;
            animation->enemies[i].color.A = 255;
            animation->enemies[i].speed = 1;
            animation->enemies[i].direction = 0x0;
        }

    for(int i=0; i<20; i++)

        {
            animation->resources[i].x = rand() % animation->width;
            animation->resources[i].y = rand() % animation->height;
            animation->resources[i].rad = 10 ;
            animation->resources[i].color.R = 0;
            animation->resources[i].color.G = 255;
            animation->resources[i].color.B = 255;
            animation->resources[i].color.A = 255;
        }



    // Load the ocean image
    if(!Background_load(animation->renderer, &animation->background, OCEAN_IMG))
        return false;

    // Background move direction
    animation->background.direction = DIRECTION_DOWN;

    // Background speed in pixel/second
    animation->background.speed = 0;

    if(!Plane_load(animation->renderer, &animation->plane, PLANE_IMG, PLANE_SHADOW))
    {
        Background_destroy(&animation->background);
        return false;
    }

    animation->plane.margin = PLANE_MARGIN;
    animation->plane.shadowOffset.x = PLANE_SHADOW_X;
    animation->plane.shadowOffset.y = PLANE_SHADOW_Y;

    // Set plane initial position
    Plane_setX(&animation->plane, (animation->width - animation->plane.image.rect.w) / 2); // Horiz. center
    Plane_setY(&animation->plane, animation->height - animation->plane.image.rect.h - animation->plane.margin);

    // Plane move direction
    animation->plane.direction = DIRECTION_STOP;

    // Plane move speed in pixel/second
    animation->plane.speed = PLANE_SPEED;


    return true;
}

void Animation_destroy(Animation *animation)
{
    Plane_destroy(&animation->plane);
    Background_destroy(&animation->background);
    Death_destroy(&animation->death);
}

bool Animation_handleEvent(Animation *animation, SDL_Event *e)
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
            Dot_setDirection(&animation->player, e->key.keysym.sym);
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
            Dot_unsetDirection(&animation->player, e->key.keysym.sym);
            break;
        }
    }

    return quit;
}

void Eat(Dot* player, Dot* dot)
{
    player->rad += 10;

    dot->x = -(dot->rad + 1);
    dot->y = -(dot->rad + 1);
    dot->speed = 0;

}

void Player_die(Dot* player)
{
    player->speed = 0;
    player->color.R = 0;
    player->color.G = 0;
    player->color.B = 0;

}


void Animation_update(Animation *animation, int framerate)
{
    // Move ocean
    Background_move(&animation->background, framerate);

    // Move player
    Dot_move(&animation->player, animation->width, animation->height, framerate);

    //score counter
    animation->score++/0.00001;

    //int possibledirections[6] = {SDLK_DOWN, SDLK_UP, SDLK_RIGHT, SDLK_LEFT};
    //int previousdirections[3] = {SDLK_DOWN,SDLK_DOWN,SDLK_DOWN};

    for (int i = 0; i < 3; i++)
    {
       Chase_player(&animation->player, &animation->enemies[i]);

       bool theycollide = Detect_collision(&animation->player, &animation->enemies[i]);
       bool nextscreen = true;
       if(theycollide)
       {

           if(animation->player.rad >= animation->enemies[i].rad)

           {
               Eat(&animation->player, &animation->enemies[i]);
           }

           else

           {

               Player_die(&animation->player);
               animation->gameover = true;

           }
       }
    }

    for(int i= 0; i<20; i++)
    {
        bool theycollide = Detect_collision(&animation->player, &animation->resources[i]);

        if(theycollide)
        {
            Eat(&animation->player, &animation->resources[i]);

        }
    }

}

void Animation_render(Animation *animation)
{
    // Render background
    Background_render(animation->renderer, &animation->background, animation->width, animation->height);


    //Draw plane
    void Plane_render(SDL_Renderer *renderer, Plane *plane);

    //Draw circle
    DrawDot(animation->renderer, &animation->player);

    //Draw Enemies
    for (int i = 0; i<3; i++)
    {
        DrawDot(animation->renderer, &animation->enemies[i]);
    }

    //Draw resources
    for (int i = 0; i<20; i++)
    {
        DrawDot(animation->renderer, &animation->resources[i]);
    }

    //score render
    char score[100];
    sprintf(score, "Score: %d", animation->score);
    stringRGBA(animation->renderer,960,20,score, 255, 0, 0, 255);

}

