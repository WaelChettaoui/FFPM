#include "Death.h"
#include "image.h"


bool Death_load(SDL_Renderer *renderer, Death *death, const char *file)
{
    return Image_load(renderer, &death->image, file);
}

void Death_destroy(Death *death)
{
    Image_destroy(&death->image);
}

void Death_render(SDL_Renderer *renderer, Death *death, int width, int height)
{
    // Unused width
    (void) width;

    SDL_Rect crop = death->image.rect;

    if(death->direction == DIRECTION_DOWN)
    {
        if(death->image.rect.y < height)
        {
            crop.y = 0;
            crop.h = death->image.rect.h - death->image.rect.y;

            SDL_Point pos = { 0, death->image.rect.y };
            Image_renderCrop(renderer, &death->image, &crop, &pos);
        }

        if(death->image.rect.y != 0)
        {
            crop.y = death->image.rect.h - death->image.rect.y;
            crop.h = MIN(death->image.rect.y, height);

            SDL_Point pos = { 0, 0 };
            Image_renderCrop(renderer, &death->image, &crop, &pos);
        }
    }
}
