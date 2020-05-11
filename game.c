#include "rogue.h"
#include "game.h"
#include "level.h"
#include "utils.h"



void render(Game * game)
{
    clear();
	//게임화면 아래의 스탯창을 표시
    printGameHub(game->levels[game->currentLevel - 1]);
	//현재 지도를 그림
    drawLevel(game->levels[game->currentLevel - 1]);
}


//게임이 실제로 돌아가는 함수
void gameLoop(Game * game)
{
	clear();
	system("cls");
    int ch = '\0';
    Position * newPosition;
    Level * level;

	//첫 레벨일 경우 레벨 초기화
    if (game->currentLevel == 0)
    {
        game->levels[game->currentLevel] = createLevel(1);
        game->currentLevel++;
    }
    level = game->levels[game->currentLevel - 1];


    //메인 게임 반복

    while (ch != 'q')
    {
		//몬스터가 플레이어보다 먼저 순서를 가져감
		moveMonsters(level, level->user);
		//끊임없이 플레이어의 입력을 전달
        newPosition = handleInput(ch, level->user);
		//플레이어가 이동 가능한 지형인지 확인
        checkPostion(newPosition, level);
		//플레이어 이동 후 게임을 렌더링
        render(game);
		//플레이어가 죽을 시 게임 종료
        if (level->user->health <= 0)
        {
			exit(1);
        }
        ch = getch();
    }
	exit(1);
}