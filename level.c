#include "rogue.h"
#include "level.h"
#include "utils.h"


Level * createLevel(int level)
{
    Level * newLevel;
    newLevel = malloc(sizeof(Level));

    newLevel->level = level;
    newLevel->numberOfRooms = 6;	//�� ������ 6���� ����
    newLevel->rooms = roomsSetUp();
    connectDoors(newLevel);			//��
    newLevel->tiles = saveLevelPositions();

    newLevel->user = playerSetUp();
    placePlayer(newLevel->rooms, newLevel->user);

    addMonsters(newLevel);

    return newLevel;
}

void drawLevel(Level * level)
{
	int x, y, i;

	// Ÿ�ϵ� ���
	for (y = 0; y < MAX_HEIGHT; y++)
	{
		for (x = 0; x < MAX_WIDTH; x++)
		{
			if (x<(level->user->position->x + 4) && x>(level->user->position->x - 4) &&
				y<(level->user->position->y + 4) && y>(level->user->position->y - 4)) {
				mvaddch(y, x, level->tiles[y][x]);
			}
		}
	}

	// ���� ���
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    for (i = 0; i < level->numberOfMonsters; i++)
    {	
		//���ͳ� �������� �þ� ���� �������� ������ ����
		if (level->monsters[i]->position->x<(level->user->position->x + 4) && level->monsters[i]->position->x>(level->user->position->x - 4) &&
			level->monsters[i]->position->y<(level->user->position->y + 4) && level->monsters[i]->position->y>(level->user->position->y - 4))
		{
			int type = level->monsters[i]->pathfinding;
			//pathfinding�� ���ڿ� ���� ���� �ٸ� ������ ����
				attron(COLOR_PAIR(type));
				drawMonster(level->monsters[i]);
				attroff(COLOR_PAIR(type));
		}
		else {
			//�þ� �ۿ� ��� �������̳� ���ʹ� ���������� ������ ���� �Ⱥ��̰� ��
			//�̴� �ܼ�â ���� ������ ���� �ִ� ���� ȯ��� �Ϻ��ϰ� ������ ����
			attron(COLOR_PAIR(1));
			drawMonster(level->monsters[i]);
			attroff(COLOR_PAIR(1));
		}
    }

    drawPlayer(level->user);
}

Room ** roomsSetUp()
{
    int x;
    Room ** rooms;
    rooms = malloc(sizeof(Room)*6);

    for (x = 0; x < 6; x++)
    {
		//���� ���� 6���� ��ǥ�� ����������
        rooms[x] = createRoom(x, 4);
        drawRoom(rooms[x]);
    }

    return rooms;
}

void connectDoors(Level * level)
{
    int i, j;
    int randomRoom, randomDoor;
    int count;

    for (i = 0; i < level->numberOfRooms; i++)
    {
        for (j = 0; j < level->rooms[i]->numberOfDoors; j++)
        {
			//��¦���� ����Ǿ� �ִٸ� ������
            if (level->rooms[i]->doors[j]->connected == 1) 
            {
                continue;
            }

            count = 0;

			//��� ��� ���� �� �� �ֵ��� ���� �ȵ� ��� ������ ���� ����
            while (count < 2)
            {
                randomRoom = rand() % level->numberOfRooms;
                randomDoor = rand() % level->rooms[randomRoom]->numberOfDoors;

				//�̹� ��򰡿� ����� ��¦�̳� ���� ��� ī��Ʈ�� �ø��� ��Ƽ��
                if (level->rooms[randomRoom]->doors[randomDoor]->connected == 1 || randomRoom == i)
                {
                    count++;
                    continue;
                }
				//��Ƽ�� �� �Ǿ��� ��� �� ��¦�� �մ� ���� ����
                pathFind(&level->rooms[randomRoom]->doors[randomDoor]->position, &level->rooms[i]->doors[j]->position);

                level->rooms[randomRoom]->doors[randomDoor]->connected = 1;
                level->rooms[i]->doors[j]->connected = 1;
                break;
            }



        }
    }

}


