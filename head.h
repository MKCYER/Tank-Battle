#pragma once
int puanduan(HWND hwnd);
int denglu(HWND hwnd);
void TcharToChar(const TCHAR* tchar, char* _char);
void CharToTchar(const char* _char, TCHAR* tchar);
void changemap(HINSTANCE hinstance, HWND hwnd);
void Drawgame(HDC hdc, HWND hwnd);
int puanduan1(HWND hwnd);
int denglu1(HWND hwnd);
int chakan(HWND hwnd);
void buildbullt();
void playerbegin();
void buildmap();
void buildAItank(HWND hwnd);
void cleanaitank(int w);
void buildaibullet(int k);
void writedata(HWND hwnd,int x);