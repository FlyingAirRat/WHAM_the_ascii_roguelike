#include<stdio.h>
#include<my_global.h>
#include<mysql.h>
#include<string.h>
#pragma comment(lib,"libmysql.lib")
#define DB_HOST "airrat.xyz"
#define DB_USER "pi"
#define DB_PASS "painiscupcake"
#define DB_NAME "pi"
#define CHOP(x) x[strlen(x) - 1] = '\0'

int ranking(int playerScore) {


	system("cls");
	MYSQL       *connection = NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char name[50];
	char query[255];

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
		DB_USER, DB_PASS,
		DB_NAME, 3306,
		(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	printf("�����մϴ�! ���ӿ��� �¸��ϼ̽��ϴ�!\n����� ����: %d\n��ŷ�� �ø� �г����� �Է��ϼ��� : ", playerScore);
	fgets(name, 50, stdin);
	CHOP(name);
	sprintf(query, "insert into highscore values "
		"('%s', '%d')",
		name, playerScore);
	//DB�� ���� �̸��� �Բ� ������ ����
	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}


	sprintf(query, "SELECT * FROM highscore order by score desc limit 10");
	query_stat = mysql_query(connection, query); 
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	sql_result = mysql_store_result(connection);
	//DB�� ����� ������ TOP 10�� �߷� �������Ӱ� �Բ� ���
	int i = 0;
	printf("\n\n					       ""TOP 10 ���� ����\n");
	printf("				================================================\n");
		for(i = 1; (sql_row = mysql_fetch_row(sql_result)) != NULL;i++){
		printf("		  			  %-2d�� %+15s    %-15d\n", i, sql_row[0], atoi(sql_row[1]));
	}

	mysql_free_result(sql_result);
	mysql_close(&conn);
	exit(1);
}