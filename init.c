#include <stdio.h>
#include <Windows.h>

void init() {
	//��ũ�ѹ� �Ⱥ��̰� �ϱ�
	HANDLE consolehandleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(consolehandleOutput, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(consolehandleOutput, new_size);
	//WinAPI �ܼ�â ���� ���� ��� ��Ȱ��ȭ
	//�ܼ�â �۾��κ��� Ŭ���ϰų�
	//�巡���ص� ���ø�尡 Ȱ��ȭ���� ����
	DWORD prev_mode;
	HANDLE consolehandleInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(consolehandleInput, &prev_mode);
	SetConsoleMode(consolehandleInput, ENABLE_EXTENDED_FLAGS |
		(prev_mode & ~ENABLE_QUICK_EDIT_MODE));

}