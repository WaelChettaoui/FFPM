
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
#ifndef ANIMATIONQUIT_H
#define ANIMATIONQUIT_H

#include "utils.h"
#include "background.h"

#define PLANE_IMG       "assets/plane.png"
#define PLANE_SPEED     250
#define PLANE_MARGIN    20

#define PLANE_SHADOW    "assets/plane-shadow.png"
#define PLANE_SHADOW_X  50
#define PLANE_SHADOW_Y  10

#define OCEAN_IMG       "assets/youdied.png"
#define OCEAN_SPEED     200

#define YOU_START        "assets/youstart.png"

struct AnimationQuit
{
    // Animation screen dimensions
    int width;
    int height;

    // Animation renderer
    SDL_Renderer *renderer;

    // Background and Plane
    Background background;

};
typedef struct AnimationQuit AnimationQuit;


bool AnimationQuit_start(SDL_Renderer *renderer, int width, int height);

bool AnimationQuit_load(AnimationQuit *animationQuit);
void AnimationQuit_destroy(AnimationQuit *animationQuit);

bool AnimationQuit_handleEvent(AnimationQuit *animationQuit, SDL_Event *e);
void AnimationQuit_update(AnimationQuit *animationQuit, int framerate);
void AnimationQuit_render(AnimationQuit *animationQuit);

#endif // ANIMATIONQUIT_H
