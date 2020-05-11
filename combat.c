#include "rogue.h"
#include "utils.h"


void combat(Player * player, Monster * monster, int order)
{
	//order가 1일 경우 플레이어의 선제 공격
    if (order == 1)
    {
		//플레이어가 열쇠를 가진 상태로 엘리베이터와 접촉시
		//게임의 최종 목적이 달성되고 플레이어의 승리가 됨 
		if (monster->symbol == 'E' && player->hasKey == 1) {
			playerWin(player->turn, player->health);
		}
		if (monster->defence > player->attack) {
			//방어력이 너무 높기 때문에 어떠한 데미지도 없음
		}
		else {
			monster->health -= player->attack;
			//만약 플레이어의 공격으로 몬스터가 죽지 않았을 때
			//몬스터의 반격이 이어짐
			if (monster->health > 0)
			{
				if (monster->attack != 0) {
					player->health -= monster->attack;
				}
			}
			else
			{
				killMonster(monster, player);
			}
		}
    }
    //order가 1이 아닐 경우 몬스터 선제공격
    else
    {
        player->health -= monster->attack;
        if (player->health > 0)
        {
            monster->health -= player->attack;
        }
    }
}