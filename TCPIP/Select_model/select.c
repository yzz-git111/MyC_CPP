
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
		WSACleanup(); //���������
	}
	return TRUE;
}

int main()
{
	SetConsoleCtrlHandler(CloseAllSockets,TRUE);
	//1�������
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA wdSockInfo;
	int err = WSAStartup(wdVersion,&wdSockInfo);

	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return 0;
	}

	//2У��汾��
	if (2 != HIBYTE(wdSockInfo.wVersion) || 2!=LOBYTE(wdSockInfo.wVersion))
	{
		WSACleanup();
		return 0;
	}

	//3����socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socketServer)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}

	//4�󶨶˿ں�IP��ַ
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

	//5 listen����
	int listen_result = listen(socketServer, SOMAXCONN);
	if (listen_result == SOCKET_ERROR)
	{
		int listen_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//6 select����
	//fd_set  AllSockets;
	
	//����
	FD_ZERO(&AllSockets);
	//��ӷ����socket
	FD_SET(socketServer , &AllSockets);

	struct timeval timeout;	//ʱ���
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	while (1)
	{
		fd_set accept_recv_sockets = AllSockets;  //ÿ��ѭ�������е�Sockets��ֵ��accept_recv_sockets  �޸ĵ���TempSokets
											//����recv��accept
		fd_set sendSockets = AllSockets;   //����send
		//FD_CLR(socketServer,&AllSockets);

		int select_result = select(0,&accept_recv_sockets,&sendSockets, NULL,&timeout);
		
		if (0 == select_result)  //����Ӧ
		{
			continue;
		}
		else if(0 < select_result)  //����Ӧ
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
						//accept����
						continue;
					}
					else
					{
						printf("�ͻ���%d���ӳɹ���\n",socketClient);
						FD_SET(socketClient,&AllSockets);
						send(socketClient, "�������յ�����", sizeof("�������յ�����"), 0);
					}
				}
				else
				{
					//recv
					char buf[1000] = { 0 };
					int recv_result = recv(accept_recv_sockets.fd_array[i], buf, 60, 0);
					if (0 == recv_result)
					{
						//�ͷ�������
						SOCKET tempsock = accept_recv_sockets.fd_array[i];
						FD_CLR(accept_recv_sockets.fd_array[i],&AllSockets);  //clear socket
						closesocket(tempsock);
					}
					else if (SOCKET_ERROR == recv_result)
					{
						//����
						int	recv_error = WSAGetLastError();
						//Զ������ǿ�ȹر���һ�����е����ӡ�   ������10054
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
						//��ȷ ��ӡ
						printf("client %d send : %s\n", accept_recv_sockets.fd_array[i], buf);
					}
				}
			}
		}
		else   //����
		{
			int select_error = WSAGetLastError();
			break;
		}
	}

	for (u_int k = 0; k < AllSockets.fd_count; k++)
	{
		closesocket(AllSockets.fd_array[k]);
	}
	WSACleanup(); //���������

	system("pause");
	return 0;
}