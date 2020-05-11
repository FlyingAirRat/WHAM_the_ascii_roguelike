#include "rogue.h"
#include "player.h"

//�÷��̾� �ʱ�ȭ
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



//����� �Է��� �޾ƿ��� �Լ�
Position * handleInput(int input, Player * user)
{
    Position * newPosition;
    newPosition = malloc(sizeof(Position));
    switch (input)
    {
        //���� �̵�
        case 'k':
        case 'K':
            newPosition->y = user->position->y - 1;
            newPosition->x = user->position->x;
            break;

        //�Ʒ��� �̵�
        case 'j':
        case 'J':
            newPosition->y = user->position->y + 1;
            newPosition->x = user->position->x;
            break;

        //���� �̵�
        case 'h':
        case 'H':
            newPosition->y = user->position->y;
            newPosition->x = user->position->x - 1;
            break;

        //������ �̵�
        case 'l':
        case 'L':
            newPosition->y = user->position->y;
            newPosition->x = user->position->x + 1;
            break;

		//���� �� �̵�
		case 'y':
		case 'Y':
			newPosition->y = user->position->y - 1;
			newPosition->x = user->position->x - 1;
			break;

		//������ �� �̵�
		case 'u':
		case 'U':
			newPosition->y = user->position->y - 1;
			newPosition->x = user->position->x + 1;
			break;

		//���� �Ʒ� �̵�
		case 'b':
		case 'B':
			newPosition->y = user->position->y + 1;
			newPosition->x = user->position->x - 1;
			break;

		//������ �Ʒ� �̵�
		case 'n':
		case 'N':
			newPosition->y = user->position->y + 1;
			newPosition->x = user->position->x + 1;
			break;

			
        default:
			//�ƹ��� �̵� ���� ����
            break;

    }
	//���� ��꿡 ���� �� �� ����
	user->turn++;
    return newPosition;

}

//������ �÷��̾ ������
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

//�÷��̾ �¸������� �޼� ��
//������ ��� �� ��, ��ŷ���忡 ����� �ڰ��� ����
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