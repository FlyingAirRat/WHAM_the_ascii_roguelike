#include <stdio.h>
#include "rogue.h"


//메뉴 선택 화면을 출력함
int menuDraw() {
	int x = 55;
	int y = 12;
	int ch = '\0';
	mvprintw(y, x - 2, "> Game start");
	mvprintw(y + 1, x, "Quit");

	//q 입력시 종료
	while (ch != 'q') {
		
		switch (ch) {
			//메뉴선택창 위로 이동
			case 'k':
			case 'K':{
				if (y > 12) {
					mvprintw(y, x - 2, " ");
					mvprintw(--y, x - 2, ">");
				}
				break;
			}
			//메뉴선택창 아래로 이동
			case 'J':
			case 'j':{
				if (y < 13) {
					mvprintw(y, x - 2, " ");
					mvprintw(++y, x - 2, ">");
				}
				break;
			}
			case ' ': {
				return y - 12;
			}
		}
		ch = getch();
	}
}