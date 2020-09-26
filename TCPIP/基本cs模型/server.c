#include <WinSock2.h>   //ת���ĵ����Բ鿴version  //version��2,2��  WinSock2.2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

int main()
{

//1  �������

	WORD wdVersion = MAKEWORD(2, 2);		//int a = *((char*)(&wdVersion));   //ȡwdVersion�ĵ�һ���ֽ�  ��Ϊsizeof��char��= 1��Bite��
											//int b = *((char*)(&wdVersion)+1);   //ȡ�ڶ����ֽ�
											//���ֽ�װ���汾��  ���ֽ�װ���汾��
	WSADATA wdSockInfo;

	int nResult = WSAStartup(wdVersion, &wdSockInfo);		//���ṹ��ĵ�ַ
	if (nResult)
	{
		switch (nResult)
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

//2 У��汾��
	if (2 != HIBYTE(wdSockInfo.wVersion) || 2 != LOBYTE(wdSockInfo.wVersion))
	{//�汾����   //��һ������  �汾�Ͳ���
		WSACleanup();  //�ر������
		return 0;
	}
	

//3 ����SOCKET
	SOCKET socketServer = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//���
	if (INVALID_SOCKET == socketServer)
	{
		//socket()��������
		int socket_error = WSAGetLastError();  //��WSAGetLastError���ش����� ��socket_error����

		WSACleanup();  //�ر������  ���ùر�socket
		return 0;
	}



//4 �󶨶˿ں͵�ַ  bind����
	struct sockaddr_in s_in;   //c �����ṹ�����Ҫ�ӹؼ���struct  ��c++ ����classʱֱ���������+����  ����
	s_in.sin_family = AF_INET; //��ַ����
	s_in.sin_port = htons(12345);   //�˿ں�  0->2^16-1 (0-65535)     
							   //��htons��������ת�� �� ��ʵ����int -> u_int ϵͳ������ʽ����ת��������Ϊ�˹淶  ��htonsת��Ϊ�˿ڵĶ��������
	s_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1 ���ػػ���ַ
													//����cmd�� ��ipconfig�ұ�����ip��ַ
		
	int bind_result = bind(socketServer,(const struct sockaddr *)&s_in,sizeof(s_in));  //bind()����
	if (bind_result == SOCKET_ERROR)
	{
		//bind��������
		int bind_error = WSAGetLastError();
		//�ͷ�
		closesocket(socketServer);
		//���������
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


//6 accept����
	//��accept������סclient������socket  ������socketͨ��
	struct sockaddr_in clientaddr;	//Ԥ�ȶ���ͻ��˵�addr �������ܿͻ��˵�IP��ַ�Ͷ˿ںŵ���Ϣ
	int lenofclientaddr = sizeof(clientaddr);		//��������client������addr�Ĵ�С
	SOCKET socketClient = accept(socketServer, (struct sockaddr*)&clientaddr, &lenofclientaddr);
	if (socketClient == INVALID_SOCKET)
	{
		printf("�ͻ�������ʧ��\n");
		int accept_error = WSAGetLastError();
		closesocket(socketServer);
		WSACleanup();
		return 0;
	}
	printf("�ͻ������ӳɹ�\n");

	send(socketClient, "�������յ�����", sizeof("�������յ�����"), 0);

	while (1)
	{
	//8  recv����
		char buf[1500] = { 0 };
		int recv_result = recv(socketClient,buf,60,0);  //����������������׵�ַ
		if (0 == recv_result)
		{
			printf("�ͻ�������");
		}
		else if (SOCKET_ERROR == recv_result)
		{
			printf("����");
			int errorcode = WSAGetLastError();  //���ش�����
		}
		else
		{	//��ȷ
			printf("Client send : %s\n", buf);
		}


	//7 send����
		scanf("%s",buf);
		int send_error = send(socketClient, buf, strlen(buf), 0);
		if (SOCKET_ERROR == send_error)
		{
			printf("����");
			int errorcode = WSAGetLastError();  //���ش�����
		}
	}



	
	closesocket(socketClient);
	closesocket(socketServer);  //�ر�socket
	WSACleanup();    //�ر������  ���Ⱥ�˳��

	system("pause");
	return 0;
}