char ** saveLevelPositions()
	//���� ������ ��� ���� ������ ����
{
    int x, y;
    char ** positions;
    positions = malloc(sizeof(char *) * 25);

    for (y = 0; y < MAX_HEIGHT; y++)
    {
        positions[y] = malloc(sizeof(char) * 100);
        for (x = 0; x < MAX_WIDTH; x++)
        {
            positions[y][x] = mvinch(y, x);
        }
    }

    return positions;
}



void checkPostion(Position * newPosition, Level * level)
	//�̵� ���� ��ġ�� ������ �ִ��� üũ
{
    Player * user;
    user = level->user;
	char newPosChar = mvinch(newPosition->y, newPosition->x);
    switch (newPosChar)
    {
        case '.':		//�̵� ���� Ÿ���� ��� �̵�
        case '#':
        case '+':
            playerMove(newPosition, user, level->tiles);
            break;
        case 'Z':		//��ƼƼ�� ��� �׵�� ��ȣ�ۿ�
        case 'X':		//������, ������Ʈ�鵵 ���� ���
        case 'T':
		case 'z':
		case '!':
		case '[':
		case 'M':
		case 'H':
		case 'E':
            combat(user, getMonsterAt(newPosition, level->monsters), 1);
        default:
            break;
    }

}



void moveMonsters(Level * level)
{
    int x;
    for (x = 0; x < level->numberOfMonsters; x++)
    {
        if (level->monsters[x]->alive == 0)	//���Ͱ� �׾��ٸ� �̵� ����
            continue;

		if (level->monsters[x]->position->x<(level->user->position->x + 4) &&
			level->monsters[x]->position->x>(level->user->position->x - 4) &&
			level->monsters[x]->position->y<(level->user->position->y + 4) &&
			level->monsters[x]->position->y>(level->user->position->y - 4))
			//���Ͱ� �þ� �ȿ� ���Դٸ� �۵���
		{

			if (level->monsters[x]->pathfinding == 1)
			{
				pathfindingRandom(level->monsters[x]->position);
			}
			else if(level->monsters[x]->pathfinding == 2)
			{
				pathfindingSeek(level->monsters[x]->position, level->user->position, level->user);
			}
			else {
				//�������� �ʰ� �״�� ������
			}
		}
    }

}


void addMonsters(Level* level)
{
    int x;
    level->monsters = malloc(sizeof(Monster *)* 10);
	//������ ���� �� 10���� ��� �׸�ŭ�� �Ҵ���
    level->numberOfMonsters = 0;

    for (x = 0; x < level->numberOfRooms; x++)
    {
            level->monsters[level->numberOfMonsters] = selectMonster(level->level);
            setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[x]);
            level->numberOfMonsters++;
			//�� �ϳ����� ���͸� �ϳ��� ��ġ
    }
	int randroom = 0;
	//������ �濡 �������� ��ġ�ϱ� ���� �Ź� rand�Լ��� ���� ���� ����
	//������ ��ǥ�� ����� Ư���� �濡 ����
	level->monsters[level->numberOfMonsters] = createMonster('!', 1, 0, 0, 0, 3);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[5]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//���ݷ��� �ø� �� �ִ� Į ������
	level->monsters[level->numberOfMonsters] = createMonster('[', 1, 0, 0, 0, 4);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[randroom]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//�÷��̾��� ü���� ȸ����Ű�� ���޻���
	level->monsters[level->numberOfMonsters] = createMonster('H', 1, 0, 0, 0, 5);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[randroom]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//������ Ż�ⱸ���� �������� �ڹ���� ��� ����������
	level->monsters[level->numberOfMonsters] = createMonster('E', 1, 0, 0, 50, 6);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[3]);
	level->numberOfMonsters++;

}
