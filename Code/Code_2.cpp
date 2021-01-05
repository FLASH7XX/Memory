#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<easyx.h>
#include<graphics.h>

#define Version 0.9			//该版本实现了点击方格显示数字，未实现数字配对检测，未消除任何bug

#define gameX 800
#define gameY 600

#define squareX 80
#define squareY 75

int numbers[gameX / squareX + 2][gameY / squareY + 2];

void setNumber(void);
void drawSquare(void);
void printArray(void);
void printNumber(void);
void printAll(void);

int main()
{
	initgraph(gameX, gameY);
	drawSquare();
	setNumber();
	printNumber();
	//printAll();
	_getch();
	return 0;
}

void setNumber(void)
{
	int i;
	int j;
	int numR;

	srand((unsigned int)time(0));
	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			numR = rand() % 25;
			numbers[i][j] = numR;
		}
	}
}

void printArray(void)
{
	int i;
	int j;

	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			printf("%d ", numbers[i][j]);
		}
		printf("\n");
	}
}

void drawSquare(void)
{
	int x = 0;
	int y = 0;
	int tempx = 0;
	int tempy = 0;

	while (x < gameX && y < gameY)
	{
		setfillcolor(BLACK);
		setlinecolor(WHITE);

		fillrectangle(x, y, x + squareX, y + squareY);
		x += squareX;

		if (x >= gameX)
		{
			x = tempx;
			tempy += squareY;
			y = tempy;
		}
	}
}

void printNumber(void)
{
	while (true)
	{
		MOUSEMSG mouse;				// 定义鼠标消息

		mouse = GetMouseMsg();		// 获取鼠标消息
		switch (mouse.uMsg)
		{
		case WM_LBUTTONDOWN:
			int x;
			int y;

			x = mouse.x / squareX;
			y = mouse.y / squareY;

			LOGFONT font;
			gettextstyle(&font);
			font.lfHeight = 48;									// 设置字体高度为 48
			_tcscpy_s(font.lfFaceName, _T("Consolas"));			// 设置字体为 Consolas
			font.lfQuality = ANTIALIASED_QUALITY;				// 设置输出效果为抗锯齿  
			settextstyle(&font);

			TCHAR numRC[5];
			_stprintf_s(numRC, _T("%d"), numbers[x][y]);
			outtextxy(x * squareX + 25, y * squareY + 15, numRC);

			Sleep(1000);
			fillrectangle(x * squareX, y * squareY, x * squareX + squareX, y * squareY + squareY);

			break;
		}
	}
}

void printAll(void)
{
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 48;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	int i;
	int j;

	for (i = 0; i < gameX / squareX; i++)
	{
		for (j = 0; j < gameY / squareY; j++)
		{
			TCHAR numC[5];
			_stprintf_s(numC, _T("%d"), numbers[i][j]);
			outtextxy(i * squareX + 25, j * squareY + 15, numC);
		}
	}
}