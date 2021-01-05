		/***********************************************/
		/*												/
		/			   游戏名：Memory					/
		/	组员：谢华桐 唐艺佳 童锴  分组：第37组			/
		/	完成时间：2021年1月4日	版本：Ver.3.0		/
		/												/
		/				游戏规则：						/
		/		游戏开始后，点击任意方格，					/
		/		可翻开数字1秒，随后数字隐藏。				/
		/		翻开两个相同数字即可消除方格。				/
		/		翻开的数字范围是：0~RANGE (默认为25)。		/
		/		现在面向最佳成绩前进吧！					/
		/											   */
		/***********************************************/

#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>					//用于生成随机数及计时
#include<windows.h>

#include<easyx.h>					//用于绘制界面、文字及插入图片
#include<graphics.h>

#pragma comment(lib,"Winmm.lib")	//用于播放音频
#include<mmsystem.h>

#define VERSION 3.0			//最终版本，该版本在Mbackup3的基础上消除了目前已发现的所有bug，加入了游戏规则界面和音乐，更新了存档逻辑。
#define Team 37				//作业分组编号

#define GAMEX 640			//游戏主界面长
#define GAMEY 480			//游戏主界面宽

#define SQUAREX 80			//方格长
#define SQUAREY 80			//方格宽

#define RANGE 25			//随机数范围 (0~RANGE)

int numbers[GAMEX / SQUAREX + 2][GAMEY / SQUAREY + 2];		//二维数组存放生成的随机数（多2行2列防止显示错误）

int bestStep = 0;			//最少步数
int bestTime = 0;			//最短用时

void initialize(void);		//初始化函数，包括绘制主界面、背景图、消息框、菜单按键、播放背景声音
void setNumber(int);		//按时间生成随机数并存储在数组中
void drawSquare(void);		//绘制主界面的所有小方格
void gameStart(void);		//开始游戏函数，绘制所有小方格，记录时间、步数，每一步进行判断
void bestScore(void);		//最佳成绩界面函数，包括最佳成绩的清除、读取与保存
void importScore(void);		//读取或保存游戏函数，目前仅支持一次性读取或保存
int load(void);				//读取文件
int save(void);				//保存文件
void rule(void);			//游戏规则函数，显示游戏简要规则以及版本号、制作组

int main()
{
	initialize();						//初始化，绘制主界面、背景图、消息框、菜单按键、播放背景声音

	while (1)
	{
		MOUSEMSG mouse;					//定义鼠标消息
		mouse = GetMouseMsg();			//获取鼠标消息
		int x;							//记录鼠标x坐标所在的方格
		int y;							//记录鼠标y坐标所在的方格

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:				//鼠标左键抬起时返回参数
			x = mouse.x / SQUAREX;		//将鼠标x坐标转换为所在的方格
			y = mouse.y / SQUAREY;		//将鼠标y坐标转换为所在的方格

			if (mouse.y > SQUAREY && mouse.y < GAMEY + SQUAREY) gameStart();																		//鼠标左键单击背景图片，转到游戏主界面
			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2) rule();					//鼠标左键单击菜单左上角，转到游戏规则界面
			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2) bestScore();		//鼠标左键单击菜单右上角，转到最佳成绩界面
			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2) initialize();		//鼠标左键单击菜单左下角，重新初始化(包括初始化随机数)
			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2) exit(0);			//鼠标左键单击菜单右下角，退出游戏
			break;
		}
	}
	_getch();
	return 0;
}

