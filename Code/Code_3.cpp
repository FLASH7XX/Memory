#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#include<easyx.h>
#include<graphics.h>

#define Version 1.0			//（大升级）该版本重新确定了界面尺寸，实现了绘制游戏界面、游戏存档、点击方格显示数字，但未实现数字配对检测，未消除任何bug

#define gameX 640
#define gameY 480

#define squareX 80
#define squareY 80

int numbers[gameX / squareX + 2][gameY / squareY + 2];

void initialize(void);
void setNumber(int);
void drawSquare(void);
void printArray(void);
void printNumber(void);
void printAll(void);
void savemenu(void);
int save(void);
int load(void);

int main()
{
	initialize();

	while (1)
	{
		MOUSEMSG mouse;		// 定义鼠标消息
		mouse = GetMouseMsg();		// 获取一条鼠标消息
		int x;
		int y;

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			x = mouse.x / squareX;
			y = mouse.y / squareY;

			if (mouse.x > 0 && mouse.x < gameX / 2 && mouse.y > gameY + squareY && mouse.y < gameY + squareY + squareY / 2) initialize();
			if (mouse.x > gameX / 2 && mouse.x < gameX && mouse.y > gameY + squareY && mouse.y < gameY + squareY + squareY / 2) savemenu();
			if (mouse.x > 0 && mouse.x < gameX / 2 && mouse.y > gameY + squareY + squareY / 2 && mouse.y < gameY + squareY * 2);
			if (mouse.y > squareY && mouse.y < gameY + squareY)
			{
				while (1)
				{
					printNumber();
				}
			}

			if (mouse.x > gameX / 2 && mouse.x < gameX && mouse.y> gameY + squareY + squareY / 2 && mouse.y < gameY + squareY * 2) exit(0);

			break;
		}
	}
	_getch();
	return 0;
}

