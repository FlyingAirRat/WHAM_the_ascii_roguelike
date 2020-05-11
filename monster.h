#ifndef MONSTER_H
#define MONSTER_H

#include "position.h"
#include "player.h"


typedef struct Monster
{
    char string[2];
    char symbol;
    int health;
    int attack;
    int speed;
    int defence;
    int pathfinding;
    int alive;
    Position * position;
} Monster;

/* monster functions */
Monster * selectMonster(int level);
Monster * createMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding);

void pathfindingRandom(Position * position);
void pathfindingSeek(Position * start, Position * destination, Player * player);
Monster * getMonsterAt(Position * position, Monster ** monsters);
void killMonster(Monster * monster, Player * player);
void drawMonster(Monster * monster);



#endif