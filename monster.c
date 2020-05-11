#include "rogue.h"
#include "monster.h"
#include "level.h"



Monster * selectMonster(int level)
{
    int monster = (rand() % 2) + 1;
   //2/1확률로 조금 더 강한 좀비가 나타날 확률이 있음

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
	//몬스터가 화면에 표시될 심볼을 버퍼에서 출력
    sprintf(newMonster->string, "%c", symbol);

    newMonster->position = malloc(sizeof(Position));

    return newMonster;
}

void killMonster(Monster * monster, Player * player)
{
	//몬스터를 사망 처리
    monster->alive = 0;
	//칼을 가진 상태로 열쇠를 얻을 시
	//키 블레이드로 무기가 업그레이드
	//이는 플레이어가 열쇠를 얻고 난 뒤
	//엘리베이터로 가는 중의 공격받아 죽는
	//허무한 상황을 방지하는 역할을 함
	if (monster->pathfinding == 3) {
		player->hasKey = 1;
		if (player->attack != 1) {
			player->attack = 255;
		}
	}
	//칼을 얻었을 시 열쇠를 갖고 있었다면
	//키 블레이드로 무기가 업그레이드
	if (monster->pathfinding == 4) {
		player->attack += 2;
		if (player->hasKey == 1) {
			player->attack = 255;
		}
	}
	//체력 회복제를 얻었을 경우
	//최대 체력을 넘지 않게 체력회복
	if (monster->pathfinding == 5) {
		player->health += 15;
		if (player->health > player->maxHealth) {
			player->health = player->maxHealth;
		}
	}
}


//랜덤한 곳으로 이동하는 몬스터 이동 패턴
void pathfindingRandom(Position * position)
{
    int random = rand() % 5;

    switch (random)
    {
		//위로 이동
        case 0:
            if (mvinch(position->y - 1, position->x) == '.'||
				mvinch(position->y - 1, position->x) == '#'||
				mvinch(position->y - 1, position->x) == '+')
            {
                position->y = position->y - 1;
            }
            break;

        //아래로 이동
        case 1:
            if (mvinch(position->y + 1, position->x) == '.'||
				mvinch(position->y + 1, position->x) == '#'||
				mvinch(position->y + 1, position->x) == '+')
            {
                position->y = position->y + 1;
            }
            break;

        //왼쪽으로 이동
        case 2:
            if (mvinch(position->y, position->x - 1) == '.'||
				mvinch(position->y, position->x - 1) == '#'||
				mvinch(position->y, position->x - 1) == '+')
            {
                position->x = position->x - 1;
            }
            break;

        //오른쪽으로 이동
        case 3:
            if (mvinch(position->y, position->x + 1) == '.'||
				mvinch(position->y, position->x + 1) == '#'||
				mvinch(position->y, position->x + 1) == '+')
            {
                position->x = position->x + 1;
            }
            break;

		//아무 이동하지 않음
        case 4:
            break;

    }
}


//플레이어를 추적하는 몬스터 이동 패턴
void pathfindingSeek(Position * start, Position * destination, Player * player)
{
	drawPlayer(player);	//몬스터가 플레이어 타일과 겹치는 것을 방지하기 위함
	int randomizer = rand() % 2;
	//혹시나 플레이어 타일에 몬스터가 있을 경우 데미지를 줌
	if (mvinch(start->y, start->x) == '@'){
		player->health--;
	}
	else {
		//왼쪽 위 이동
		if (
			(abs((start->x - 1) - destination->x) < abs(start->x - destination->x)) &&
			(abs((start->y - 1) - destination->y) < abs(start->y - destination->y)) &&
			(mvinch(start->y - 1, start->x - 1) == '.' ||
				mvinch(start->y - 1, start->x - 1) == '#' ||
				mvinch(start->y - 1, start->x - 1) == '+')
			|| mvinch(start->y - 1, start->x - 1) == '@')
		{
			//그 곳에 플레이어가 있다면 데미지
			if (mvinch(start->y - 1, start->x - 1) == '@') {
				player->health--;
			}
			//플레이어가 그 곳에 없다면 그 곳으로 이동
			else {
				start->x = start->x - 1;
				start->y = start->y - 1;
			}
		}
		//오른쪽 위 이동
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
		//왼쪽 아래 이동
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
		//오른쪽 아래 이동
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




		//왼쪽 이동
		//직선 이동의 경우 randomizer의 영향을 받음
		//이는 복도에서 몬스터의 추격을 따돌릴 확률을 제공
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

			//오른쪽 이동
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

			//아래 이동
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

			//위 이동
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
			//가만히 있음
		}
	}
}

Monster * getMonsterAt(Position * position, Monster ** monsters)
{
    int x;
    for (x = 0; x < 10; x++)
    {
		//맵상의 몬스터들의 좌표와 넘겨받은 좌표가 같을경우 그 몬스터를 반환
        if ((position->y == monsters[x]->position->y) && (position->x == monsters[x]->position->x))
            return monsters[x];
    }

    return NULL;
}

void drawMonster(Monster * monster)
{	//몬스터가 살아있을 경우에만 몬스터의 심볼을 좌표에 출력
    if (monster->alive)
    {
        mvprintw(monster->position->y, monster->position->x, monster->string);
    }
}