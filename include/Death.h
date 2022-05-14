#ifndef DEATH_H
#define DEATH_H

#include "utils.h"
#include "image.h"

struct Death
{
    // Image
    Image image;

    // Move direction
    Direction direction;

    // Speed in pixel/second
    int speed;
};
typedef struct Death Death;

bool Death_load(SDL_Renderer *renderer, Death *death, const char *file);
void Death_destroy(Death *death);
void Death_render(SDL_Renderer *renderer, Death *death, int width, int height);
#endif // DEATH_H
