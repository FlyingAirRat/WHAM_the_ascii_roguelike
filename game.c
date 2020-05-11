#include "rogue.h"
#include "game.h"
#include "level.h"
#include "utils.h"



void render(Game * game)
{
    clear();
	//����ȭ�� �Ʒ��� ����â�� ǥ��
    printGameHub(game->levels[game->currentLevel - 1]);
	//���� ������ �׸�
    drawLevel(game->levels[game->currentLevel - 1]);
}


//������ ������ ���ư��� �Լ�
void gameLoop(Game * game)
{
	clear();
	system("cls");
    int ch = '\0';
    Position * newPosition;
    Level * level;

	//ù ������ ��� ���� �ʱ�ȭ
    if (game->currentLevel == 0)
    {
        game->levels[game->currentLevel] = createLevel(1);
        game->currentLevel++;
    }
    level = game->levels[game->currentLevel - 1];


    //���� ���� �ݺ�

    while (ch != 'q')
    {
		//���Ͱ� �÷��̾�� ���� ������ ������
		moveMonsters(level, level->user);
		//���Ӿ��� �÷��̾��� �Է��� ����
        newPosition = handleInput(ch, level->user);
		//�÷��̾ �̵� ������ �������� Ȯ��
        checkPostion(newPosition, level);
		//�÷��̾� �̵� �� ������ ������
        render(game);
		//�÷��̾ ���� �� ���� ����
        if (level->user->health <= 0)
        {
			exit(1);
        }
        ch = getch();
    }
	exit(1);
}