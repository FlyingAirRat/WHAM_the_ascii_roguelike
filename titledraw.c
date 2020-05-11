#include <stdio.h>
#include "rogue.h"
void titleDraw() {
	clear();
	mvprintw(0,0,"\n\n\n\n\n");
	printw("				      W         W H    H    AA    MM      MM  [ ]\n");
	printw("				      W         W H    H   A  A   M M    M M  | |\n");
	printw("				       W   W   W  HHHHHH  A    A  M  M  M  M  | |\n");
	printw("				       W  W W  W  H    H AAAAAAAA M   MM   M  | |\n");
	printw("				        W W W W   H    H A      A M        M    \n");
	printw("				         W   W    H    H A      A M        M   @\n");
}