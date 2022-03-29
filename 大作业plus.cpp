#include <Windows.h>
#include <WinNT.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <time.h>
#include <stdio.h>
#include "head.h"
#include "resource.h"
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Threadproc(LPVOID ipParam);
LPCWSTR lps_cl = L"Mygame";//类名
LPCWSTR wd_text = L"一个充满**的游戏";//窗口标题
HINSTANCE hg_app;//全局实例句柄
struct struct3 {
	int x, y, point;//x子弹横坐标，y子弹纵坐标,point方向
	bool exist;     //子弹存在与否的变量,1为存在，0不存在
}aibullet[100];//AItank的弹药包
struct tankdata
{
	int x, y, hp, point, value;//上0，右1，下2，左3
	int butnum;//玩家子弹数
}player;//坦克信息
struct struct2 {
	int x, y, hp, point;//上0，右1，下2，左3
	int kind;//ai坦克类型
}AItank[20];
int ainum;
struct struct1 {
	int x, y, point;//x子弹横坐标，y子弹纵坐标,point方向
	bool exist;     //子弹存在与否的变量,1为存在，0不存在
}bullet[20];
int tank_width = 43, tank_high = 42,bul_num,mapnum,ai_bulnum;
int ai_width = 43, ai_high = 42;
int bul_width = 44, bul_high = 50;
int boomhigh = 45, boomwidth = 45;
int key = 0,flag=1;
int map[600][600];//地图数组
int WINAPI wWinMain(HINSTANCE hThisApp,HINSTANCE hPrevApp,LPWSTR lpCmd,int nShow)
{
	WNDCLASSEX wc = {sizeof(WNDCLASS) };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance = hThisApp;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.lpszClassName = lps_cl;
	wc.hIcon = LoadIcon(hThisApp,lps_cl);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindowEx(WS_EX_WINDOWEDGE,
		lps_cl,
		wd_text,
		WS_OVERLAPPEDWINDOW,
		0,//窗口初始x坐标
		0,//窗口初始y坐标
		600,//窗口高
		600,//宽
		NULL,//父窗口
		NULL,
		hThisApp,
		NULL);
	if (hwnd == NULL)
		return -1;
	ShowWindow(hwnd, nShow);
	UpdateWindow(hwnd);
	hg_app = hThisApp;
	SetTimer(hwnd, Timer1, 300, NULL);//设置定时器，控制子弹
	SetTimer(hwnd, Timer2, 5000, NULL);//控制AI坦克的生成速度
	SetTimer(hwnd, Timer3, 500, NULL);//控制AI坦克移动间隔
	SetTimer(hwnd, Timer4, 3000,NULL);//控制AItank发射子弹
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))

	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)

