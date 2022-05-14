#include "Dot.h"
#include "SDL_render.h"
#include "animation.h"

void DrawDot(SDL_Renderer * renderer, Dot* player)
{

   int32_t radius = player->rad;
   int32_t  centreX = player->x;
   int32_t  centreY = player->y;
   const int32_t diameter = (radius * 2);
   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);


   SDL_SetRenderDrawColor(renderer, player->color.R , player->color.G, player->color.B, player->color.A);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
   SDL_RenderFillCircle(renderer, centreX, centreY, radius);
}


SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    //CHECK_RENDERER_MAGIC(renderer, -1);

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;


    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}



void Dot_moveX(Dot *dot, int x)
{
    dot->x += x;
}

void Dot_moveY(Dot *dot, int y)
{
    dot->y += y;
}

void Dot_setDirection(Dot *dot, SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_UP:
        dot->direction &= ~DIRECTION_DOWN;
        dot->direction |= DIRECTION_UP;
        break;

    case SDLK_DOWN:
        dot->direction &= ~SDLK_UP;
        dot->direction |= DIRECTION_DOWN;
        break;

    case SDLK_RIGHT:
        dot->direction &= ~SDLK_LEFT;
        dot->direction |= DIRECTION_RIGHT;
        break;

    case SDLK_LEFT:
        dot->direction &= ~DIRECTION_RIGHT;
        dot->direction |= DIRECTION_LEFT;
        break;
    }
}

void Dot_unsetDirection(Dot *dot, SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_UP:
        dot->direction &= ~DIRECTION_UP;
        break;

    case SDLK_DOWN:
        dot->direction &= ~DIRECTION_DOWN;
        break;

    case SDLK_RIGHT:
        dot->direction &= ~DIRECTION_RIGHT;
        break;

    case SDLK_LEFT:
        dot->direction &= ~DIRECTION_LEFT;
        break;
    }
}

void Dot_move (Dot *dot, int screenWidth, int screenHeight, int framerate)
{
    int dotStep = dot->speed / framerate;
    int diameter = dot->rad*2;

    if( (dot->direction & DIRECTION_UP)
            && dot->y - dotStep >= dot->rad)
    {
        Dot_moveY(dot, -dotStep);
    }
    else if( (dot->direction & DIRECTION_DOWN)
             && dot->y + dotStep <= screenHeight- dot->rad)
    {
        Dot_moveY(dot, dotStep);
    }

    if( (dot->direction & DIRECTION_RIGHT)
            && dot->x + dotStep <= screenWidth - dot->rad)
    {
        Dot_moveX(dot, dotStep);
    }
    else if( (dot->direction & DIRECTION_LEFT)
             && dot->x - dotStep >= dot->rad)
    {
        Dot_moveX(dot, -dotStep);
    }
}

bool Detect_collision(Dot* player, Dot* enemy)
{
    int distance = sqrt(pow(enemy->x - player->x, 2) + pow(enemy->y - player->y, 2));
    int mindistance = enemy->rad + player->rad;

        // Detect  collision
        return(distance <= mindistance);
}

bool Detect_collision_enemy(Dot* player, Dot* enemy, Animation *animation)
{
    int distance = sqrt(pow(enemy->x - player->x, 2) + pow(enemy->y - player->y, 2));
    int mindistance = enemy->rad + player->rad;

        // Detect  collision
        if(distance <= mindistance)
        {
            animation->gameover = true;
        }

        return animation->gameover;
}


void Chase_player(Dot* player, Dot* enemy)
{
    int distance = sqrt(pow(enemy->x - player->x, 2) + pow(enemy->y - player->y, 2));

    int newpositionx = enemy->x + enemy->speed;
    int newpositiony = enemy->y + enemy->speed;

    int newdistancex = sqrt(pow(newpositionx - player->x, 2) + pow(enemy->y - player->y, 2));


    if(newdistancex < distance)
    {
        enemy->x = newpositionx;
        //enemy->y = newpositiony;
    }

    else

    {
        enemy->x -= enemy->speed;
        //enemy->y -= enemy->speed;
    }

    int newdistancey = sqrt(pow(enemy->x - player->x, 2) + pow(newpositiony - player->y, 2));

    if(newdistancey < distance)
    {
        enemy->y = newpositiony;
    }

    else

    {
        enemy->y -= enemy->speed;
    }

    SDL_Log("This is distance %d \n %d", distance, newdistancex);
}
