#include <stdio.h>
#include "rogue.h"


//�޴� ���� ȭ���� �����
int menuDraw() {
	int x = 55;
	int y = 12;
	int ch = '\0';
	mvprintw(y, x - 2, "> Game start");
	mvprintw(y + 1, x, "Quit");

	//q �Է½� ����
	while (ch != 'q') {
		
		switch (ch) {
			//�޴�����â ���� �̵�
			case 'k':
			case 'K':{
				if (y > 12) {
					mvprintw(y, x - 2, " ");
					mvprintw(--y, x - 2, ">");
				}
				break;
			}
			//�޴�����â �Ʒ��� �̵�
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