void initialize(void)
{
	initgraph(gameX, gameY + squareY * 2);
	drawSquare();
	setNumber(25);

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	rectangle(0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2);
	rectangle(gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2);
	rectangle(0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2);
	rectangle(gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2);

	RECT button1 = { 0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2 };
	drawtext(_T("重新开始游戏（未实现）"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2 };
	drawtext(_T("保存/读取游戏"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button3 = { 0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2 };
	drawtext(_T("游戏规则（未实现）"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button4 = { gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2 };
	drawtext(_T("退出游戏"), &button4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, 0, gameX, squareY);
	RECT msg = { 0, 0, gameX, squareY };
	drawtext(_T("单击任意空白格开始游戏！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void setNumber(int range)
{
	int i;
	int j;
	int numR;

	srand((unsigned int)time(0));
	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			numR = rand() % range;
			numbers[i][j] = numR;
		}
	}
}

void drawSquare(void)
{
	int x = 0;
	int y = 80;
	int tempx = 0;
	int tempy = 80;

	while (x < gameX && y < gameY + 80)
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

void printArray(void)
{
	int i;
	int j;

	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			printf("%2d ", numbers[i][j]);
		}
		printf("\n");
	}
}

void printNumber(void)
{
	while (1)
	{
		MOUSEMSG mouse;		// 定义鼠标消息
		mouse = GetMouseMsg();		// 获取一条鼠标消息

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			int x;
			int y;
			x = mouse.x / squareX;
			y = mouse.y / squareY - 1;

			if (mouse.y > squareY && mouse.y < gameY + squareY)
			{
				LOGFONT font;
				gettextstyle(&font);								// 获取当前字体设置
				font.lfHeight = 40;									// 设置字体高度为 48
				_tcscpy_s(font.lfFaceName, _T("Consolas"));			// 设置字体为 Consolas
				font.lfQuality = ANTIALIASED_QUALITY;				// 设置输出效果为抗锯齿  
				settextstyle(&font);								// 设置字体样式

				char str[4];
				sprintf_s(str, "%d", numbers[x][y]);
				RECT rect = { x * squareX, (y + 1) * squareY, x * squareX + squareX, (y + 1) * squareY + squareY };
				drawtext(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				fillrectangle(0, 0, gameX, squareY);
				RECT msg = { 0, 0, gameX, squareY };
				drawtext(_T("请等待1s..."), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				Sleep(1000);

				fillrectangle(x * squareX, (y + 1) * squareY, x * squareX + squareX, (y + 1) * squareY + squareY);
				fillrectangle(0, 0, gameX, squareY);
				drawtext(_T("再选一个方格！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
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

	int x;
	int y;

	for (x = 0; x < gameX / squareX; x++)
	{
		for (y = 0; y < gameY / squareY; y++)
		{
			char str[4];
			sprintf_s(str, "%d", numbers[x][y]);
			RECT rect = { x * squareX, (y + 1) * squareY, x * squareX + squareX, (y + 1) * squareY + squareY };
			drawtext(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void savemenu(void)
{
	fillrectangle(0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2);
	fillrectangle(gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2);
	fillrectangle(0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2);
	fillrectangle(gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2);

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	RECT button1 = { 0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2 };
	drawtext(_T("读取游戏"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2 };
	drawtext(_T("保存游戏"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button3 = { 0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2 };
	drawtext(_T("返回"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button4 = { gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2 };
	drawtext(_T("双击退出游戏"), &button4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int savemenu = 1;
	while (savemenu)
	{
		MOUSEMSG mouse;		// 定义鼠标消息
		mouse = GetMouseMsg();		// 获取一条鼠标消息
		int x;
		int y;

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			x = mouse.x / squareX;
			y = mouse.y / squareY;

			if (mouse.x > 0 && mouse.x < gameX / 2 && mouse.y > gameY + squareY && mouse.y < gameY + squareY + squareY / 2)
			{
				if (load() == 0)
				{
					fillrectangle(0, 0, gameX, squareY);
					RECT msg = { 0, 0, gameX, squareY };
					drawtext(_T("读取游戏失败！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					fillrectangle(0, 0, gameX, squareY);
					RECT msg = { 0, 0, gameX, squareY };
					drawtext(_T("读取游戏成功！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}

			if (mouse.x > gameX / 2 && mouse.x < gameX && mouse.y > gameY + squareY && mouse.y < gameY + squareY + squareY / 2)
			{
				if (save() == 0)
				{
					fillrectangle(0, 0, gameX, squareY);
					RECT msg = { 0, 0, gameX, squareY };
					drawtext(_T("保存游戏失败！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					fillrectangle(0, 0, gameX, squareY);
					RECT msg = { 0, 0, gameX, squareY };
					drawtext(_T("保存游戏成功！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}

			if (mouse.x > 0 && mouse.x < gameX / 2 && mouse.y > gameY + squareY + squareY / 2 && mouse.y < gameY + squareY * 2)
			{
				rectangle(0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2);
				rectangle(gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2);
				rectangle(0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2);
				rectangle(gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2);

				RECT button1 = { 0, gameY + squareY, gameX / 2, gameY + squareY + squareY / 2 };
				drawtext(_T("重新开始游戏"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT button2 = { gameX / 2, gameY + squareY, gameX, gameY + squareY + squareY / 2 };
				drawtext(_T("保存/读取游戏"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT button3 = { 0, gameY + squareY + squareY / 2, gameX / 2, gameY + squareY * 2 };
				drawtext(_T("设置难度"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT button4 = { gameX / 2, gameY + squareY + squareY / 2, gameX, gameY + squareY * 2 };
				drawtext(_T("双击退出游戏"), &button4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				savemenu = 0;
			}
			break;

		case WM_LBUTTONDBLCLK:
			x = mouse.x / squareX;
			y = mouse.y / squareY;

			if (mouse.x > gameX / 2 && mouse.x < gameX && mouse.y> gameY + squareY + squareY / 2 && mouse.y < gameY + squareY * 2)
			{
				exit(0);
				savemenu = 0;
			}
			break;
		}
	}
}

int save(void)
{
	int i;
	int j;

	FILE* fp;
	fopen_s(&fp, "save.txt", "w+");

	if (fp == NULL)
	{
		printf("Cannot save yet!");
		return 0;
	}

	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			fprintf(fp, "%d\t", numbers[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
	return 0;
}

int load(void)
{
	int i;
	int j;

	FILE* fp;
	fopen_s(&fp, "save.txt", "w+");

	if (fp == NULL)
	{
		printf("Cannot load yet!");
		return 0;
	}

	for (i = 0; i < gameY / squareY + 2; i++)
	{
		for (j = 0; j < gameX / squareX + 2; j++)
		{
			fscanf_s(fp, "%d\t", &numbers[i][j]);
		}
	}
}