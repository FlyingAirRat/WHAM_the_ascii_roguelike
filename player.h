#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"

typedef struct Player
{
    Position * position;
    int health;
    int attack;
    int maxHealth;
	int turn;
	int hasKey;
    // Room * room;
} Player;

/* player functions */
Player * playerSetUp();
Position * handleInput(int input, Player * user);
void playerMove(Position * newPosition, Player * user, char ** level);
void drawPlayer(Player * player);
void playerWin(int playerTurn, int playerHealth);

#endif