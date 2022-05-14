#ifndef ANIMATIONCREDITS_H
#define ANIMATIONCREDITS_H
#include "utils.h"
#include "background.h"

#define PLANE_IMG       "assets/plane.png"
#define PLANE_SPEED     250
#define PLANE_MARGIN    20

#define PLANE_SHADOW    "assets/plane-shadow.png"
#define PLANE_SHADOW_X  50
#define PLANE_SHADOW_Y  10

#define OCEAN_IMG       "assets/credits.png"
#define OCEAN_SPEED     200

#define YOU_START       "assets/youstart.png"

struct AnimationCredits
{
    // Animation screen dimensions
    int width;
    int height;

    // Animation renderer
    SDL_Renderer *renderer;

    // Background and Plane
    Background background;

};
typedef struct AnimationCredits AnimationCredits;


bool AnimationCredits_start(SDL_Renderer *renderer, int width, int height);

bool AnimationCredits_load(AnimationCredits *animationCredits);
void AnimationCredits_destroy(AnimationCredits *animationCredits);

bool AnimationCredits_handleEvent(AnimationCredits *animationCredits, SDL_Event *e);
void AnimationCredits_update(AnimationCredits *animationCredits, int framerate);
void AnimationCredits_render(AnimationCredits *animationCredits);
#endif // ANIMATIONCREDITS_H
