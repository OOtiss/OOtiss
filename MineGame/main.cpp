#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

#pragma comment(lib,"winmm.lib")

#define ROW 10  //行
#define	COL 10  //列
#define NUM 10  //雷
#define SIZE 25  //图片大小

int count = 0;  //点开个数
int map[ROW+2][COL+2];
IMAGE img[12];  //存放图片

//初始化
void GameInit()
{
	//用随机数布雷
	srand((unsigned int)time(NULL));
	for (int i = 1; i < ROW+2; i++){
		for (int j = 1; j < COL+2; j++){
			map[i][j] = 0;
		}
	}
	int n = 0;
	while (n < NUM)
	{
		int r = rand() % ROW + 1;
		int c = rand() % COL + 1;

		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			n++;
		}
	}
	//填充游戏区不为雷的格子
	for (int i = 1; i <= ROW; i++){
		for (int j = 1; j <= COL; j++){
			if (map[i][j] == 0)
			{
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + 1; n++) {
						if (map[m][n] == -1)
						{
							map[i][j]++;
						}
					}
				}
			}
		}
	}
	//加密
	for (int i = 1; i <= ROW; i++) {
		for (int j = 1; j <= COL; j++) {
			map[i][j] += 20;
		}
	}
}

//绘制游戏区
void GameDraw()
{
	for (int i = 0; i <= ROW + 1; i++){
		for (int j = 0; j <= COL + 1; j++){
			if (map[i][j] == -1) 
			{
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[9]);  //雷
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8) 
			{
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[map[i][j]]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) 
			{
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[10]);
			}
			else if (map[i][j] > 30) 
			{
				putimage((j - 1) * SIZE, (i - 1) * SIZE, &img[11]);
			}
		}
		//printf("\n");
	}
}

//递归实现翻开相邻0
void OpenZero(int r,int c)
{
	map[r][c] -= 20;
	count++;

	for (int m = r - 1; m <= r + 1; m++) {
		for (int n = c - 1; n <= c + 1; n++) {     //九宫格
			if (m >= 1 && m <= ROW && n >= 1 && n <= COL)  //游戏区
			{
				if (map[m][n] >= 19 && map[m][n] <= 28)  //翻开空白照片
				{
					if (map[m][n] != 20)
					{
						map[m][n] -= 20;
						count++;
					}
					else
					{
						OpenZero(m, n);
					}
				}
			}
		}
	}
}

//鼠标运行
int PlayGame()
{
	MOUSEMSG msg = { 0 };
	int r, c;

	while (1)
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			r = msg.y / SIZE + 1;
			c = msg.x / SIZE + 1;

			if (map[r][c] >= 19 && map[r][c] <= 28)  //翻开空白照片
			{
				mciSendString(L"open click.wav alias click", 0, 0, 0);
				mciSendString(L"play click wait", 0, 0, 0);
				mciSendString(L"close click", 0, 0, 0);
				if (map[r][c] == 20)
				{
					OpenZero(r, c);
				}
				else
				{
					map[r][c] -= 20;
					count++;
				}
			}
			return map[r][c];
			break;

		case WM_RBUTTONDOWN:
			r = msg.y / SIZE + 1;
			c = msg.x / SIZE + 1;

			if (map[r][c] >= 19 && map[r][c] <= 28)  //标记
			{
				mciSendString(L"open rightClick.wav alias click", 0, 0, 0);
				mciSendString(L"play click wait", 0, 0, 0);
				mciSendString(L"close click", 0, 0, 0);
				map[r][c] += 50;
			}
			else if (map[r][c] > 30)       //取消标记
			{
				map[r][c] -= 50;
			}	
			return map[r][c];
			break;
		}
	}
}

int main()
{
	HWND hwnd = initgraph(ROW * SIZE, COL * SIZE, SHOWCONSOLE);

	//播放音乐
	mciSendString(L"open start.mp3 alias start", 0, 0, 0);
	mciSendString(L"play start", 0, 0, 0);

	//加载图片
	do {
		loadimage(&img[0], L"./0.jpg", SIZE, SIZE);
		loadimage(&img[1], L"./1.jpg", SIZE, SIZE);
		loadimage(&img[2], L"./2.jpg", SIZE, SIZE);
		loadimage(&img[3], L"./3.jpg", SIZE, SIZE);
		loadimage(&img[4], L"./4.jpg", SIZE, SIZE);
		loadimage(&img[5], L"./5.jpg", SIZE, SIZE);
		loadimage(&img[6], L"./6.jpg", SIZE, SIZE);
		loadimage(&img[7], L"./7.jpg", SIZE, SIZE);
		loadimage(&img[8], L"./8.jpg", SIZE, SIZE);  //8
		loadimage(&img[9], L"./9.jpg", SIZE, SIZE);  //雷
		loadimage(&img[10], L"./10.jpg", SIZE, SIZE);  //空白
		loadimage(&img[11], L"./11.jpg", SIZE, SIZE);  //标记
	} while (0);

	GameInit();
	
	//刷新鼠标信息与界面
	while (1) {
		GameDraw();
		if (PlayGame() == -1)
		{
			GameDraw();
			MessageBox(hwnd, L"You are a Low B!",L"", MB_OK);
			break;
		}
		if ( ROW* COL - NUM == count)
		{
			GameDraw();
			mciSendString(L"open search.wav alias click", 0, 0, 0);
			mciSendString(L"play click wait", 0, 0, 0);
			MessageBox(hwnd, L"You are a superman!", L"", MB_OK);
			mciSendString(L"close click", 0, 0, 0);
			
			break;
		}
	}

	return 0;
}