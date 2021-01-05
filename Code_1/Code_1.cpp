#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<easyx.h>
#include<graphics.h>

#define Version 0.1			//原始版本，仅能显示方格和随机数

#define gameX 800
#define gameY 600

#define squareX 80
#define squareY 75

int numbers[gameX / squareX + 2][gameY / squareY + 2];

void setNumber(void);
void drawSquare(void);
void printArray(void);		//用于检查生成的随机数组
void printAll(void);

int main()
{
	initgraph(gameX, gameY);
	drawSquare();
	setNumber();
	printAll();
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