#include "rogue.h"
#include "utils.h"


//A* 알고리즘을 이용한 복도 생성


//시작점으로부터 frontierCount번째 확장된 좌표들을 담음
void addPositionYX(int ** frontier, int frontierCount, int y, int x)
{
    frontier[frontierCount][0] = y;
    frontier[frontierCount][1] = x;
}


int checkPosition(int y, int x)
{
    char temp = mvinch(y, x);

	//확장될 좌표가 복도가 생성되어선 안될 곳일땐 0을 반환
    if (temp == '.' || temp == '|' || temp == '-')
        return 0;
    else
        return 1;
}

//확장할 방향을 정함
int addNeighbors(int ** frontier, int frontierCount, int *** cameFrom, int y, int x)
{
    // 위로
    if (y > 0 && cameFrom[y - 1][x][0] < 0 && checkPosition(y - 1, x))
    {
        addPositionYX(frontier, frontierCount, y - 1, x);
        frontierCount++;
        cameFrom[y - 1][x][0] = y;
        cameFrom[y - 1][x][1] = x;
    }

    // 아래로
    if (y < (MAX_HEIGHT - 1) && cameFrom[y + 1][x][0] < 0 && checkPosition(y + 1, x))
    {
        addPositionYX(frontier, frontierCount, y + 1, x);
        frontierCount++;
        cameFrom[y + 1][x][0] = y;
        cameFrom[y + 1][x][1] = x;
    }

    // 오른쪽으로
    if (x < (MAX_WIDTH - 1) && cameFrom[y][x + 1][0] < 0 && checkPosition(y, x + 1))
    {
        addPositionYX(frontier, frontierCount, y, x + 1);
        frontierCount++;
        cameFrom[y][x + 1][0] = y;
        cameFrom[y][x + 1][1] = x;
    }

    // 왼쪽으로
    if (x > 0 && cameFrom[y][x - 1][0] < 0 && checkPosition(y, x - 1))
    {
        addPositionYX(frontier, frontierCount, y, x - 1);
        frontierCount++;
        cameFrom[y][x - 1][0] = y;
        cameFrom[y][x - 1][1] = x;
    }
    return frontierCount;

}


//길찾기 함수
void pathFind(Position * start, Position * end)
{
    int i, j;
    int x, y;
    int tempY;
	//맵 전체 크기만큼에 int를 곱한만큼을 할당
    int ** frontier = malloc(sizeof(int*) * MAX_HEIGHT * MAX_WIDTH);
    int *** cameFrom = malloc(sizeof(int**) * MAX_HEIGHT );

	//초기화
    int frontierIndex = 0;
    int frontierCount = 0;

    for (i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
    {
        frontier[i] = malloc(sizeof(int)*2);
    }

    for (i = 0; i < MAX_HEIGHT; i++)
    {
		//확장해온 곳의 좌표를 담을 곳을 할당
        cameFrom[i] = malloc(sizeof(int*)* MAX_WIDTH);
        for (j = 0; j < MAX_WIDTH; j++)
        {
            cameFrom[i][j] = malloc(sizeof(int)*2);
            cameFrom[i][j][0] = -1;
            cameFrom[i][j][1] = -1;
        }
    }

    // 시작 지점과 도착 지점의 가중치를 추가
    cameFrom[start->y][start->x][0] = -9;
    cameFrom[start->y][start->x][1] = -9;

    // 시작 지점을 최초 확장 중심지로 설정
    addPositionYX(frontier, frontierCount, start->y, start->x);
    frontierCount++;

	//시작지점에서 도착 지점까지 확장하게 되었을때 중지
    while (frontierIndex < frontierCount)
    {
        y = frontier[frontierIndex][0];
        x = frontier[frontierIndex][1];

        frontierIndex++;

        if (y == end->y && x == end->x)
        {
            break;
        }
		//확장을 해나가며 매번 그 좌표를 frontier 배열에 저장
        frontierCount = addNeighbors(frontier, frontierCount, cameFrom, y, x);
    }

    y = end->y;
    x = end->x;

	//이제까지 걸어온 좌표들에 복도를 설치함
    while (y != start->y || x != start->x)
    {
        tempY = y;
        y = cameFrom[tempY][x][0];
        x = cameFrom[tempY][x][1];
		mvprintw(y, x, "#");
    }
}