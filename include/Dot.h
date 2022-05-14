#ifndef DOT_H
#define DOT_H
#include "SDL_render.h"
#include "Color.h"
#include "utils.h"


struct Dot
{
    int x;
    int y;
    int rad;
    Color color;
    Direction direction;
    int speed;
};
typedef struct Dot Dot;

void DrawDot(SDL_Renderer * renderer, Dot *player);
void Dot_move(Dot *dot, int screenWidth, int screenHeight, int framerate);
void Dot_setDirection(Dot *dot, SDL_Keycode keycode);
void Dot_unsetDirection(Dot *dot, SDL_Keycode keycode);
bool Detect_collision(Dot* player, Dot* enemy);
//bool Detect_collision_enemy(Dot* player, Dot* enemy, Animation *animation);
void Chase_player(Dot* player, Dot* enemy);
#endif // DOT_H

