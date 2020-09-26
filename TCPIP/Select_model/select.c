
//#define FD_SETSIZE 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

fd_set  AllSockets;

BOOL WINAPI CloseAllSockets(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		for (u_int k = 0; k < AllSockets.fd_count; k++)
		{
			closesocket(AllSockets.fd_array[k]);
		}
		WSACleanup(); //清理网络库
	}
	return TRUE;
}

int main()
{
	SetConsoleCtrlHandler(CloseAllSockets,TRUE);
	//1打开网络库
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA wdSockInfo;
	int err = WSAStartup(wdVersion,&wdSockInfo);

	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return 0;
	}

	//2校验版本号
	if (2 != HIBYTE(wdSockInfo.wVersion) || 2!=LOBYTE(wdSockInfo.wVersion))
	{
		WSACleanup();
		return 0;
	}

	//3创建socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socketServer)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}

	//4绑定端口和IP地址
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(12345);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	int bind_result = bind(socketServer, (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (bind_result == SOCKET_ERROR)
	{
		int bind_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//5 listen函数
	int listen_result = listen(socketServer, SOMAXCONN);
	if (listen_result == SOCKET_ERROR)
	{
		int listen_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//6 select函数
	//fd_set  AllSockets;
	
	//清零
	FD_ZERO(&AllSockets);
	//添加服务端socket
	FD_SET(socketServer , &AllSockets);

	struct timeval timeout;	//时间段
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	while (1)
	{
		fd_set accept_recv_sockets = AllSockets;  //每次循环将所有的Sockets赋值给accept_recv_sockets  修改的是TempSokets
											//用于recv和accept
		fd_set sendSockets = AllSockets;   //用于send
		//FD_CLR(socketServer,&AllSockets);

		int select_result = select(0,&accept_recv_sockets,&sendSockets, NULL,&timeout);
		
		if (0 == select_result)  //无响应
		{
			continue;
		}
		else if(0 < select_result)  //有响应
		{
			for (u_int j = 0; j < sendSockets.fd_count; j++)
			{
				int send_result = send(sendSockets.fd_array[j],"Received",sizeof("Received"),0);
				if (send_result == SOCKET_ERROR)
				{
					int send_error = WSAGetLastError();
					continue;
				}
			}

			for (u_int i = 0; i < accept_recv_sockets.fd_count; i++)
			{
				if (accept_recv_sockets.fd_array[i] == socketServer)
				{
					//accept
					SOCKET socketClient = accept(socketServer, NULL, NULL);
					if (socketClient == INVALID_SOCKET)
					{
						//accept出错
						continue;
					}
					else
					{
						printf("客户端%d连接成功。\n",socketClient);
						FD_SET(socketClient,&AllSockets);
						send(socketClient, "服务器收到连接", sizeof("服务器收到连接"), 0);
					}
				}
				else
				{
					//recv
					char buf[1000] = { 0 };
					int recv_result = recv(accept_recv_sockets.fd_array[i], buf, 60, 0);
					if (0 == recv_result)
					{
						//客服端下线
						SOCKET tempsock = accept_recv_sockets.fd_array[i];
						FD_CLR(accept_recv_sockets.fd_array[i],&AllSockets);  //clear socket
						closesocket(tempsock);
					}
					else if (SOCKET_ERROR == recv_result)
					{
						//出错
						int	recv_error = WSAGetLastError();
						//远程主机强迫关闭了一个现有的连接。   错误码10054
						switch(recv_error)
						{
							case 10054:
							{
								SOCKET tempsock1 = accept_recv_sockets.fd_array[i];
								FD_CLR(accept_recv_sockets.fd_array[i], &AllSockets);  //clear socket
								closesocket(tempsock1);
								continue;
							}
						}
					}
					else
					{
						//正确 打印
						printf("client %d send : %s\n", accept_recv_sockets.fd_array[i], buf);
					}
				}
			}
		}
		else   //出错
		{
			int select_error = WSAGetLastError();
			break;
		}
	}

	for (u_int k = 0; k < AllSockets.fd_count; k++)
	{
		closesocket(AllSockets.fd_array[k]);
	}
	WSACleanup(); //清理网络库

	system("pause");
	return 0;
}