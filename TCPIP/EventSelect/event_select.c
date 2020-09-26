#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

typedef struct fd_evesoc_set
{
	short count;
	SOCKET allSocket[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT allEvent[WSA_MAXIMUM_WAIT_EVENTS];
}FD_EVESOC_SET;

int main()
{
	//1 打开网络库 并 校验版本号
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA lpWSAData;
	int start_result = WSAStartup(wdVersion, &lpWSAData);
	if (start_result != 0)
	{
		printf("WSAStartup failed with error: %d\n", start_result);
		return 1;
	}
	if (2 != HIBYTE(lpWSAData.wVersion) || 2 != LOBYTE(lpWSAData.wVersion))
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 0;
	}

	//2 创建socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//检测
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

	//4 开始监听	//listen函数
	int listen_result = listen(socketServer, SOMAXCONN);
	if (listen_result == SOCKET_ERROR)
	{
		int listen_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}


	//创建一个事件
		//WSAEVENT WSAAPI WSACreateEvent();
	WSAEVENT eventServer = WSACreateEvent();
	if (eventServer == WSA_INVALID_EVENT)
	{
		int event_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//绑定并投递
	int select_result = WSAEventSelect(socketServer, eventServer, FD_ACCEPT);
	if (select_result == SOCKET_ERROR)
	{
		WSACloseEvent(eventServer);
		closesocket(socketServer);
		WSACleanup();
	}

	//装进结构体
	FD_EVESOC_SET evesocset = { 0,{0} ,{0} };
	evesocset.allSocket[evesocset.count] = socketServer;
	evesocset.allEvent[evesocset.count] = eventServer;
	evesocset.count++;

	
	while (1)
	{	
		//询问事件
		DWORD wait_result = WSAWaitForMultipleEvents(evesocset.count, evesocset.allEvent, FALSE, WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == wait_result)
		{
			int wait_error = WSAGetLastError();
			printf("错误码:%d", wait_error);
			break;
		}
		//得到下标
		DWORD setindex = wait_result - WSA_WAIT_EVENT_0;

		//得到下标后的具体操作
		WSANETWORKEVENTS NetworkEvents;
		int enumevent_result = WSAEnumNetworkEvents(evesocset.allSocket[setindex], evesocset.allEvent[setindex], &NetworkEvents);
		if (SOCKET_ERROR == enumevent_result)
		{
			int enumevent_error = WSAGetLastError();
			printf("错误码:%d", enumevent_error);
			break;
		}	
		
		//列举事件 FD_ACCEPT
		if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (0 == NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
			{	//正常处理
				SOCKET socketClient = accept(evesocset.allSocket[setindex],NULL,NULL);
				if (INVALID_SOCKET == socketClient)
				{
					continue;
				}

				//创建事件对象
				WSAEVENT eventClient = WSACreateEvent();
				if (WSA_INVALID_EVENT == eventClient)
				{
					closesocket(socketClient);
					continue;
				}

				//投递给系统
				if (SOCKET_ERROR == WSAEventSelect(socketClient, eventClient, FD_READ | FD_CLOSE | FD_WRITE))
				{
					WSACloseEvent(eventClient);
					//WSACloseEvent(eventServer);
					closesocket(socketClient);
					continue;
				}

				//装进结构体
				evesocset.allSocket[evesocset.count] = socketClient;
				evesocset.allEvent[evesocset.count] = eventClient;
				evesocset.count++;
			}
			else
			{//出错处理
				continue;
			}
		}

		//列举事件 FD_WRITE
		if (NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if (0 == NetworkEvents.iErrorCode[FD_WRITE_BIT])
			{	//正常处理  调用send()
				if (SOCKET_ERROR == send(evesocset.allSocket[setindex], "CONNECTED", strlen("CONNECTED"), 0))
				{
					int send_error = WSAGetLastError();
					//closesocket(socketClient);
					printf("send error code : %d", send_error);
					continue;
				}
			}
			else
			{
				continue;
			}
		}

		//列举事件 FD_READ
		char buf[1000] = { 0 };
		if (NetworkEvents.lNetworkEvents & FD_READ)
		{
			if (0 == NetworkEvents.iErrorCode[FD_READ_BIT])
			{
				int recv_result = recv(evesocset.allSocket[setindex],buf,1499,0);
				if (SOCKET_ERROR == recv_result)
				{
					int recv_error = WSAGetLastError();
					printf("recv error code : %d",recv_error);
					continue;
				}
				else
				{
					printf("Client %d sends : %s \n", setindex, buf);
				}
			}
			else
			{
				continue;
			}
		}

		//列举事件 FD_CLOSE
		if (NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			if(evesocset.count > 0)
			{
				//清理套接字
				closesocket(evesocset.allSocket[setindex]);
				evesocset.allSocket[setindex] = evesocset.allSocket[evesocset.count - 1];
				//清理事件
				WSACloseEvent(evesocset.allEvent[setindex]);
				evesocset.allEvent[setindex] = evesocset.allEvent[evesocset.count - 1];
				//count自减
				evesocset.count--;
			}
		}
	}

	//释放数组的事件和套接字
	for (int i = 0; i < evesocset.count; i++)
	{
		closesocket(evesocset.allSocket[i]);
		WSACloseEvent(evesocset.allEvent[i]);
	}
	evesocset.count = 0;

	//关闭句柄
	BOOL close_result = WSACloseEvent(eventServer);

	closesocket(socketServer);
	WSACleanup();
	system("pause");
	return 0;
}

