#include <stdio.h>
#include <Windows.h>

void init() {
	//스크롤바 안보이게 하기
	HANDLE consolehandleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(consolehandleOutput, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(consolehandleOutput, new_size);
	//WinAPI 콘솔창 빠른 편집 모드 비활성화
	//콘솔창 글씨부분을 클릭하거나
	//드래그해도 선택모드가 활성화되지 않음
	DWORD prev_mode;
	HANDLE consolehandleInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(consolehandleInput, &prev_mode);
	SetConsoleMode(consolehandleInput, ENABLE_EXTENDED_FLAGS |
		(prev_mode & ~ENABLE_QUICK_EDIT_MODE));

}