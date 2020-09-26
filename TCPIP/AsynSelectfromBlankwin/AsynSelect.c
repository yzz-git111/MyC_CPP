//���ڵ�ͷ�ļ�  �������ͷ�ļ��ظ�������
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
//windows���ڵ���������WinMain  WINAPI��windows�������ڵ�API��ʶ��
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

	//ע��ṹ��
	RegisterClassEx(&win);

	//��������
	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"WinbyC", L"�첽ѡ�񴰿�", WS_OVERLAPPEDWINDOW, 200, 200, 600, 400, NULL, NULL, hInstance, NULL);
	if (NULL == hwnd)
	{
		return 0;
	}

	//��ʾ����
	ShowWindow(hwnd, nShowCmd);	//nShowCmdĬ��1

	//���´���
	UpdateWindow(hwnd);



/*===============�ⲿ��������Ĳ���===============*/

	//1 ������� ������汾��
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

	//2 ����socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socketServer)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}

	//3 ��
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

	//4 listen����
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

/*===============���ϲ���������Ĳ���===============*/



	//��Ϣѭ��
	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))  //�������з�Χ����Ϣ
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

//�ص�����
LRESULT CALLBACK WinBackFun(HWND hwnd, UINT msgID, WPARAM wparam, LPARAM lparam)
{
	HDC hdc = GetDC(hwnd);
	switch (msgID)	//�˳�
	{
	case UM_ASYNSELECT:
	{
		//MessageBox(NULL, L"���ź�",L"��ʾ",MB_OK);
		//��ȡsocket
		SOCKET socketTemp = (SOCKET)wparam;
		//��ȡ��Ϣ
		if (0 != HIWORD(lparam))
		{
			break;  //HIWORD������0���д���
		}
		switch (LOWORD(lparam))
		{
		case FD_ACCEPT:
		{
			
			TextOut(hdc, 0, y, L"���ճɹ�", strlen("���ճɹ�"));  //���� ��0,0��
			y += 15;
			SOCKET socketClient = accept(socketTemp, NULL, NULL);
			if (INVALID_SOCKET == socketClient)
			{	//����
				int error = WSAGetLastError();
				printf("������:%d", error);
				break;
			}
			//socketClientͶ�ݸ�ϵͳ
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
			//�رո�socket�ϵ���Ϣ
			WSAAsyncSelect(sock ,hwnd,0,0);
			//�ر�socket
			closesocket(sock);
			//��������ɾ��
			for (int i = 0; i < sockcount; i++)
			{
				if (sock == sock[sockcount])
				{
					sock[i] = sock[sockcount - 1]; //���һ����ֵ����i��
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