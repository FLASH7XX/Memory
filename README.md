基于游戏开发的C语言程序设计入门与实践
课程设计报告
游戏题目：Memory（记忆翻数字）
完成日期：2020年1月4日

目录
1.游戏介绍————————————————————————————3
2.实现的主要步骤—————————————————————————3
3.包含的主要技术方法———————————————————————6
4.对应步骤的核心代码———————————————————————7
5.心得体会、总结—————————————————————————8

注意：
a.请将字符集选项调整为使用多字节字符集，否则会编译失败！
b.本游戏程序全程使用VS2019编写，因此依据VS2019的规范使用了_tcscpy_s(), sprintf_s(), fopen_s(), fscanf_s() 函数。
c.附带视频为H265格式，用Windows Media Player可能无法打开，可能需要选择其它播放器如PotPlayer。

1.游戏介绍：
游戏开始，点击任意方格，可翻开数字1秒，随后数字隐藏。翻开两个相同数字即可消除方格。翻开的数字范围是：0~RANGE (默认为25)。游戏包含主程序、规则介绍模块、最佳成绩模块、保存与读取模块、重置模块、背景图片和音乐。

2.实现的主要步骤：
(1)初始化游戏界面，画出小方格。
(2)设置随机数、保存数组。
(3)实现单击方格出现数字。
(4)绘制交互界面，适当更改界面尺寸。
(5)载入背景图片。
(6)添加规则、成绩、存档菜单。
(7)载入音乐。
(8)最终测试与修改。

3.包含的主要技术方法：
(1)使用 time.h ，根据时间生成随机数。
(2)使用 time.h ，记录游戏用时。
(3)使用 EasyX 图形库绘制方格、交互界面、文字，载入并显示图片。
(4)使用 Winmm.lib 载入并播放音频。
(5)调用文件指针，读取或保存数据。

4.对应步骤的核心代码：
(1)使用 time.h ，根据时间生成随机数：
void setNumber(int range)					//按时间生成随机数并存储在数组中
{
	int i;
	int j;
	int numR;

	srand((unsigned int)time(0));		//以时间作为随机数种子生成随机数
	for (i = 0; i < GAMEY / SQUAREY + 2; i++)
  {
		for (j = 0; j < GAMEX / SQUAREX + 2; j++)
		{
			numR = rand() % range;			//控制随机数的范围为 0~range
			numbers[i][j] = numR;			//将随机数存放在数组中
    }
  }
}

(2)使用 time.h ，记录游戏用时。
clock_t start = clock();										//开始计时
clock_t end = (clock() - start) / CLOCKS_PER_SEC;		//停止计时

(3)使用 EasyX 图形库绘制方格、交互界面、文字，载入并显示图片。
fillrectangle(0, 0, GAMEX, SQUAREY);						//绘制消息框
RECT msg = { 0, 0, GAMEX, SQUAREY };
drawtext(_T(str), &msg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);		
//在消息框中显示文字
LOGFONT font;
gettextstyle(&font);
font.lfHeight = 40;									//设置字体大小为 40
_tcscpy_s(font.lfFaceName, _T("Consolas"));	//设置字体样式为 Consolas
font.lfQuality = ANTIALIASED_QUALITY;			//设置字体抗锯齿
settextstyle(&font);

IMAGE img_bk;
loadimage(&img_bk, "bkimage.jpg");			//载入背景图片
putimage(0, SQUAREY, &img_bk);				//将背景图片绘制在主界面

(4)使用 Winmm.lib 载入并播放音频。
mciSendString("open startmusic.mp3 alias startmusic", NULL, 0, NULL);		//载入游戏开始音乐
mciSendString("play startmusic repeat", NULL, 0, NULL);						//循环播放游戏开始音乐
mciSendString("close startmusic", NULL, 0, NULL);	//停止播放游戏开始音乐

(5)调用文件指针，读取或保存数据。
FILE* fp;													//设置文件指针
fopen_s(&fp, "bestScore.txt", "w");					//以写入方式清空并打开文件
if (fp == NULL)
{
	return 0;												//若文件为空则返回 0
}
else
{
	fprintf(fp, "%d\t%d", bestStep, bestTime);	//否则正常写入文件
	fseek(fp, 0, SEEK_SET);							//将指针移至文件开头
	return 1;
}

5.心得体会、总结：
小游戏的结构可谓是麻雀虽小，五脏俱全。游戏逻辑需要严谨地考虑，不同模块之间的关系也得协调好，对外部接口的调用也要熟练、稳定。
在编写小游戏时，本组总结出两个注意点：
(1)载入音频文件时，要注意音频文件的封面图片不能过大（最好去除封面图片），并且要关闭 Nahimic Service服务，否则调用后无法播放。
(2) 调用文件指针时，需要在调用结束时使用 fseek() 函数将指针移回文件开头，否则下次调用时将出现寻址错误。
