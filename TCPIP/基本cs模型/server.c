#include <WinSock2.h>   //转到文档可以查看version  //version（2,2）  WinSock2.2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

int main()
{

//1  打开网络库

	WORD wdVersion = MAKEWORD(2, 2);		//int a = *((char*)(&wdVersion));   //取wdVersion的第一个字节  因为sizeof（char）= 1（Bite）
											//int b = *((char*)(&wdVersion)+1);   //取第二个字节
											//高字节装副版本号  低字节装主版本号
	WSADATA wdSockInfo;

	int nResult = WSAStartup(wdVersion, &wdSockInfo);		//传结构体的地址
	if (nResult)
	{
		switch (nResult)
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

//2 校验版本号
	if (2 != HIBYTE(wdSockInfo.wVersion) || 2 != LOBYTE(wdSockInfo.wVersion))
	{//版本出错   //有一个不对  版本就不对
		WSACleanup();  //关闭网络库
		return 0;
	}
	

//3 创建SOCKET
	SOCKET socketServer = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//检测
	if (INVALID_SOCKET == socketServer)
	{
		//socket()函数出错
		int socket_error = WSAGetLastError();  //用WSAGetLastError返回错误码 用socket_error接收

		WSACleanup();  //关闭网络库  不用关闭socket
		return 0;
	}



//4 绑定端口和地址  bind函数
	struct sockaddr_in s_in;   //c 声明结构体对象要加关键词struct  而c++ 定义class时直接类的名字+对象  即可
	s_in.sin_family = AF_INET; //地址类型
	s_in.sin_port = htons(12345);   //端口号  0->2^16-1 (0-65535)     
							   //用htons（）类型转换 。 其实本身int -> u_int 系统可以隐式类型转换，但是为了规范  用htons转换为端口的定义的类型
	s_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1 本地回环地址
													//或者cmd中 用ipconfig找本机的ip地址
		
	int bind_result = bind(socketServer,(const struct sockaddr *)&s_in,sizeof(s_in));  //bind()函数
	if (bind_result == SOCKET_ERROR)
	{
		//bind函数出错
		int bind_error = WSAGetLastError();
		//释放
		closesocket(socketServer);
		//清理网络库
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


//6 accept函数
	//用accept函数接住client传来的socket  并且用socket通信
	struct sockaddr_in clientaddr;	//预先定义客户端的addr 用来接受客户端的IP地址和端口号等信息
	int lenofclientaddr = sizeof(clientaddr);		//用来接受client传来的addr的大小
	SOCKET socketClient = accept(socketServer, (struct sockaddr*)&clientaddr, &lenofclientaddr);
	if (socketClient == INVALID_SOCKET)
	{
		printf("客户端连接失败\n");
		int accept_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}
	printf("客户端连接成功\n");

	send(socketClient, "服务器收到连接", sizeof("服务器收到连接"), 0);

	while (1)
	{
	//8  recv函数
		char buf[1500] = { 0 };
		int recv_result = recv(socketClient,buf,60,0);  //数组名就是数组的首地址
		if (0 == recv_result)
		{
			printf("客户端下线");
		}
		else if (SOCKET_ERROR == recv_result)
		{
			printf("错误");
			int errorcode = WSAGetLastError();  //返回错误码
		}
		else
		{	//正确
			printf("Client send : %s\n", buf);
		}


	//7 send函数
		scanf("%s",buf);
		int send_error = send(socketClient, buf, strlen(buf), 0);
		if (SOCKET_ERROR == send_error)
		{
			printf("错误");
			int errorcode = WSAGetLastError();  //返回错误码
		}
	}



	
	closesocket(socketClient);
	closesocket(socketServer);  //关闭socket
	WSACleanup();    //关闭网络库  （先后顺序）

	system("pause");
	return 0;
}
