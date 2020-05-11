#include "rogue.h"
#include "monster.h"
#include "level.h"



Monster * selectMonster(int level)
{
    int monster = (rand() % 2) + 1;
   //2/1Ȯ���� ���� �� ���� ���� ��Ÿ�� Ȯ���� ����

    switch (monster)
    {
        case 1:
            return createMonster('z', 3, 1, 1, 1, 2);
        case 2:
            return createMonster('Z', 4, 2, 1, 1, 2);
    }



}

Monster * createMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding)
{
    Monster * newMonster;
    newMonster = malloc(sizeof(Monster));


    newMonster->symbol = symbol;
    newMonster->health = health;
    newMonster->attack = attack;
    newMonster->speed = speed;
    newMonster->defence = defence;
    newMonster->pathfinding = pathfinding;
    newMonster->alive = 1;
	//���Ͱ� ȭ�鿡 ǥ�õ� �ɺ��� ���ۿ��� ���
    sprintf(newMonster->string, "%c", symbol);

    newMonster->position = malloc(sizeof(Position));

    return newMonster;
}

void killMonster(Monster * monster, Player * player)
{
	//���͸� ��� ó��
    monster->alive = 0;
	//Į�� ���� ���·� ���踦 ���� ��
	//Ű ���̵�� ���Ⱑ ���׷��̵�
	//�̴� �÷��̾ ���踦 ��� �� ��
	//���������ͷ� ���� ���� ���ݹ޾� �״�
	//�㹫�� ��Ȳ�� �����ϴ� ������ ��
	if (monster->pathfinding == 3) {
		player->hasKey = 1;
		if (player->attack != 1) {
			player->attack = 255;
		}
	}
	//Į�� ����� �� ���踦 ���� �־��ٸ�
	//Ű ���̵�� ���Ⱑ ���׷��̵�
	if (monster->pathfinding == 4) {
		player->attack += 2;
		if (player->hasKey == 1) {
			player->attack = 255;
		}
	}
	//ü�� ȸ������ ����� ���
	//�ִ� ü���� ���� �ʰ� ü��ȸ��
	if (monster->pathfinding == 5) {
		player->health += 15;
		if (player->health > player->maxHealth) {
			player->health = player->maxHealth;
		}
	}
}


//������ ������ �̵��ϴ� ���� �̵� ����
void pathfindingRandom(Position * position)
{
    int random = rand() % 5;

    switch (random)
    {
		//���� �̵�
        case 0:
            if (mvinch(position->y - 1, position->x) == '.'||
				mvinch(position->y - 1, position->x) == '#'||
				mvinch(position->y - 1, position->x) == '+')
            {
                position->y = position->y - 1;
            }
            break;

        //�Ʒ��� �̵�
        case 1:
            if (mvinch(position->y + 1, position->x) == '.'||
				mvinch(position->y + 1, position->x) == '#'||
				mvinch(position->y + 1, position->x) == '+')
            {
                position->y = position->y + 1;
            }
            break;

        //�������� �̵�
        case 2:
            if (mvinch(position->y, position->x - 1) == '.'||
				mvinch(position->y, position->x - 1) == '#'||
				mvinch(position->y, position->x - 1) == '+')
            {
                position->x = position->x - 1;
            }
            break;

        //���������� �̵�
        case 3:
            if (mvinch(position->y, position->x + 1) == '.'||
				mvinch(position->y, position->x + 1) == '#'||
				mvinch(position->y, position->x + 1) == '+')
            {
                position->x = position->x + 1;
            }
            break;

		//�ƹ� �̵����� ����
        case 4:
            break;

    }
}


