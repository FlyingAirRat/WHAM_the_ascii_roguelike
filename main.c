#include "rogue.h"
#include "game.h"
#include "utils.h"
#include <Windows.h>

int main()
{
	init();
	initscr();
	noecho();	//버퍼에 입력된 문자열을 화면에 출력하지 않음
	while (1)
	{
		titleDraw();
		int menuCode = menuDraw();
		if (menuCode == 0) {
			system("cls");
			screenSetUp();
			Game game;
			game.currentLevel = 0;
			gameLoop(&game);
		}
		else if (menuCode == 1) {
			return 0;
		}
	}
	
	clear();
    endwin();



    return 0;
}