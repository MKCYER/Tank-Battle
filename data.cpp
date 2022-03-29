#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Resource.h"
#include "head.h"
typedef struct userdata {
	char name[5];
	int value;
}user;//用户数据结构体
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	//获取字节长度    
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char     
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void CharToTchar(const char* _char, TCHAR* tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
/*int denglu(HWND hwnd)
{
	TCHAR a[10],w[10],b[10],m[1];
	char c[20], d[20];
	int k;
	HDC hdc;
	HANDLE hFile = CreateFile(TEXT("d:\\userdata.txt"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	int i, flag;
	flag = 0;
	DWORD read, write;
	GetDlgItemText(hwnd, IDB_four, a, 6);
	hdc = GetDC(hwnd);
	int j = 0;
	i = 0;
	ReadFile(hFile, m, 2, &read, NULL);//unicon文件第一个字符为0xff用于说明此文件为unicon类型的文件
	memset(b, 0, sizeof(b));
	while (1)
	{
		ReadFile(hFile, b, 10, &read, NULL);
		if (lstrlen(b) != 5)
			break;
		if (lstrcmp(a, b) == 0)
		{
			flag = 1;
			break;
		}
	}
	CloseHandle(hFile);
	return flag;

}
int puanduan(HWND hwnd)
{
	TCHAR a[10],b[10],m[1];
	char c[20],d[20];
	int k;
	HDC hdc;
	HANDLE hFile = CreateFile(TEXT("d:\\userdata.txt"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	int i,flag;
	flag = 0;
	DWORD read, write;
	GetDlgItemText(hwnd, IDB_seve, a, 6);
	hdc = GetDC(hwnd);
	int j=0;
	i = 0;
	ReadFile(hFile, m, 2, &read, NULL);
	memset(b, 0, sizeof(b));
	while(1)
	{
		ReadFile(hFile, b, 10, &read, NULL);
		if (lstrlen(b) != 5)
			break;
		if (lstrcmp(a, b) == 0)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		WriteFile(hFile, a, 10, &write, NULL);
	CloseHandle(hFile);
		return flag;
}
*/
int puanduan1(HWND hwnd)
{
	FILE* fp;
	TCHAR b[10];
	char c[10];
	int flag = 0;
	user a;
	fp = fopen("userdata1.txt","a+");
	GetDlgItemText(hwnd, IDB_seve, b, 6);
	TcharToChar(b, c);
	while (!feof(fp))
	{
		fscanf(fp, "%s %d\n", &a.name, &a.value);
		if (strcmp(a.name, c) == 0)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		fprintf(fp, "%s %d\n", c, 0);
	fclose(fp);
	return flag;
}

int denglu1(HWND hwnd)
{
	FILE* fp;
	TCHAR b[10];
	char c[10];
	int flag = 0;
	user a;
	fp = fopen("userdata1.txt", "a+");
	GetDlgItemText(hwnd, IDB_four, b, 6);
	TcharToChar(b, c);
	while (!feof(fp))
	{
		fscanf(fp, "%s %d\n", &a.name, &a.value);
		if (strcmp(a.name, c) == 0)
		{
			flag = 1;
			break;
		}
	}
	fclose(fp);
	return flag;
}
int chakan(HWND hwnd)
{
	FILE* fp;
	char c[10];
	char put[50];
	TCHAR put1[50];
	memset(put, 0, sizeof(put));
	int flag = 0;
	user a[100];
	int j, k, maxi;
	j = 0;
	k = -1;
	fp = fopen("userdata1.txt", "a+");
	while (!feof(fp))
	{
		fscanf(fp, "%s %d\n", &a[j].name, &a[j].value);
		if (k < a[j].value)
		{
			k = a[j].value;
			maxi = j;
		}
		j++;
	}
	sprintf(put, "用户名：%s 得分:%d", a[maxi].name, a[maxi].value);
	CharToTchar(put, put1);
	MessageBox(hwnd, put1, L"tip", MB_OK);
	fclose(fp);
	return flag;
}

void writedata(HWND hwnd,int x)
{
	FILE* fp;
	TCHAR b[10];
	char c[10];
	int flag = 0;
	user a;
	fp = fopen("userdata1.txt", "a+");
	GetDlgItemText(hwnd, IDB_four, b, 6);
	TcharToChar(b, c);
	fprintf(fp, "%s %d\n", c, x);
	fclose(fp);
}