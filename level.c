#include "rogue.h"
#include "level.h"
#include "utils.h"


Level * createLevel(int level)
{
    Level * newLevel;
    newLevel = malloc(sizeof(Level));

    newLevel->level = level;
    newLevel->numberOfRooms = 6;	//방 갯수를 6개로 설정
    newLevel->rooms = roomsSetUp();
    connectDoors(newLevel);			//문
    newLevel->tiles = saveLevelPositions();

    newLevel->user = playerSetUp();
    placePlayer(newLevel->rooms, newLevel->user);

    addMonsters(newLevel);

    return newLevel;
}

void drawLevel(Level * level)
{
	int x, y, i;

	// 타일들 출력
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

	// 몬스터 출력
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    for (i = 0; i < level->numberOfMonsters; i++)
    {	
		//몬스터나 아이템이 시야 내에 있을때는 색깔을 입힘
		if (level->monsters[i]->position->x<(level->user->position->x + 4) && level->monsters[i]->position->x>(level->user->position->x - 4) &&
			level->monsters[i]->position->y<(level->user->position->y + 4) && level->monsters[i]->position->y>(level->user->position->y - 4))
		{
			int type = level->monsters[i]->pathfinding;
			//pathfinding의 숫자에 따라 각기 다른 색깔을 입힘
				attron(COLOR_PAIR(type));
				drawMonster(level->monsters[i]);
				attroff(COLOR_PAIR(type));
		}
		else {
			//시야 밖에 벗어난 아이템이나 몬스터는 검은색으로 색깔을 입혀 안보이게 함
			//이는 콘솔창 배경색 변경이 막혀 있는 게임 환경상 완벽하게 정보를 숨김
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
		//방의 갯수 6개의 좌표는 정해져있음
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
			//문짝끼리 연결되어 있다면 지나감
            if (level->rooms[i]->doors[j]->connected == 1) 
            {
                continue;
            }

            count = 0;

			//모든 방과 연결 될 수 있도록 연결 안된 방과 무조건 복도 생성
            while (count < 2)
            {
                randomRoom = rand() % level->numberOfRooms;
                randomDoor = rand() % level->rooms[randomRoom]->numberOfDoors;

				//이미 어딘가와 연결된 문짝이나 방일 경우 카운트를 올리고 컨티뉴
                if (level->rooms[randomRoom]->doors[randomDoor]->connected == 1 || randomRoom == i)
                {
                    count++;
                    continue;
                }
				//컨티뉴 안 되었을 경우 두 문짝을 잇는 복도 생성
                pathFind(&level->rooms[randomRoom]->doors[randomDoor]->position, &level->rooms[i]->doors[j]->position);

                level->rooms[randomRoom]->doors[randomDoor]->connected = 1;
                level->rooms[i]->doors[j]->connected = 1;
                break;
            }



        }
    }

}


char ** saveLevelPositions()
	//현재 생성된 모든 맵의 정보를 담음
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
	//이동 예정 위치에 무엇이 있는지 체크
{
    Player * user;
    user = level->user;
	char newPosChar = mvinch(newPosition->y, newPosition->x);
    switch (newPosChar)
    {
        case '.':		//이동 가능 타일일 경우 이동
        case '#':
        case '+':
            playerMove(newPosition, user, level->tiles);
            break;
        case 'Z':		//엔티티일 경우 그들과 상호작용
        case 'X':		//아이템, 오브젝트들도 몬스터 취급
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
        if (level->monsters[x]->alive == 0)	//몬스터가 죽었다면 이동 안함
            continue;

		if (level->monsters[x]->position->x<(level->user->position->x + 4) &&
			level->monsters[x]->position->x>(level->user->position->x - 4) &&
			level->monsters[x]->position->y<(level->user->position->y + 4) &&
			level->monsters[x]->position->y>(level->user->position->y - 4))
			//몬스터가 시야 안에 들어왔다면 작동함
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
				//움직이지 않고 그대로 서있음
			}
		}
    }

}


void addMonsters(Level* level)
{
    int x;
    level->monsters = malloc(sizeof(Monster *)* 10);
	//몬스터의 수는 총 10개로 잡고 그만큼을 할당함
    level->numberOfMonsters = 0;

    for (x = 0; x < level->numberOfRooms; x++)
    {
            level->monsters[level->numberOfMonsters] = selectMonster(level->level);
            setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[x]);
            level->numberOfMonsters++;
			//방 하나마다 몬스터를 하나씩 배치
    }
	int randroom = 0;
	//랜덤한 방에 아이템을 배치하기 위해 매번 rand함수를 돌려 방을 정함
	//게임의 목표인 열쇠는 특정한 방에 생성
	level->monsters[level->numberOfMonsters] = createMonster('!', 1, 0, 0, 0, 3);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[5]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//공격력을 올릴 수 있는 칼 아이템
	level->monsters[level->numberOfMonsters] = createMonster('[', 1, 0, 0, 0, 4);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[randroom]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//플레이어의 체력을 회복시키는 구급상자
	level->monsters[level->numberOfMonsters] = createMonster('H', 1, 0, 0, 0, 5);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[randroom]);
	level->numberOfMonsters++;
	randroom = rand() % level->numberOfRooms;
	//게임의 탈출구이자 목적지인 자물쇠로 잠긴 엘레베이터
	level->monsters[level->numberOfMonsters] = createMonster('E', 1, 0, 0, 50, 6);
	setStartingPosition(level->monsters[level->numberOfMonsters], level->rooms[3]);
	level->numberOfMonsters++;

}
