#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)


int main()
{
	//1 ������ⲢУ��汾��
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA wdSockInfo;

	int RESULT = WSAStartup(wdVersion, &wdSockInfo);
	if (0 != RESULT)
	{
		switch (RESULT)
		{
		case WSASYSNOTREADY:
			printf("�������Ի���·���Ƿ���ȷ");
			break;
		case WSAVERNOTSUPPORTED:
			printf("�汾��һ��");
			break;
		case WSAEINPROGRESS:
			printf("���������");
			break;
		case WSAEPROCLIM:
			printf("���������");
			break;
		case WSAEFAULT:
			printf("����2����");
			break;
		}
		return 0;
	}

	if (2 != HIBYTE(wdSockInfo.wVersion) || 2 != LOBYTE(wdSockInfo.wVersion))
	{
		WSACleanup();
		return 0;
	}


	//2 ����������socket
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}


	//3 ���ӵ�������
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(12345);			//�������Ķ˿ں�
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //��������IP��ַ

	int connect_result = connect(ServerSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (connect_result == SOCKET_ERROR)
	{
		int connect_error = WSAGetLastError();
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}

	//send(ServerSocket, "�ͻ����յ���Ϣ", sizeof("�ͻ����յ���Ϣ"), 0);


	while (1)
	{
		////4 �������շ�����
		//	//recv����
		//char buf[1500] = { 0 };
		//int recv_result = recv(ServerSocket, buf, 60, 0);  //����������������׵�ַ
		//if (0 == recv_result)
		//{
		//	printf("�ͻ�������");
		//}
		//else if (SOCKET_ERROR == recv_result)
		//{
		//	printf("����");
		//	int errorcode = WSAGetLastError();  //���ش�����
		//}
		//else
		//{	//��ȷ
		//	printf("Server send :  %s\n", buf);
		//}


		//send����
		char buf[1500] = { 0 };
		scanf("%s", buf);

		//��������������
		if ('e' == buf[0] && 'x' == buf[1] && 'i' == buf[2] && 't' == buf[3])
		{
			break;
		}
		int send_error = send(ServerSocket, buf, strlen(buf), 0);
		if (SOCKET_ERROR == send_error)
		{
			printf("����\n");
			int errorcode = WSAGetLastError();  //���ش�����
		}
	}

	closesocket(ServerSocket);
	WSACleanup();

	system("pause");
	return 0;
}