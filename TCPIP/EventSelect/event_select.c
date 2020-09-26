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
	//1 ������� �� У��汾��
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

	//2 ����socket
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//���
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

	//4 ��ʼ����	//listen����
	int listen_result = listen(socketServer, SOMAXCONN);
	if (listen_result == SOCKET_ERROR)
	{
		int listen_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}


	//����һ���¼�
		//WSAEVENT WSAAPI WSACreateEvent();
	WSAEVENT eventServer = WSACreateEvent();
	if (eventServer == WSA_INVALID_EVENT)
	{
		int event_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}

	//�󶨲�Ͷ��
	int select_result = WSAEventSelect(socketServer, eventServer, FD_ACCEPT);
	if (select_result == SOCKET_ERROR)
	{
		WSACloseEvent(eventServer);
		closesocket(socketServer);
		WSACleanup();
	}

	//װ���ṹ��
	FD_EVESOC_SET evesocset = { 0,{0} ,{0} };
	evesocset.allSocket[evesocset.count] = socketServer;
	evesocset.allEvent[evesocset.count] = eventServer;
	evesocset.count++;

	
	while (1)
	{	
		//ѯ���¼�
		DWORD wait_result = WSAWaitForMultipleEvents(evesocset.count, evesocset.allEvent, FALSE, WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == wait_result)
		{
			int wait_error = WSAGetLastError();
			printf("������:%d", wait_error);
			break;
		}
		//�õ��±�
		DWORD setindex = wait_result - WSA_WAIT_EVENT_0;

		//�õ��±��ľ������
		WSANETWORKEVENTS NetworkEvents;
		int enumevent_result = WSAEnumNetworkEvents(evesocset.allSocket[setindex], evesocset.allEvent[setindex], &NetworkEvents);
		if (SOCKET_ERROR == enumevent_result)
		{
			int enumevent_error = WSAGetLastError();
			printf("������:%d", enumevent_error);
			break;
		}	
		
		//�о��¼� FD_ACCEPT
		if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (0 == NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
			{	//��������
				SOCKET socketClient = accept(evesocset.allSocket[setindex],NULL,NULL);
				if (INVALID_SOCKET == socketClient)
				{
					continue;
				}

				//�����¼�����
				WSAEVENT eventClient = WSACreateEvent();
				if (WSA_INVALID_EVENT == eventClient)
				{
					closesocket(socketClient);
					continue;
				}

				//Ͷ�ݸ�ϵͳ
				if (SOCKET_ERROR == WSAEventSelect(socketClient, eventClient, FD_READ | FD_CLOSE | FD_WRITE))
				{
					WSACloseEvent(eventClient);
					//WSACloseEvent(eventServer);
					closesocket(socketClient);
					continue;
				}

				//װ���ṹ��
				evesocset.allSocket[evesocset.count] = socketClient;
				evesocset.allEvent[evesocset.count] = eventClient;
				evesocset.count++;
			}
			else
			{//������
				continue;
			}
		}

		//�о��¼� FD_WRITE
		if (NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if (0 == NetworkEvents.iErrorCode[FD_WRITE_BIT])
			{	//��������  ����send()
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

		//�о��¼� FD_READ
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

		//�о��¼� FD_CLOSE
		if (NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			if(evesocset.count > 0)
			{
				//�����׽���
				closesocket(evesocset.allSocket[setindex]);
				evesocset.allSocket[setindex] = evesocset.allSocket[evesocset.count - 1];
				//�����¼�
				WSACloseEvent(evesocset.allEvent[setindex]);
				evesocset.allEvent[setindex] = evesocset.allEvent[evesocset.count - 1];
				//count�Լ�
				evesocset.count--;
			}
		}
	}

	//�ͷ�������¼����׽���
	for (int i = 0; i < evesocset.count; i++)
	{
		closesocket(evesocset.allSocket[i]);
		WSACloseEvent(evesocset.allEvent[i]);
	}
	evesocset.count = 0;

	//�رվ��
	BOOL close_result = WSACloseEvent(eventServer);

	closesocket(socketServer);
	WSACleanup();
	system("pause");
	return 0;
}