void initialize(void)								//初始化函数，包括绘制主界面、背景图、消息框、菜单按键、播放背景声音
{
	initgraph(GAMEX, SQUAREY * 2 + GAMEY);			//初始化程序界面
	setNumber(RANGE);								//设置随机数，参见setNumber()函数定义
	setfillcolor(BLACK);							//设置背景色
	setlinecolor(WHITE);							//设置线条色

	fillrectangle(0, SQUAREY, GAMEX, GAMEY + SQUAREY);						//初始化主界面

	IMAGE img_bk;
	loadimage(&img_bk, "bkimage.jpg");										//载入背景图片
	putimage(0, SQUAREY, &img_bk);											//将背景图片绘制在主界面

	mciSendString("open bkfighter.mp3 alias bkmusic", NULL, 0, NULL);		//载入背景音乐
	mciSendString("close startmusic", NULL, 0, NULL);						//停止播放游戏开始音乐
	mciSendString("close winmusic", NULL, 0, NULL);							//停止播放游戏胜利音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);					//循环播放背景音乐

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 20;														//设置字体大小为 20
	_tcscpy_s(font.lfFaceName, _T("Consolas"));								//设置字体样式为 Consolas
	font.lfQuality = ANTIALIASED_QUALITY;									//设置字体抗锯齿
	settextstyle(&font);

	rectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);						//绘制四个菜单方格
	rectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
	rectangle(0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2);
	rectangle(GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2, GAMEX, GAMEY + SQUAREY * 2);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("游戏规则"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);						//在左上角菜单方格中显示"游戏规则"

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("最佳成绩与存档"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);					//在右上角菜单方格中显示"最佳成绩与存档"

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("回到主菜单"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);					//在左下角菜单方格中显示"回到主菜单"

	RECT button4 = { GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2, GAMEX, GAMEY + SQUAREY * 2 };
	drawtext(_T("单击退出游戏"), &button4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);					//在右下角菜单方格中显示"单击退出游戏"

	font.lfHeight = 40;																				//设置字体大小为 40
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);															//绘制消息框
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("单击下方图片开始游戏！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//在消息框中显示文字
}

void setNumber(int range)					//按时间生成随机数并存储在数组中
{
	int i;
	int j;
	int numR;

	srand((unsigned int)time(0));			//以时间作为随机数种子生成随机数
	for (i = 0; i < GAMEY / SQUAREY + 2; i++)
	{
		for (j = 0; j < GAMEX / SQUAREX + 2; j++)
		{
			numR = rand() % range;			//控制随机数的范围为 0~RANGE
			numbers[i][j] = numR;			//将随机数存放在数组中
		}
	}
}

void drawSquare(void)			//绘制主界面的所有小方格
{
	int x = 0;
	int y = SQUAREY;
	int tempx = 0;				//存储当前绘制的行数
	int tempy = SQUAREY;		//存储当前绘制的列数

	while (x < GAMEX && y < GAMEY + 80)
	{
		setfillcolor(BLACK);
		setlinecolor(WHITE);

		fillrectangle(x, y, x + SQUAREX, y + SQUAREY);			//绘制一个方格
		x += SQUAREX;											//绘制右边一个方格

		if (x >= GAMEX)											//当一行方格绘制完毕，转到下一行绘制方格
		{
			x = tempx;
			tempy += SQUAREY;
			y = tempy;
		}
	}
}

