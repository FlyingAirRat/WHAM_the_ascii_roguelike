#include "rogue.h"
#include "player.h"

//플레이어 초기화
Player * playerSetUp()
{
    Player * newPlayer;
    newPlayer = malloc(sizeof(Player));
    newPlayer->position = malloc(sizeof(Position));

    newPlayer->health = 30;
    newPlayer->attack = 1;
    newPlayer->maxHealth = 30;
	newPlayer->turn = 0;
	newPlayer->hasKey = 0;
    
    return newPlayer;
}



//사용자 입력을 받아오는 함수
Position * handleInput(int input, Player * user)
{
    Position * newPosition;
    newPosition = malloc(sizeof(Position));
    switch (input)
    {
        //위로 이동
        case 'k':
        case 'K':
            newPosition->y = user->position->y - 1;
            newPosition->x = user->position->x;
            break;

        //아래로 이동
        case 'j':
        case 'J':
            newPosition->y = user->position->y + 1;
            newPosition->x = user->position->x;
            break;

        //왼쪽 이동
        case 'h':
        case 'H':
            newPosition->y = user->position->y;
            newPosition->x = user->position->x - 1;
            break;

        //오른쪽 이동
        case 'l':
        case 'L':
            newPosition->y = user->position->y;
            newPosition->x = user->position->x + 1;
            break;

		//왼쪽 위 이동
		case 'y':
		case 'Y':
			newPosition->y = user->position->y - 1;
			newPosition->x = user->position->x - 1;
			break;

		//오른쪽 위 이동
		case 'u':
		case 'U':
			newPosition->y = user->position->y - 1;
			newPosition->x = user->position->x + 1;
			break;

		//왼쪽 아래 이동
		case 'b':
		case 'B':
			newPosition->y = user->position->y + 1;
			newPosition->x = user->position->x - 1;
			break;

		//오른쪽 아래 이동
		case 'n':
		case 'N':
			newPosition->y = user->position->y + 1;
			newPosition->x = user->position->x + 1;
			break;

			
        default:
			//아무런 이동 하지 않음
            break;

    }
	//점수 계산에 쓰일 턴 수 집계
	user->turn++;
    return newPosition;

}

//실제로 플레이어가 움직임
void playerMove(Position * newPosition, Player * user, char ** level)
{
    user->position->y = newPosition->y;
    user->position->x = newPosition->x;

}

void drawPlayer(Player * player)
{
    mvprintw(player->position->y, player->position->x, "@");
    move(player->position->y, player->position->x);
}

//플레이어가 승리조건을 달성 시
//점수를 계산 한 후, 랭킹보드에 등록할 자격을 가짐
void playerWin(int playerTurn, int playerHealth) {
	endwin();
	system("cls");
	int playerScore = 0;
	if (playerTurn > 500) {
		playerScore = 10000 - (playerTurn * 10);
		if (playerScore < 0) {
			playerScore = 0;
		}
	}
	else {
		playerScore = 5000 + (1000 - playerTurn) * playerHealth;
	}
	ranking(playerScore);
}