//�÷��̾ �����ϴ� ���� �̵� ����
void pathfindingSeek(Position * start, Position * destination, Player * player)
{
	drawPlayer(player);	//���Ͱ� �÷��̾� Ÿ�ϰ� ��ġ�� ���� �����ϱ� ����
	int randomizer = rand() % 2;
	//Ȥ�ó� �÷��̾� Ÿ�Ͽ� ���Ͱ� ���� ��� �������� ��
	if (mvinch(start->y, start->x) == '@'){
		player->health--;
	}
	else {
		//���� �� �̵�
		if (
			(abs((start->x - 1) - destination->x) < abs(start->x - destination->x)) &&
			(abs((start->y - 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y - 1, start->x - 1) == '.' ||
				mvinch(start->y - 1, start->x - 1) == '#' ||
				mvinch(start->y - 1, start->x - 1) == '+')
			|| mvinch(start->y - 1, start->x - 1) == '@')
		{
			//�� ���� �÷��̾ �ִٸ� ������
			if (mvinch(start->y - 1, start->x - 1) == '@') {
				player->health--;
			}
			//�÷��̾ �� ���� ���ٸ� �� ������ �̵�
			else {
				start->x = start->x - 1;
				start->y = start->y - 1;
			}
		}
		//������ �� �̵�
		else if (
			(abs((start->x + 1) - destination->x) < abs(start->x - destination->x)) &&
			(abs((start->y - 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y - 1, start->x + 1) == '.' ||
				mvinch(start->y - 1, start->x + 1) == '#' ||
				mvinch(start->y - 1, start->x + 1) == '+')
			|| mvinch(start->y - 1, start->x + 1) == '@')
		{
			if (mvinch(start->y - 1, start->x + 1) == '@') {
				player->health--;
			}
			else {
				start->x = start->x + 1;
				start->y = start->y - 1;
			}

		}
		//���� �Ʒ� �̵�
		else if (
			(abs((start->x - 1) - destination->x) < abs(start->x - destination->x)) &&
			(abs((start->y + 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y + 1, start->x - 1) == '.' ||
				mvinch(start->y + 1, start->x - 1) == '#' ||
				mvinch(start->y + 1, start->x - 1) == '+')
			|| mvinch(start->y + 1, start->x - 1) == '@')
		{
			if (mvinch(start->y + 1, start->x - 1) == '@') {
				player->health--;
			}
			else {
				start->x = start->x - 1;
				start->y = start->y + 1;
			}


		}
		//������ �Ʒ� �̵�
		else if (
			(abs((start->x + 1) - destination->x) < abs(start->x - destination->x)) &&
			(abs((start->y + 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y + 1, start->x + 1) == '.' ||
				mvinch(start->y + 1, start->x + 1) == '#' ||
				mvinch(start->y + 1, start->x + 1) == '+')
			|| mvinch(start->y + 1, start->x + 1) == '@')
		{
			if (mvinch(start->y + 1, start->x + 1) == '@') {
				player->health--;
			}
			else {
				start->x = start->x + 1;
				start->y = start->y + 1;
			}

		}




		//���� �̵�
		//���� �̵��� ��� randomizer�� ������ ����
		//�̴� �������� ������ �߰��� ������ Ȯ���� ����
		else if ((randomizer == 0) &&
			(abs((start->x - 1) - destination->x) < abs(start->x - destination->x)) &&
			(mvinch(start->y, start->x - 1) == '.' ||
			mvinch(start->y, start->x - 1) == '#' ||
			mvinch(start->y, start->x - 1) == '+') ||
			mvinch(start->y, start->x - 1) == '@' &&
			mvinch(start->y, start->x - 1) != 'Z')
		{
			if (mvinch(start->y, start->x - 1) == '@') {
				player->health--;
			}
			else {
				start->x = start->x - 1;
			}

			//������ �̵�
		}
		else if ((randomizer == 0) &&
			(abs((start->x + 1) - destination->x) < abs(start->x - destination->x)) &&
			(mvinch(start->y, start->x + 1) == '.' ||
			mvinch(start->y, start->x + 1) == '#' ||
			mvinch(start->y, start->x + 1) == '+') ||
			mvinch(start->y, start->x + 1) == '@' &&
			mvinch(start->y, start->x + 1) != 'Z')
		{
			if (mvinch(start->y, start->x + 1) == '@') {
				player->health--;
			}
			else {
				start->x = start->x + 1;
			}

			//�Ʒ� �̵�
		}
		else if ((abs((start->y + 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y + 1, start->x) == '.' ||
			mvinch(start->y + 1, start->x) == '#' ||
			mvinch(start->y + 1, start->x) == '+') ||
			mvinch(start->y + 1, start->x) == '@' &&
			mvinch(start->y + 1, start->x) != 'Z')
		{
			if (mvinch(start->y + 1, start->x) == '@') {
				player->health--;
			}
			else {
				start->y = start->y + 1;
			}

			//�� �̵�
		}
		else if ((abs((start->y - 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y - 1, start->x) == '.' ||
			mvinch(start->y - 1, start->x) == '#' ||
			mvinch(start->y - 1, start->x) == '+') ||
			mvinch(start->y - 1, start->x) == '@' &&
			mvinch(start->y - 1, start->x) != 'Z')
		{
			if (mvinch(start->y - 1, start->x) == '@') {
				player->health--;
			}
			else {
				start->y = start->y - 1;
			}
		}
		else
		{
			//������ ����
		}
	}
}

Monster * getMonsterAt(Position * position, Monster ** monsters)
{
    int x;
    for (x = 0; x < 10; x++)
    {
		//�ʻ��� ���͵��� ��ǥ�� �Ѱܹ��� ��ǥ�� ������� �� ���͸� ��ȯ
        if ((position->y == monsters[x]->position->y) && (position->x == monsters[x]->position->x))
            return monsters[x];
    }

    return NULL;
}

void drawMonster(Monster * monster)
{	//���Ͱ� ������� ��쿡�� ������ �ɺ��� ��ǥ�� ���
    if (monster->alive)
    {
        mvprintw(monster->position->y, monster->position->x, monster->string);
    }
}