void gameStart(void)				//开始游戏函数，绘制所有小方格，记录时间、步数，每一步进行判断
{
	drawSquare();

	mciSendString("open startmusic.mp3 alias startmusic", NULL, 0, NULL);		//载入游戏开始音乐
	mciSendString("close bkmusic", NULL, 0, NULL);								//停止播放背景音乐
	mciSendString("close winmusic", NULL, 0, NULL);								//停止播放游戏胜利音乐
	mciSendString("play startmusic repeat", NULL, 0, NULL);						//循环播放游戏开始音乐

	clock_t start = clock();		//开始计时

	int notFind = 1;				//此区域初始化作用均参见下方循环体
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
	sprintf_s(strStep, "已用步数：%d步", step);											//将菜单左上角更改为已用步数

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T(strStep), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("正在计时···"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//将菜单右上角更改为正在计时，总计用时在游戏结束后显示

	while (notFind)																		//未找到相同的数则进入循环
	{
		MOUSEMSG mouse;
		mouse = GetMouseMsg();

		switch (mouse.uMsg)
		{
		case WM_LBUTTONUP:
			int x;
			int y;
			x = mouse.x / SQUAREX;
			y = mouse.y / SQUAREY - 1;

			if (mouse.y > SQUAREY && mouse.y < GAMEY + SQUAREY)							//鼠标左键单击主界面的某个方格，记录并绘制方格所代表的数字
			{
				LOGFONT font;
				gettextstyle(&font);
				font.lfHeight = 40;
				_tcscpy_s(font.lfFaceName, _T("Consolas"));
				font.lfQuality = ANTIALIASED_QUALITY;
				settextstyle(&font);

				char str[4];
				sprintf_s(str, "%d", numbers[x][y]);
				RECT rect = { x * SQUAREX, (y + 1) * SQUAREY, x * SQUAREX + SQUAREX, (y + 1) * SQUAREY + SQUAREY };
				drawtext(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//绘制数字

				if (numbers[x][y] == temp && (x != tempx || y != tempy))					//若显示的数字和上个数字相等，且不是重复单击同一个方格，则结算步数、时间，本轮游戏结束
				{
					notFind = 0;				//向下执行结束后退出循环体
					find = 1;					//设置find变量防止误点，参见下文

					mciSendString("open winmusic.mp3 alias winmusic", NULL, 0, NULL);		//载入游戏胜利音乐
					mciSendString("close bkmusic", NULL, 0, NULL);							//停止播放背景音乐
					mciSendString("close startmusic", NULL, 0, NULL);						//停止播放游戏开始音乐
					mciSendString("play winmusic repeat", NULL, 0, NULL);					//循环播放游戏胜利音乐

					char strLast[4];
					sprintf_s(strLast, "%d", temp);
					RECT rect = { tempx * SQUAREX, (tempy + 1) * SQUAREY, tempx * SQUAREX + SQUAREX, (tempy + 1) * SQUAREY + SQUAREY };
					drawtext(_T(strLast), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//在上次单击的方格中绘制上次显示的数字

					fillrectangle(0, 0, GAMEX, SQUAREY);
					RECT msg = { 0, 0, GAMEX, SQUAREY };
					drawtext(_T("恭喜你，找到了！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//消息框更新

					clock_t end = (clock() - start) / CLOCKS_PER_SEC;									//停止计时

					step += 1;
					if (bestStep == 0 || bestStep > step) bestStep = step;								//结算步数，若小于最少步数则更新最少步数

					font.lfHeight = 20;
					settextstyle(&font);

					if (bestTime == 0 || bestTime > end) bestTime = end;								//若本次用时小于最短用时则更新最短用时

					char strEnd[20];
					sprintf_s(strEnd, "总计用时：%d秒", end);

					fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
					RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
					drawtext(_T(strEnd), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//显示用时
				}
				else//若显示的数字和上个数字不相等，则等待后继续选择方格
				{
					fillrectangle(0, 0, GAMEX, SQUAREY);

					RECT msg = { 0, 0, GAMEX, SQUAREY };
					drawtext(_T("请等待1s..."), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//更新消息框

					step += 1;

					Sleep(1000);
					FlushMouseMsgBuffer();		//等待1s，等待结束时清除鼠标缓冲区，防止多次点击

					fillrectangle(x * SQUAREX, (y + 1) * SQUAREY, x * SQUAREX + SQUAREX, (y + 1) * SQUAREY + SQUAREY);

					fillrectangle(0, 0, GAMEX, SQUAREY);
					drawtext(_T("再选一个方格！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//更新消息框
				}
				temp = numbers[x][y];		//更新最后一次显示的数字
				tempx = x;					//更新最后一次显示的数字所在的方格
				tempy = y;

				font.lfHeight = 20;
				settextstyle(&font);

				fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);

				char strStep[20];
				sprintf_s(strStep, "总计步数：%d", step);

				RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
				drawtext(_T(strStep), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新步数
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)			//鼠标左键单击菜单左下角，重新初始化并返回主界面
			{
				initialize();
				notFind = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)		//鼠标左键单击菜单右下角，退出游戏
			{
				exit(0);
			}

			if (find == 1)		//防止找到数字后点击无功能的区域触发函数
			{
				while (find)
				{
					MOUSEMSG mouse;
					mouse = GetMouseMsg();

					switch (mouse.uMsg)
					{
					case WM_LBUTTONUP:
						int x;
						int y;
						x = mouse.x / SQUAREX;
						y = mouse.y / SQUAREY - 1;
						if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)		//只限定鼠标左键单击左下角与右下角的菜单，其它情况被忽略
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

void bestScore(void)		//最佳成绩界面函数，包括最佳成绩的清除、读取与保存
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
	drawtext(_T("清除最佳成绩"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//在左上角菜单方格中显示"清除最佳成绩"

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("返回主菜单"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//在右上角菜单方格中显示"返回主菜单"

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("读取或保存最佳成绩"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//在左下角菜单方格中显示"读取或保存最佳成绩"

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("最佳成绩与存档"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);					//更新消息框

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

	drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//在主界面上半部分中显示最少步数

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

	drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//在主界面下半部分中显示最短用时

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

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)		//鼠标左键单击菜单左上角，清除最佳成绩
			{
				bestStep = 0;
				bestTime = 0;

				font.lfHeight = 40;
				settextstyle(&font);

				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };
				drawtext(_T("已清除最佳成绩！"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新消息框

				fillrectangle(0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY);
				RECT msgUp = { 0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY };

				char strUp[20];
				sprintf_s(strUp, "暂无最少步数···");

				drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//更新游戏主界面上半部分

				fillrectangle(0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY);
				RECT msgDn = { 0, GAMEY / 2 + SQUAREY, GAMEX, GAMEY + SQUAREY };

				char strDn[20];
				sprintf_s(strDn, "暂无最短用时···");

				drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//更新游戏主界面下半部分
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)			//鼠标左键单击菜单右上角，返回主菜单
			{
				initialize();
				bestScore = 0;
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)			//鼠标左键单击菜单左下角，转到读取或保存界面
			{
				importScore();
				bestScore = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)		//鼠标左键单击菜单右下角，退出游戏
			{
				exit(0);
			}

		}
	}
}

void importScore(void)		//读取或保存游戏函数，目前仅支持一次性读取或保存
{
	fillrectangle(0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2);
	fillrectangle(0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2);

	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 40;
	_tcscpy_s(font.lfFaceName, _T("Consolas"));
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("读取或保存最佳成绩"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//更新消息框

	font.lfHeight = 20;
	settextstyle(&font);

	RECT button1 = { 0, GAMEY + SQUAREY, GAMEX / 2, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("读取最佳成绩"), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//在左上角菜单方格中显示"读取最佳成绩"

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	drawtext(_T("保存最佳成绩"), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//在右上角菜单方格中显示"保存最佳成绩"

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("返回主菜单"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			//在左下角菜单方格中显示"返回主菜单"

	int importScore = 1;		//此区域初始化的作用参见下文循环体
	int cannotLoad = 0;
	int cannotSave = 0;

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

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)		//鼠标左键单击菜单左上角，保存最佳成绩
			{

				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };

				font.lfHeight = 40;
				settextstyle(&font);

				char str[40];

				if (cannotLoad == 1)
				{
					sprintf_s(str, "暂无新数据，请退出主菜单！");							//若此次运行已读取或保存过，则退出主菜单前无法再次读取，原因参见上文注释
				}
				else if (load() == 0)
				{
					sprintf_s(str, "读取成绩失败或无最佳成绩！");							//若文件为空则读取成绩失败或无最佳成绩
				}
				else
				{
					sprintf_s(str, "读取成绩成功！");										//读取成绩成功
					cannotLoad = 1;
					cannotSave = 1;
				}

				drawtext(_T(str), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新消息框

				fillrectangle(0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY);
				RECT msgUp = { 0, SQUAREY, GAMEX, GAMEY / 2 + SQUAREY };

				char strUp[40];

				if (bestStep == 0)
				{
					sprintf_s(strUp, "暂无最少步数···");
				}
				else
				{
					sprintf_s(strUp, "最少步数：%d步", bestStep);
				}

				drawtext(_T(strUp), &msgUp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新主界面上半部分

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

				drawtext(_T(strDn), &msgDn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新主界面下半部分
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y > GAMEY + SQUAREY && mouse.y < GAMEY + SQUAREY + SQUAREY / 2)		//鼠标左键单击菜单右上角，读取最佳成绩
			{

				fillrectangle(0, 0, GAMEX, SQUAREY);
				RECT msg = { 0, 0, GAMEX, SQUAREY };

				font.lfHeight = 40;
				settextstyle(&font);

				char str[40];
				if (cannotSave == 1)
				{
					sprintf_s(str, "暂无新数据，请退出主菜单！");							//若此次运行已读取或保存过，则退出主菜单前无法再次保存，原因参见上文注释
				}
				else if (save() == 0)
				{
					sprintf_s(str, "保存成绩失败或无最佳成绩！");							//若文件为空则保存成绩失败或无最佳成绩
				}
				else
				{
					sprintf_s(str, "保存成绩成功！");										//保存成绩成功
					cannotLoad = 1;
					cannotSave = 1;
				}

				drawtext(_T(str), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		//更新消息框
			}

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)			//鼠标左键单击菜单左下角，返回主菜单
			{
				initialize();
				importScore = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)		//鼠标左键单击菜单右下角，退出游戏
			{
				exit(0);
			}
		}
	}
}

int load(void)								//读取文件
{
	FILE* fp;								//设置文件指针
	fopen_s(&fp, "bestScore.txt", "r");		//以只读方式打开 bestScore.txt

	if (fp == NULL)							//若文件为空则返回 0
	{
		return 0;
	}
	else
	{
		int tempStep, tempTime;
		fscanf_s(fp, "%d%d", &tempStep, &tempTime);								//读取文件中的步数和用时
		fseek(fp, 0, SEEK_SET);													//将指针移至文件开头
		if (tempStep > 0 && tempTime > 0)
		{
			if (tempStep < bestStep || bestStep == 0) bestStep = tempStep;		//比较文件中的步数和当前最少步数，若文件中的步数更少则更新最少步数
			if (tempTime < bestTime || bestTime == 0) bestTime = tempTime;		//比较文件中的用时和当前最短用时，若文件中的用时更短则更新最短用时
			return 1;
		}
		else
		{
			return 0;
		}
	}
	//fclose(fp);																//关闭文件
}

int save(void)																//保存文件
{
	if (bestStep != 0 && bestTime != 0)										//只有存在最少步数和最短用时才能成功保存
	{
		FILE* fp;															//设置文件指针
		fopen_s(&fp, "bestScore.txt", "w");									//以写入方式清空并打开 bestScore.txt

		if (fp == NULL)
		{
			return 0;										//若文件为空则返回 0
		}
		else
		{
			fprintf(fp, "%d\t%d", bestStep, bestTime);		//否则正常写入文件
			fseek(fp, 0, SEEK_SET);							//将指针移至文件开头
			return 1;
		}
		//fclose(fp);											//关闭文件
	}
	else
	{
		return 0;											//不存在最少步数和最短用时则返回 0
	}
}

void rule(void)		//游戏规则函数，显示游戏简要规则以及版本号、制作组
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
	char strVer[30];
	sprintf_s(strVer, "游戏版本号：%.1f", VERSION);
	drawtext(_T(strVer), &button1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);						//在左上角菜单方格中显示游戏版本号

	RECT button2 = { GAMEX / 2, GAMEY + SQUAREY, GAMEX, GAMEY + SQUAREY + SQUAREY / 2 };
	char strTeam[30];
	sprintf_s(strTeam, "制作小组：Team No.%d", Team);
	drawtext(_T(strTeam), &button2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);					//在右上角菜单方格中显示制作小组号

	RECT button3 = { 0, GAMEY + SQUAREY + SQUAREY / 2, GAMEX / 2, GAMEY + SQUAREY * 2 };
	drawtext(_T("返回主菜单"), &button3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);				//在左下角菜单方格中显示"返回主菜单"

	font.lfHeight = 40;
	settextstyle(&font);

	fillrectangle(0, SQUAREY, GAMEX, GAMEY + SQUAREY);
	RECT msgRule = { 0, SQUAREY * 3, GAMEX, GAMEY + SQUAREY };

	char strRule[256];
	sprintf_s(strRule, "游戏开始，点击任意方格，\n可翻开数字1秒，随后数字隐藏。\n翻开两个相同数字即可消除方格。\n翻开的数字范围是：0~%d。\n现在面向最佳成绩前进吧！\n", RANGE);
	drawtext(_T(strRule), &msgRule, DT_CENTER);													//在主界面中显示规则

	fillrectangle(0, 0, GAMEX, SQUAREY);
	RECT msg = { 0, 0, GAMEX, SQUAREY };
	drawtext(_T("游戏规则"), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);						//更新消息框

	int rule = 1;

	while (rule)
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

			if (mouse.x > 0 && mouse.x < GAMEX / 2 && mouse.y > GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)		//鼠标左键单击菜单左下角，返回主菜单
			{
				initialize();
				rule = 0;
			}

			if (mouse.x > GAMEX / 2 && mouse.x < GAMEX && mouse.y> GAMEY + SQUAREY + SQUAREY / 2 && mouse.y < GAMEY + SQUAREY * 2)	//鼠标左键单击菜单右下角，退出游戏
			{
				exit(0);
			}

		}
	}
}