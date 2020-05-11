#include "rogue.h"
#include "utils.h"


void combat(Player * player, Monster * monster, int order)
{
	//order�� 1�� ��� �÷��̾��� ���� ����
    if (order == 1)
    {
		//�÷��̾ ���踦 ���� ���·� ���������Ϳ� ���˽�
		//������ ���� ������ �޼��ǰ� �÷��̾��� �¸��� �� 
		if (monster->symbol == 'E' && player->hasKey == 1) {
			playerWin(player->turn, player->health);
		}
		if (monster->defence > player->attack) {
			//������ �ʹ� ���� ������ ��� �������� ����
		}
		else {
			monster->health -= player->attack;
			//���� �÷��̾��� �������� ���Ͱ� ���� �ʾ��� ��
			//������ �ݰ��� �̾���
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
    //order�� 1�� �ƴ� ��� ���� ��������
    else
    {
        player->health -= monster->attack;
        if (player->health > 0)
        {
            monster->health -= player->attack;
        }
    }
}