{
	HDC hdc1;
	static HDC hmemdc;
	PAINTSTRUCT ps;
	int i, j,k,w;
	static HICON hicon;
	static int cxicon, cyicon, cxclient, cyclient;
	static HBITMAP hbitmap;
	TCHAR a[40],b[20];
	char c[20];
	int x = 6;
	TCHAR char1[20];
	HWND childhwnd;
	HANDLE hThread;
	DWORD  threadID;
	switch (msg)
	{
	case WM_SIZE:
	{
		UpdateWindow(hwnd);//向消息队列发送一个wm_point消息
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		hdc1 = BeginPaint(hwnd, &ps);
		hmemdc = CreateCompatibleDC(hdc1);
		hbitmap = (HBITMAP)LoadImage(hg_app, L"timg.bmp", IMAGE_BITMAP, 600, 600, LR_LOADFROMFILE);
		SelectObject(hmemdc, hbitmap);
		StretchBlt(hdc1, 0, 0, 600, 600, hmemdc, 0, 0, 600, 600, SRCCOPY);
		DeleteDC(hmemdc);
		DeleteObject(hbitmap);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CREATE://主窗口登录
	{
		RECT rect;
		hdc1 = GetDC(hwnd);
		//创建三个按钮
		CreateWindow(L"Button", L"登录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,300, 150, 160, 60, hwnd, (HMENU)IDB_ONE, hg_app, NULL);
		CreateWindow(L"EDIT", L"请输入用户名", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 150, 160, 60, hwnd, (HMENU)IDB_four, hg_app, NULL);
		CreateWindow(L"Button", L"注册", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 300, 250, 160, 60, hwnd, (HMENU)IDB_six, hg_app, NULL);
		CreateWindow(L"EDIT", L"请输入几个字符作为用户名", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 250, 160, 60, hwnd, (HMENU)IDB_seve, hg_app, NULL);
		CreateWindow(L"Button", L"查看排行榜(第一)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 350, 160, 60, hwnd, (HMENU)IDB_THREE, hg_app, NULL);
		CreateWindow(L"Button", L"查看游戏说明", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 450, 160, 60, hwnd, (HMENU)IDB_eight, hg_app, NULL);
		ReleaseDC(hwnd, hdc1);
		break;
	}
	case WM_COMMAND://主窗口响应
	{
		switch (LOWORD(wParam))
		{
		case IDB_ONE:
			//MessageBox(hwnd, L"您点击了第一个按钮。", L"提示", MB_OK | MB_ICONINFORMATION);
			//MessageBox(hwnd, L"您点击了第二个按钮。", L"提示", MB_OK | MB_ICONINFORMATION);
			if (denglu1(hwnd) == 1)
			{
				key = 1;
				PlaySound(L"Battle City.wav", NULL, SND_ASYNC);
			    childhwnd = GetDlgItem(hwnd, IDB_ONE);
				ShowWindow(childhwnd, SW_HIDE);
				childhwnd = GetDlgItem(hwnd, IDB_four);
				ShowWindow(childhwnd, SW_HIDE);
				childhwnd = GetDlgItem(hwnd, IDB_six);
				ShowWindow(childhwnd, SW_HIDE);
				childhwnd = GetDlgItem(hwnd, IDB_seve);
				ShowWindow(childhwnd, SW_HIDE);
				childhwnd = GetDlgItem(hwnd, IDB_THREE);
				ShowWindow(childhwnd, SW_HIDE);
				childhwnd = GetDlgItem(hwnd, IDB_eight);
				ShowWindow(childhwnd, SW_HIDE);
				mapnum = 1;
				playerbegin();//用户初始化
				bul_num = 0;//子弹数量初始化
				ai_bulnum = 0;
				memset(bullet, 0, sizeof(bullet));//子弹结构体初始化
				memset(AItank, 0, sizeof(AItank));
				memset(aibullet, 0, sizeof(aibullet));
				changemap(hg_app, hwnd);
				
			}
			else
				MessageBox(hwnd, L"请先注册", L"tip", MB_OK);
			
			hThread = CreateThread(NULL, 0, Threadproc, NULL, 0, &threadID);
			break;
		case IDB_six:
			if (puanduan1(hwnd) == 0)
				MessageBox(hwnd, L"注册成功", L"tips", MB_OK);
			else
				MessageBox(hwnd, L"已存在", L"tip", MB_OK);
			break;
		case IDB_THREE:
			chakan(hwnd);
			break;
		case IDB_eight:
			MessageBox(hwnd, L"←↑→↓为方向键\r空格发射炮弹\resc退出游戏", L"happy game", MB_OK);
		default:
			break;
		}
		break;
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		     case VK_LEFT://按下左键
				 player.point = 3;
				 if (map[player.x - 10][player.y ] == 1)
				 player.x -= 10;
				 break;
			 case VK_UP://按下上键
				 player.point = 0;
				 if (map[player.x ][player.y - 10] == 1)
				 player.y -= 10;
				 break;
			 case VK_RIGHT://按下右键
				 player.point = 1;
				 if (map[player.x  + 10][player.y ] == 1)
				 player.x += 10;
				 break;
			 case VK_DOWN://按下下键
				 player.point = 2;
				 if (map[player.x ][player.y +10] == 1)
				 player.y += 10;
				 break;
			 case VK_SPACE:
				 buildbullt();
				 PlaySound(L"Peow.wav", NULL, SND_ASYNC);//发射炮弹时给个音
				 break;
			 case VK_ESCAPE:
				 writedata(hwnd, player.value);
				 sprintf(c, "你的击杀数为%d", player.value);
				 CharToTchar(c, b);
				 MessageBox(hwnd, b, L"游戏结束", MB_OK);
				 PostQuitMessage(0);
				 break;
		}
		flag = 0;
		hdc1 = GetDC(hwnd);
		Drawgame(hdc1,hwnd);
		ReleaseDC(hwnd,hdc1);
		break;
	
	case WM_TIMER:
		switch (wParam)
		{
		case Timer1:
			if (key == 1)
			{
				for (i = 0; i <= bul_num; i++)
				{
					if (bullet[i].point == 0)//上0，右1，下2，左3
						bullet[i].y -= 10;
					if (bullet[i].point == 1)
						bullet[i].x += 10;
					if (bullet[i].point == 2)
						bullet[i].y += 10;
					if (bullet[i].point == 3)
						bullet[i].x -= 10;
				}
				for (i = 0; i < ai_bulnum; i++)
				{
					if (aibullet[i].point == 0)//上0，右1，下2，左3
						aibullet[i].y -= 10;
					if (aibullet[i].point == 1)
						aibullet[i].x += 10;
					if (aibullet[i].point == 2)
						aibullet[i].y += 10;
					if (aibullet[i].point == 3)
						aibullet[i].x -= 10;
				}
				hdc1 = GetDC(hwnd);
				Drawgame(hdc1, hwnd);
				ReleaseDC(hwnd, hdc1);
			}
			break;
		case Timer2:
			if(key==1)
			buildAItank(hwnd);
		case Timer3:
			srand(time(NULL));
			if (key == 1) {
				buildmap();
				for (j = 0; j <= ainum; j++)
				{
					k = (rand() * 8465 + 2156) % 3;//上0，右1，下2，左3
					if (player.x < AItank[j].x && player.y < AItank[j].y)
						if (k <= 1)
							k = 0;
						else
							k = 3;
					if (player.x > AItank[j].x && player.y < AItank[j].y)
						if (k <= 1)
							k = 0;
						else
							k = 1;
					if (player.x < AItank[j].x && player.y > AItank[j].y)
						if (k <= 1)
							k = 2;
						else
							k = 3;
					if (player.x > AItank[j].x && player.y > AItank[j].y)
						if (k <= 1)
							k = 1;
						else
							k = 2;
					if (k == 0 && map[AItank[j].x][AItank[j].y - 5] == 1)
						AItank[j].y -= 5;
					if (k == 1 && map[AItank[j].x + 5][AItank[j].y] == 1)
						AItank[j].x += 5;
					if (k == 2 && map[AItank[j].x][AItank[j].y + 5] == 1)
						AItank[j].y += 5;
					if (k == 3 && map[AItank[j].x - 5][AItank[j].y] == 1)
						AItank[j].x -= 5;
					AItank[j].point = k;
					for (i = AItank[j].x; i <= tank_width; i++)
						for (w = AItank[j].y; w <= tank_high; w++)
							map[i][w] = 2;
				}
			}
			break;
		case Timer4:
			for (i = 0; i <= ainum; i++)
				buildaibullet(i);
			break;
		}
		break;
    }
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI Threadproc(LPVOID ipParam)
{

	return 0;
}

void playerbegin()
{
	if (mapnum == 1)
	{
		player.x = 55;
		player.y = 55;
		player.point = 1;//上0，右1，下2，左3
	}
	player.hp = 1;
	player.value = 0;
	ainum = 0;//当创建玩家tank时重置aitank数量
}

void changemap(HINSTANCE hinstance, HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HDC hmemdc,hmemdc1;
	HBITMAP hbitmap1,hbitmap2;
	int i;
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 600;
	rect.bottom = 600;
	InvalidateRect(hwnd, &rect, TRUE);//此函数在beginpaint函数前调用，即可将原窗口图片抹去
	hdc = BeginPaint(hwnd, &ps);
	if (mapnum == 1)
	{
		hbitmap1 = (HBITMAP)LoadImage(hinstance, L"level1.bmp", IMAGE_BITMAP, 747, 710, LR_LOADFROMFILE);
		hmemdc = CreateCompatibleDC(hdc);
		hmemdc1 = CreateCompatibleDC(hdc);
		SelectObject(hmemdc, hbitmap1);
		StretchBlt(hdc, 0, 0, 600, 600, hmemdc, 0, 0, 757, 757, SRCCOPY);
		hbitmap2 = (HBITMAP)LoadImage(hinstance, L"player1.bmp", IMAGE_BITMAP, 338, 170, LR_LOADFROMFILE);
		SelectObject(hmemdc, hbitmap2);
		Drawgame(hdc,hwnd);//
		DeleteDC(hmemdc);
		DeleteObject(hbitmap1);
		DeleteObject(hbitmap2);
		EndPaint(hwnd, &ps);
		buildmap();
	}
	//if
	//if
	
}

void Drawgame(HDC hdc, HWND hwnd)
{
	HBITMAP tankbmp,mapbmp,bulletbmp,boombmp,aibmp,tboombmp;
	HDC hmemdc,mapdc,bulletdc,boomdc,aidc,tboomdc;
	int i,j,k,w,dx,dy;
	char a[40];
	TCHAR b[20];
	TCHAR c[40];
	tankbmp=(HBITMAP)LoadImage(hg_app, L"player1.bmp", IMAGE_BITMAP, 338, 170, LR_LOADFROMFILE);
	mapbmp = (HBITMAP)LoadImage(hg_app, L"level1.bmp", IMAGE_BITMAP, 747, 710, LR_LOADFROMFILE);
	bulletbmp = (HBITMAP)LoadImage(hg_app, L"bore.bmp", IMAGE_BITMAP, 198, 54, LR_LOADFROMFILE);
	boombmp = (HBITMAP)LoadImage(hg_app, L"explode1.bmp", IMAGE_BITMAP, 45, 45, LR_LOADFROMFILE);
	aibmp = (HBITMAP)LoadImage(hg_app, L"enemy.bmp", IMAGE_BITMAP, 337, 339, LR_LOADFROMFILE);
	tboombmp = (HBITMAP)LoadImage(hg_app, L"explode2.bmp", IMAGE_BITMAP, 99, 99, LR_LOADFROMFILE);
	hmemdc= CreateCompatibleDC(hdc);
	mapdc = CreateCompatibleDC(hdc);
	bulletdc = CreateCompatibleDC(hdc);
	boomdc = CreateCompatibleDC(hdc);
	aidc = CreateCompatibleDC(hdc);
	tboomdc = CreateCompatibleDC(hdc);
	SelectObject(hmemdc, tankbmp);
	SelectObject(mapdc, mapbmp);
	SelectObject(bulletdc, bulletbmp);
	SelectObject(boomdc, boombmp);
	SelectObject(aidc, aibmp);
	SelectObject(tboomdc, tboombmp);
	StretchBlt(hdc, 0, 0, 600, 600, mapdc, 0, 0, 757, 757, SRCCOPY);
	i = player.point;
	StretchBlt(hdc, player.x, player.y, tank_width, tank_high, hmemdc, 0, i * tank_high, tank_width, tank_high, SRCAND);
	StretchBlt(hdc, player.x, player.y, tank_width, tank_high, hmemdc, 0, i * tank_high, tank_width, tank_high, SRCPAINT);
	srand(time(NULL));
	for (j = 0; j <= ainum; j++)//AItank移动
		if (AItank[j].hp != 0)
			StretchBlt(hdc, AItank[j].x, AItank[j].y, ai_width, ai_high, aidc, 0, AItank[j].point * ai_high, ai_width, ai_high, SRCCOPY);
    for(j=0;j<=bul_num;j++)//玩家子弹移动
		if (bullet[j].exist != 0)
		{
			StretchBlt(hdc, bullet[j].x, bullet[j].y, bul_width / 2, bul_high / 2, bulletdc, 0, 0, bul_width, bul_high, SRCCOPY);
			if (map[bullet[j].x][bullet[j].y] == 0)
			{
				bullet[j].exist = 0;
				StretchBlt(hdc, bullet[j].x, bullet[j].y, bul_width / 2, bul_high / 2, boomdc, 0, 0, boomwidth, boomhigh, SRCCOPY);
			}
			for (w = 0; w <= ainum; w++)
			{
				dx = abs(bullet[j].x - AItank[w].x - bul_width / 2);
				dy = abs(bullet[j].y - AItank[w].y - bul_high / 2);
				if (((dx <= tank_width && dx >= 0) && (dy >= 0 && dy <= tank_high)) && AItank[w].hp!=0)
				{
					bullet[j].exist = 0;
					StretchBlt(hdc, bullet[j].x, bullet[j].y, bul_width / 2, bul_high / 2, boomdc, 0, 0, boomwidth, boomhigh, SRCCOPY);
					cleanaitank(w);
					StretchBlt(hdc, AItank[w].x, AItank[w].y, ai_width, ai_high, tboomdc, 0, 0, 99, 99, SRCCOPY);
					player.value++;
				}
			}
		}
	for(j=0;j<=ai_bulnum;j++)//电脑子弹移动
		if (aibullet[j].exist == 1)
		{
			StretchBlt(hdc, aibullet[j].x, aibullet[j].y, bul_width / 2, bul_high / 2, bulletdc, 0, 0, bul_width, bul_high, SRCCOPY);
			if (map[aibullet[j].x][aibullet[j].y] == 0)
			{
				aibullet[j].exist = 0;
				StretchBlt(hdc, aibullet[j].x, aibullet[j].y, bul_width / 2, bul_high / 2, boomdc, 0, 0, boomwidth, boomhigh, SRCCOPY);
			}
			dx = abs(aibullet[j].x - player.x - bul_width / 2);
			dy = abs(aibullet[j].y - player.y - bul_high / 2);
			if (((dx <= tank_width && dx>=0) && (dy>=0&& dy <= tank_high)) && aibullet[j].exist!=0)
			{
				player.hp--;
				StretchBlt(hdc, aibullet[j].x, aibullet[j].y, bul_width / 2, bul_high / 2, boomdc, 0, 0, boomwidth, boomhigh, SRCCOPY);
				aibullet[j].exist = 0;
				if (player.hp == 0)
				{
					writedata(hwnd, player.value);
					StretchBlt(hdc, player.x, player.y, tank_width, tank_high, tboomdc, 0, 0, 99, 99, SRCCOPY);
					sprintf(a, "你的击杀数为%d", player.value);
					CharToTchar(a, b);
					MessageBox(hwnd, b, L"你凉了", MB_OK);
					PostQuitMessage(0);
				}
			}
		}
	DeleteDC(hmemdc);
	DeleteDC(mapdc);
	DeleteDC(bulletdc);
	DeleteDC(boomdc);
	DeleteDC(aidc);
	DeleteDC(tboomdc);
	DeleteObject(tankbmp);
	DeleteObject(mapbmp);
	DeleteObject(bulletbmp);
	DeleteObject(boombmp);
	DeleteObject(aibmp);
	DeleteObject(tboombmp);
}

void buildbullt()
{
	switch (player.point)//上0，右1，下2，左3
	{
	case 0:
		bullet[bul_num].point = 0;
		bullet[bul_num].x = player.x + 10;
		bullet[bul_num].y = player.y;
		break;
	case 1:
		bullet[bul_num].point = 1;
		bullet[bul_num].x = player.x + tank_width;
		bullet[bul_num].y = player.y + 10;
		break;
	case 2:
		bullet[bul_num].point = 2;
		bullet[bul_num].x = player.x + 10;
		bullet[bul_num].y = player.y + tank_high;
		break;
	case 3:
		bullet[bul_num].point = 3;
		bullet[bul_num].x = player.x;
		bullet[bul_num].y = player.y + 10;
		break;
	}
	bullet[bul_num].exist = 1;    //子弹被建立,此值为1则此子弹存在
	bul_num++;
	if (bul_num == 20)        //如果子弹编号增长到20号，那么重头开始编号
		bul_num = 0;
}


void buildaibullet(int k)
{
	switch (AItank[k].point)//上0，右1，下2，左3
	{
	case 0:
		aibullet[ai_bulnum].point = 0;
		aibullet[ai_bulnum].x = AItank[k].x + 10;
		aibullet[ai_bulnum].y = AItank[k].y;
		break;
	case 1:
		aibullet[ai_bulnum].point = 1;
		aibullet[ai_bulnum].x = AItank[k].x + tank_width;
		aibullet[ai_bulnum].y = AItank[k].y + 10;
		break;
	case 2:
		aibullet[ai_bulnum].point = 2;
		aibullet[ai_bulnum].x = AItank[k].x + 10;
		aibullet[ai_bulnum].y = AItank[k].y + tank_high;
		break;
	case 3:
		aibullet[ai_bulnum].point = 3;
		aibullet[ai_bulnum].x = AItank[k].x;
		aibullet[ai_bulnum].y = AItank[k].y + 10;
		break;
	}
	aibullet[ai_bulnum].exist = 1;    //子弹被建立,此值为1则此子弹存在
	ai_bulnum++;
	if (ai_bulnum == 80)        //如果子弹编号增长到20号，那么重头开始编号
		ai_bulnum = 0;
}


void buildmap()
{
	int i, j;
	memset(map, 0, sizeof(map));
	if (mapnum == 1)
	{
		for (i = 33; i <= 543; i++)
			for (j = 33; j <= 510; j++)
				map[i][j] = 1;
	}

}

void buildAItank(HWND hwnd)
{
	if (ainum > 12)//当aitank数量>10停止生成
		ainum = 0;
	srand(time(NULL));
	int k1, k2;
	k1 = 100+abs(rand()*1285+45)%400;
	k2 = 100 + abs(rand()*4568+60)%400;
	if (mapnum == 1)
	{
		AItank[ainum].x = k1;
		AItank[ainum].y = k2;
		AItank[ainum].point = (k1 + k2) % 3;
	}
	AItank[ainum].hp = 1;
	ainum++;
}

void cleanaitank(int w)
{
	AItank[w].x = 0;
	AItank[w].y = 0;
	AItank[w].point = 0;
	AItank[w].hp = 0;
}

