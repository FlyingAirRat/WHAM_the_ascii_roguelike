#include "rogue.h"
#include "utils.h"

//ȭ�� �ʱ�ȭ
void screenSetUp()
{
    MAX_HEIGHT = 30;
    MAX_WIDTH = 100;

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    srand(time(NULL));
}

//ȭ���ϴ� ����â ���
void printGameHub(Level * level)
{
    mvprintw(28, 0, "     Hp: %d(%d)", level->user->health, level->user->maxHealth);
    printw("    Attack: %d", level->user->attack);
	if (level->user->attack != 1 && level->user->hasKey == 1) {
		printw("	Weapon:  KEY BLADE");
	}
	else if (level->user->attack != 1) {
		printw("	Weapon:  Surgical Blade");
	}
	else {
		printw("	Weapon:  None");
	}
	printw("    turn: %d", level->user->turn);
	if (level->user->hasKey == 0) {
		printw("    Key: None");
	}
	else {
		printw("    Key: Yes");
	}
	
    printw("      ");
}