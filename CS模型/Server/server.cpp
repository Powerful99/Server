#include<iostream>
#include<WinSock2.h> //��2�汾�������
#pragma comment(lib,"ws2_32.lib") //windows 32λ�汾�ģ�û��64�汾�ģ�����64λ��Ҳ��32λ�汾��



int main()
{
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nRes = WSAStartup(sockVersion, &wsaData);//��һ���׽���

	if (0 != nRes)
	{
		switch (nRes)
		{
		case WSASYSNOTREADY: 
			printf("�������ԣ����߼�������");   break;
		case WSAVERNOTSUPPORTED: 
			printf("����������");  break;
		case WSAEINPROGRESS: 
			printf("����������");  break;
		case WSAEPROCLIM:  
			printf("��رղ���Ҫ���������ȷ�����㹻��������Դ"); 
			break;
		}
	}
	if (2 != HIBYTE(wsaData.wVersion) || 2 != LOBYTE(wsaData.wVersion))
	{
		printf("�����汾����");
		WSACleanup();//�رմ��׽���
		return 0;
	}

	//�׽��ֵĴ����͹ر�
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//��������  
		const char * sendData = "��ã�TCP�ͻ��ˣ����Ƿ�����\n";
		send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();

	return 0;
}

