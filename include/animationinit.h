#ifndef ANIMATIONINIT_H
#define ANIMATIONINIT_H

#include "utils.h"
#include "background.h"

#define PLANE_IMG       "assets/plane.png"
#define PLANE_SPEED     250
#define PLANE_MARGIN    20

#define PLANE_SHADOW    "assets/plane-shadow.png"
#define PLANE_SHADOW_X  50
#define PLANE_SHADOW_Y  10

#define OCEAN_IMG       "assets/youstart.png"
#define OCEAN_SPEED     200

#define YOU_START       "assets/youstart.png"

struct AnimationInit
{
    // Animation screen dimensions
    int width;
    int height;

    // Animation renderer
    SDL_Renderer *renderer;

    // Background and Plane
    Background background;

};
typedef struct AnimationInit AnimationInit;


bool AnimationInit_start(SDL_Renderer *renderer, int width, int height);

bool AnimationInit_load(AnimationInit *animationInit);
void AnimationInit_destroy(AnimationInit *animationInit);

bool AnimationInit_handleEvent(AnimationInit *animationInit, SDL_Event *e);
void AnimationInit_update(AnimationInit *animationInit, int framerate);
void AnimationInit_render(AnimationInit *animationInit);

#endif // ANIMATIONINIT_H
