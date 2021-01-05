#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#include<easyx.h>
#include<graphics.h>

#define Version 2.0		//该版本在Mbackup2的基础上实现了数字配对检测、计数器和计时器、重置游戏，加入了图片，消除了重复点击方格的bug


#define GAMEX 640
#define GAMEY 480

#define SQUAREX 80
#define SQUAREY 80

#define RANGE 25

int numbers[GAMEX / SQUAREX + 2][GAMEY / SQUAREY + 2];

int bestStep = 0;
int bestTime = 0;

void initialize(void);
void setNumber(int);
void drawSquare(void);
void printArray(void);
void gameStart(void);
void printAll(void);
void bestScore(void);
void importScore(void);
int load(void);
int save(void);

int main()
{
	initialize();
	while (1)
	{
		MOUSEMSG mouse;				// 定义鼠标消息
		mouse = GetMouseMsg();		// 获取一条鼠标消息
		int x;
		int y;

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			x = mouse.x / SQUAREX;
			y = mouse.y / SQUAREY;

			if (mouse.y > SQUAREY && mouse.y < GAMEY + SQUAREY) gameStart();
			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2);
			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2) bestScore();
			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2) initialize();
			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2) exit(0);
			break;
		}
	}
	_getch();
	return 0;
}

void initialize(void)
{
	initgraph(GAMEX + 1, GAMEY + SQUAREY * 2);
	setNumber(RANGE);
	setfillcolor(BLACK);
	setlinecolor(WHITE);

	fillrectangle(0, SQUAREY, GAMEX, GAMEY + SQUAREY);

	IMAGE img_bk;
	loadimage(&img_bk, "bkimage.jpg");										//载入背景图片
	putimage(0, SQUAREY, &img_bk);											//将背景图片绘制在主界面

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	rectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);
	rectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
	rectangle(0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2);
	rectangle(GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2, GAMEX, GAMEY + SQUAREY * 2);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("游戏规则"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("最佳成绩与存档"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("回到主菜单"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button4 = { GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2, GAMEX, GAMEY + SQUAREY * 2 };
	drawtext(_T("单击退出游戏"), &button4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("单击下方图片开始游戏！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void setNumber(int range)
{
	int i;
	int j;
	int numR;

	srand((unsigned int)time(0));
	for (i = 0; i < GAMEY / SQUAREY + 2; i++)
	{
		for (j = 0; j < GAMEX / SQUAREX + 2; j++)
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

	while (x < GAMEX && y < GAMEY + 80)
	{
		setfillcolor(BLACK);
		setlinecolor(WHITE);

		fillrectangle(x, y, x + SQUAREX, y + SQUAREY);
		x += SQUAREX;

		if (x >= GAMEX)
		{
			x = tempx;
			tempy += SQUAREY;
			y = tempy;
		}
	}
}

void printArray(void)
{
	int i;
	int j;

	for (i = 0; i < GAMEY / SQUAREY + 2; i++)
	{
		for (j = 0; j < GAMEX / SQUAREX + 2; j++)
		{
			printf("%2d ", numbers[i][j]);
		}
		printf("\n");
	}
}

void gameStart(void)
{
	drawSquare();

	clock_t start = clock();

	int notFind = 1;
	int temp = -1;
	int tempx = -1;
	int tempy = -1;
	int step = 0;
	int find = 0;

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 40;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("游戏开始！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	font.lfHeight = 20;
	settextstyle(&font);

	fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);

	char strStep[20];
	sprintf_s(strStep, "已用步数：%d步", step);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T(strStep), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("正在计时···"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	while (notFind)
	{
		MOUSEMSG mouse;		// 定义鼠标消息
		mouse = GetMouseMsg();		// 获取一条鼠标消息

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			int x;
			int y;
			x = mouse.x / SQUAREX;
			y = mouse.y / SQUAREY - 1;

			if (mouse.y > SQUAREY && mouse.y < GAMEY + SQUAREY)
			{
				LOGFONT font;
				gettextstyle(&font);								// 获取当前字体设置
				font.lfHeight = 40;									// 设置字体高度为 48
				_tcscpy_s(font.lfFaceName, _T("Consolas"));			// 设置字体为 Consolas
				font.lfQuality = ANTIALIASED_QUALITY;				// 设置输出效果为抗锯齿  
				settextstyle(&font);								// 设置字体样式

				char str[4];
				sprintf_s(str, "%d", numbers[x][y]);
				RECT rect = { x * SQUAREX, (y + 1) * SQUAREY, x * SQUAREX + SQUAREX, (y + 1) * SQUAREY + SQUAREY };
				drawtext(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				if (numbers[x][y] == temp && (x != tempx || y != tempy))
				{
					notFind = 0;
					find = 1;

					char strLast[4];
					sprintf_s(strLast, "%d", temp);
					RECT rect = { tempx * SQUAREX, (tempy + 1) * SQUAREY, tempx * SQUAREX + SQUAREX, (tempy + 1) * SQUAREY + SQUAREY };
					drawtext(_T(strLast), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					fillrectangle(0, 0, GAMEX, SQUAREY);
					RECT msg = { 0, 0, GAMEX, SQUAREY };
					drawtext(_T("恭喜你，找到了！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					clock_t end = (clock() - start) / CLOCKS_PER_SEC;

					step += 1;
					if (bestStep == 0 || bestStep > step) bestStep = step;

					font.lfHeight = 20;
					settextstyle(&font);

					if (bestTime == 0 || bestTime > end) bestTime = end;

					char strEnd[20];
					sprintf_s(strEnd, "总计用时：%d秒", end);

					fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
					RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
					drawtext(_T(strEnd), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					fillrectangle(0, 0, GAMEX, SQUAREY);

					RECT msg = { 0, 0, GAMEX, SQUAREY };
					drawtext(_T("请等待1s..."), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					step += 1;

					Sleep(1000);
					FlushMouseMsgBuffer();

					fillrectangle(x * SQUAREX, (y + 1) * SQUAREY, x * SQUAREX + SQUAREX, (y + 1) * SQUAREY + SQUAREY);

					fillrectangle(0, 0, GAMEX, SQUAREY);
					drawtext(_T("再选一个方格！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				temp = numbers[x][y];
				tempx = x;
				tempy = y;

				font.lfHeight = 20;
				settextstyle(&font);

				fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);

				char strStep[20];
				sprintf_s(strStep, "总计步数：%d", step);

				RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
				drawtext(_T(strStep), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				initialize();
				notFind = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				exit(0);
			}

			if (find == 1)
			{
				while (find)
				{
					MOUSEMSG mouse;		// 定义鼠标消息
					mouse = GetMouseMsg();		// 获取一条鼠标消息

					switch (mouse.uMsg)
					{
					case WM_LBUTTONUP:
						int x;
						int y;
						x = mouse.x / SQUAREX;
						y = mouse.y / SQUAREY - 1;
						if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
						{
							initialize();
							find = 0;
						}

						if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
						{
							exit(0);
							find = 0;
						}
						break;
					}
				}
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

	for (x = 0; x < GAMEX / SQUAREX; x++)
	{
		for (y = 0; y < GAMEY / SQUAREY; y++)
		{
			char str[4];
			sprintf_s(str, "%d", numbers[x][y]);
			RECT rect = { x * SQUAREX, (y + 1) * SQUAREY, x * SQUAREX + SQUAREX, (y + 1) * SQUAREY + SQUAREY };
			drawtext(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void bestScore(void)
{
	fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2);

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("清除最佳成绩"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("返回主菜单"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("读取或保存最佳成绩"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("最佳成绩与存档"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY);
	RECT msgUp = { 0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY };

	char strUp[20];

	if (bestStep == 0)
	{
		sprintf_s(strUp, "暂无最少步数···");
	}
	else
	{
		sprintf_s(strUp, "最少步数：%d步", bestStep);
	}

	drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY);
	RECT msgDn = { 0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY };

	char strDn[20];

	if (bestTime == 0)
	{
		sprintf_s(strDn, "暂无最短用时···");
	}
	else
	{
		sprintf_s(strDn, "最短用时：%d秒", bestTime);
	}

	drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int bestScore = 1;

	while (bestScore)
	{
		MOUSEMSG mouse;
		mouse = GetMouseMsg();
		int x;
		int y;

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			x = mouse.x / SQUAREX;
			y = mouse.y / SQUAREY;

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)
			{
				bestStep = 0;
				bestTime = 0;

				font.lfHeight = 40;
				settextstyle(&font);

				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };
				drawtext(_T("已清除最佳成绩！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				fillrectangle(0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY);
				RECT msgUp = { 0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY };

				char strUp[20];
				sprintf_s(strUp, "暂无最少步数···");

				drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				fillrectangle(0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY);
				RECT msgDn = { 0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY };

				char strDn[20];
				sprintf_s(strDn, "暂无最短用时···");

				drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)
			{
				initialize();
				bestScore = 0;
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				importScore();
				bestScore = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				exit(0);
			}

		}
	}
}

void importScore(void)
{
	fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2);

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("读取最佳成绩"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("保存最佳成绩"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("返回主菜单"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("读取或保存最佳成绩"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int importScore = 1;

	while (importScore)
	{
		MOUSEMSG mouse;
		mouse = GetMouseMsg();
		int x;
		int y;

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			x = mouse.x / SQUAREX;
			y = mouse.y / SQUAREY;

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)
			{
				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };

				char str[40];

				if (load() == 0)
				{
					sprintf_s(str, "读取成绩失败或无最佳成绩！");
				}
				else
				{
					sprintf_s(str, "读取成绩成功！");
				}

				drawtext(_T(str), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				fillrectangle(0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY);
				RECT msgUp = { 0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY };

				char strUp[20];

				if (bestStep == 0)
				{
					sprintf_s(strUp, "暂无最少步数···");
				}
				else
				{
					sprintf_s(strUp, "最少步数：%d步", bestStep);
				}

				drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				fillrectangle(0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY);
				RECT msgDn = { 0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY };

				char strDn[20];

				if (bestTime == 0)
				{
					sprintf_s(strDn, "暂无最短用时···");
				}
				else
				{
					sprintf_s(strDn, "最短用时：%d秒", bestTime);
				}

				drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)
			{
				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };

				char str[40];

				if (save() == 0)
				{
					sprintf_s(str, "保存成绩失败或无最佳成绩！");
				}
				else
				{
					sprintf_s(str, "保存成绩成功！");
				}

				drawtext(_T(str), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				initialize();
				importScore = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)
			{
				exit(0);
			}
		}
	}
}

int load(void)
{
	FILE* fp;
	fopen_s(&fp, "bestScore.txt", "r+");

	if (fp == NULL)
	{
		//printf("Cannot load yet!");
		return 0;
	}
	else
	{
		fscanf_s(fp, "%d%d", &bestStep, &bestTime);
	}
	fclose(fp);
}

int save(void)
{
	FILE* fp;
	fopen_s(&fp, "bestScore.txt", "w+");

	if (fp == NULL)
	{
		//printf("Cannot save yet!");
		return 0;
	}
	else
	{
		fprintf(fp, "%d\t%d", bestStep, bestTime);
		return 1;
	}

	fclose(fp);
}