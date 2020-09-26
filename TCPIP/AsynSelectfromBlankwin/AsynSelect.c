//窗口的头文件  与网络的头文件重复包含了
//#include <windows.h>   
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#define UM_ASYNSELECT WM_USER + 1

LRESULT CALLBACK WinBackFun(HWND hwnd, UINT msgID, WPARAM wparam, LPARAM lparam);

#define MAX_SOCK_NUM 1024
SOCKET sock[MAX_SOCK_NUM] = { 0 };
int sockcount = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpcmdLine, int nShowCmd)
//windows窗口的主函数是WinMain  WINAPI是windows创建窗口的API标识符
{
	WNDCLASSEX win;
	win.cbClsExtra = 0;
	win.cbSize = sizeof(win);
	win.cbWndExtra = 0;
	win.hbrBackground = NULL;
	win.hCursor = NULL;
	win.hIcon = NULL;
	win.hIconSm = NULL;
	win.hInstance = hInstance;
	win.lpfnWndProc = WinBackFun;
	win.lpszClassName = L"WinbyC";	//wchar_t	LPCWSTR
	win.lpszMenuName = NULL;
	win.style = CS_HREDRAW | CS_VREDRAW;

	//注册结构体
	RegisterClassEx(&win);

	//创建窗口
	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"WinbyC", L"异步选择窗口", WS_OVERLAPPEDWINDOW, 200, 200, 600, 400, NULL, NULL, hInstance, NULL);
	if (NULL == hwnd)
	{
		return 0;
	}

	//显示窗口
	ShowWindow(hwnd, nShowCmd);	//nShowCmd默认1

	//更新窗口
	UpdateWindow(hwnd);



/*===============这部分是网络的部分===============*/

	//1 打开网络库 并检验版本号
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA lpWSADATA;
	int start_result = WSAStartup(wdVersion, &lpWSADATA);
	if (start_result != 0)
	{
		printf("WSAStartup failed with error : %d\n", start_result);
		return 0;
	}
	if (2 != HIBYTE(lpWSADATA.wVersion) || 2 != LOBYTE(lpWSADATA.wVersion))
	{
		printf("Could not find a usable version of winsock.dll\n");
		WSACleanup();
		return 0;
	}

	//2 创建socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socketServer)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}

	//3 绑定
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(12345);
	int bind_result = bind(socketServer, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == bind_result)
	{
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//4 listen函数
	int listen_result = listen(socketServer, SOMAXCONN);
	if (SOCKET_ERROR == listen_result)
	{
		int listen_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	if (SOCKET_ERROR == WSAAsyncSelect(socketServer, hwnd, UM_ASYNSELECT, FD_ACCEPT))
	{
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}
	sock[sockcount] = socketServer;
	sockcount++;

/*===============以上部分是网络的部分===============*/



	//消息循环
	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))  //接受所有范围的消息
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	for (int i = 0; i < sockcount; i++)
	{
		closesocket(sock[i]);
	}
	return 0;
}

int y = 0;

//回调函数
LRESULT CALLBACK WinBackFun(HWND hwnd, UINT msgID, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	switch (msgID)	//退出
	{
	case UM_ASYNSELECT:
	{
		//MessageBox(NULL, L"有信号",L"提示",MB_OK);
		//获取socket
		SOCKET socketTemp = (SOCKET)wparam;
		//获取消息
		if (0 != HIWORD(lparam))
		{
			break;  //HIWORD不等于0则有错误
		}
		switch (LOWORD(lparam))
		{
		case FD_ACCEPT:
		{
			
			TextOut(hdc, 0, y, L"接收成功", strlen("接收成功"));  //坐标 （0,0）
			y += 15;
			SOCKET socketClient = accept(socketTemp, NULL, NULL);
			if (INVALID_SOCKET == socketClient)
			{	//出错
				int error = WSAGetLastError();
				printf("错误码:%d", error);
				break;
			}
			//socketClient投递给系统
			if (SOCKET_ERROR == WSAAsyncSelect(socketClient, hwnd, UM_ASYNSELECT, FD_CLOSE | FD_READ | FD_WRITE))
			{
				closesocket(socketClient);
				WSACleanup();
				return 0;
			}
			sock[sockcount] = socketClient;
			sockcount++;
			break;
		}
		case FD_READ:
		{
			char buf[1000] = {0};
			if (SOCKET_ERROR == recv(socketTemp, buf, 960, 0))
			{
				break;
			}
			TextOut(hdc,0,y,L"Client send",11);
			y += 20;
			TextOut(hdc, 20, y, buf, strlen(buf));
			y += 20;
			break;
		}
		case  FD_WRITE:
			TextOut(hdc, 0, y, L"write success", strlen("write success"));
			break;
		case FD_CLOSE:
			//关闭该socket上的消息
			WSAAsyncSelect(sock ,hwnd,0,0);
			//关闭socket
			closesocket(sock);
			//在数组中删除
			for (int i = 0; i < sockcount; i++)
			{
				if (sock == sock[sockcount])
				{
					sock[i] = sock[sockcount - 1]; //最后一个赋值给第i个
					sockcount--;
					break;
				}
			}
			break;
		}
	}
	case WM_CREATE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	ReleaseDC(hwnd,hdc);

	return DefWindowProc(hwnd, msgID, wparam, lparam);
}