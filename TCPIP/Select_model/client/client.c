#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)


int main()
{
	//1 打开网络库并校验版本号
	WORD wdVersion = MAKEWORD(2, 2);
	WSADATA wdSockInfo;

	int RESULT = WSAStartup(wdVersion, &wdSockInfo);
	if (0 != RESULT)
	{
		switch (RESULT)
		{
		case WSASYSNOTREADY:
			printf("重启电脑或检查路径是否正确");
			break;
		case WSAVERNOTSUPPORTED:
			printf("版本不一致");
			break;
		case WSAEINPROGRESS:
			printf("请重启软件");
			break;
		case WSAEPROCLIM:
			printf("请重启软件");
			break;
		case WSAEFAULT:
			printf("参数2错误");
			break;
		}
		return 0;
	}

	if (2 != HIBYTE(wdSockInfo.wVersion) || 2 != LOBYTE(wdSockInfo.wVersion))
	{
		WSACleanup();
		return 0;
	}


	//2 创建服务器socket
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
	{
		int socket_error = WSAGetLastError();
		WSACleanup();
		return 0;
	}


	//3 连接到服务器
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(12345);			//服务器的端口号
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //服务器的IP地址

	int connect_result = connect(ServerSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (connect_result == SOCKET_ERROR)
	{
		int connect_error = WSAGetLastError();
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}

	//send(ServerSocket, "客户端收到消息", sizeof("客户端收到消息"), 0);


	while (1)
	{
		////4 与服务端收发数据
		//	//recv函数
		//char buf[1500] = { 0 };
		//int recv_result = recv(ServerSocket, buf, 60, 0);  //数组名就是数组的首地址
		//if (0 == recv_result)
		//{
		//	printf("客户端下线");
		//}
		//else if (SOCKET_ERROR == recv_result)
		//{
		//	printf("错误");
		//	int errorcode = WSAGetLastError();  //返回错误码
		//}
		//else
		//{	//正确
		//	printf("Server send :  %s\n", buf);
		//}


		//send函数
		char buf[1500] = { 0 };
		scanf("%s", buf);

		//服务器正常下线
		if ('e' == buf[0] && 'x' == buf[1] && 'i' == buf[2] && 't' == buf[3])
		{
			break;
		}
		int send_error = send(ServerSocket, buf, strlen(buf), 0);
		if (SOCKET_ERROR == send_error)
		{
			printf("错误\n");
			int errorcode = WSAGetLastError();  //返回错误码
		}
	}

	closesocket(ServerSocket);
	WSACleanup();

	system("pause");
	return 0;
}