

#include <WinSock2.h>
#include <process.h>	
#include <stdlib.h>
#include "ClientLinkList.h"
#include<time.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

SOCKET g_ServerSocket = INVALID_SOCKET;      //������׽���
SOCKADDR_IN g_ClientAddr = { 0 };            //�ͻ��˵�ַ
int g_iClientAddrLen = sizeof(g_ClientAddr);
pgClient pd = (pgClient)malloc(sizeof(_gClient));
typedef struct _Send
{
	char FromName[16];
	char ToName[16];
	char data[2048];
}Send, *pSend;



//���������߳�
unsigned __stdcall ThreadSend(void* param)
{
	pSend psend = (pSend)param;  //ת��ΪSend����
	FILE *fp = NULL;
	fp = fopen("log_server.txt", "a");

	//printf("Send_data:%s, length: %d, From: %s, TO: %s\n", psend->data,strlen(psend->data), psend->FromName, psend->ToName);//xzh
	fprintf(fp,"Send_data:%s, length: %d, From: %s, TO: %s\n", psend->data, strlen(psend->data), psend->FromName, psend->ToName);//xzh
	//if (psend->ToName == psend->FromName) {
	//	return 0;
	//}
	if (strcmp(psend->FromName, psend->ToName) == 0 || strlen(psend->data) == 0) {
		return 0;
	}

	SendData(psend->FromName, psend->ToName, psend->data); //��������
	fclose(fp);
	return 0;
}


//��������
unsigned __stdcall ThreadRecv(void* param)
{
	int ret = 0;
	int type = 0;
	

	while (1)
	{

		pClient pclient = (pClient)param;
		//memset(pclient->buf, 0, sizeof(pclient->buf));


		if (!pclient)
			return 1;
		ret = recv(pclient->sClient, pclient->buf, sizeof(pclient->buf), 0);
		if (pclient->buf[0] == NULL) {
			continue;
		}
		FILE *fp = NULL;
		fp = fopen("log_server.txt", "a");
		printf("*********************\n");
		fprintf(fp,"*********************\n");
		printf("User: %s\n", pclient->userName);
		fprintf(fp, "User: %s\n", pclient->userName);
		printf("Data: %s\n", pclient->buf);
		fprintf(fp,"Data: %s\n", pclient->buf);
		printf("To: %s\n", pclient->ChatName);
		fprintf(fp,"To: %s\n", pclient->ChatName);
		printf("Type: %d\n", type);
		fprintf(fp,"Type: %d\n", type);
		printf("*********************\n");
		fprintf(fp,"*********************\n");
		fclose(fp);
		//printf("TYPE: %d\n", type);
		//printf("����: %s, %d\n", pclient->buf, sizeof(pclient->buf));


		//printf("1\n");

		if (ret == SOCKET_ERROR)
			return 1;

		//printf("2\n");


		if (pclient->buf[0] == '#' && pclient->buf[1] != '#') //#��ʾ�û�Ҫָ����һ���û���������
		{
			SOCKET socket = FindClient(&pclient->buf[1]);    //��֤һ�¿ͻ��Ƿ����
			if (socket != INVALID_SOCKET)
			{
				pClient c = (pClient)malloc(sizeof(_Client));
				c = FindClient(socket);                        //ֻҪ�ı�ChatName,������Ϣ��ʱ��ͻ��Զ�����ָ�����û���
				memset(pclient->ChatName, 0, sizeof(pclient->ChatName));
				memcpy(pclient->ChatName, c->userName, sizeof(pclient->ChatName));
			}
			else
				send(pclient->sClient, "The user have not online or not exits.", 64, 0);
			type = 0;

			continue;
		}
		else if (pclient->buf[0] == '+') {
			pgClient d = (pgClient)malloc(sizeof(_gClient));
			int num = atoi(&pclient->buf[1]);
			d->gID = num;
			d->index = 1;
			//printf("\ncheck1: %d\n", d->index);
			strcpy(d->seq[(d->index) - 1], pclient->userName);
			addG(d);
			type = 1;
			continue;

		}
		else if (pclient->buf[0] == '*') {
			int num = atoi(&pclient->buf[1]);

			pd = checkGID(num);
			if (pd != NULL) {
				//printf("\ncheck2: %d\n", pd->index);
				pd->index = (pd->index) + 1;
				strcpy(pd->seq[(pd->index) - 1], pclient->userName);
				type = 1;
				continue;
			}
			else {
				//printf("Cannot be null\n");
				continue;
			}

		}

		//printf("3\n");

		if (type == 0) {

			fp = fopen("log_server.txt", "a");
			printf("1v1!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			fprintf(fp,"1v1\n");
			fclose(fp);
			pSend psend = (pSend)malloc(sizeof(_Send));
			//�ѷ����˵��û����ͽ�����Ϣ���û�����Ϣ��ֵ���ṹ�壬Ȼ������������������Ϣ������
			memcpy(psend->FromName, pclient->userName, sizeof(psend->FromName));
			memcpy(psend->ToName, pclient->ChatName, sizeof(psend->ToName));
			memcpy(psend->data, pclient->buf, sizeof(psend->data));
			/*printf("NB0: %s\n", psend->FromName);
			printf("NB0: %s\n", psend->ToName);
			printf("NB0: %s\n", psend->data);*/
			printf("\n");
			_beginthreadex(NULL, 0, ThreadSend, psend, 0, NULL);
			Sleep(200);
		}
		else if(type == 1){
			fp = fopen("log_server.txt", "a");
			printf("MvM!!!!!!!!!!!!!!!!!!!!\n");
			fprintf(fp, "MvM\n");
			fclose(fp);
			for (int i = 0; i < (pd->index); i++) {
				//printf("jinlai?\n");
				pSend psend = (pSend)malloc(sizeof(_Send));
				//�ѷ����˵��û����ͽ�����Ϣ���û�����Ϣ��ֵ���ṹ�壬Ȼ������������������Ϣ������

				memcpy(psend->FromName, pclient->userName, sizeof(psend->FromName));
				memcpy(psend->ToName, pd->seq[i], sizeof(psend->ToName));

				if (strcmp(psend->FromName, psend->ToName) == 0) {
					continue;
				}
				//printf("\ncheck3: %s\n", pclient->buf);
				memcpy(psend->data, pclient->buf, sizeof(psend->data));

				if (psend->data == NULL) {
					continue;
				}

				//printf("NB1: %s\n", psend->FromName);
				//printf("NB1: %s\n", psend->ToName);
				//printf("NB1: %s\n", psend->data);
				//printf("\n");


				_beginthreadex(NULL, 0, ThreadSend, psend, 0, NULL);
				Sleep(200);
			}
		}
	
		//printf("4\n");

	}

	return 0;
}

//����������Ϣ�߳�
void StartRecv()
{
	/*pClient pclient = GetHeadNode();
	while (pclient = pclient->next)
		_beginthreadex(NULL, 0, ThreadRecv, pclient, 0, NULL);*/
}

//��������
unsigned __stdcall ThreadManager(void* param)
{
	while (1)
	{
		CheckConnection();  //�������״��
		Sleep(2000);        //2s���һ��
	}

	return 0;
}

//��������
unsigned __stdcall ThreadAccept(void* param)
{
	_beginthreadex(NULL, 0, ThreadManager, NULL, 0, NULL);
	Init(); //��ʼ��һ����Ҫ��while������������head��һֱΪNULL������
	InitGroup();
	FILE *fp = NULL;
	fp = fopen("log_server.txt", "a");
	while (1)
	{
		//����һ���µĿͻ��˶���
		pClient pclient = (pClient)malloc(sizeof(_Client));

		//����пͻ����������Ӿͽ�������
		if ((pclient->sClient = accept(g_ServerSocket, (SOCKADDR*)&g_ClientAddr, &g_iClientAddrLen)) == INVALID_SOCKET)
		{
			fprintf(fp,"accept failed with error code: %d\n", WSAGetLastError());
			printf("accept failed with error code: %d\n", WSAGetLastError());
			closesocket(g_ServerSocket);
			WSACleanup();
			return -1;
		}
		recv(pclient->sClient, pclient->userName, sizeof(pclient->userName), 0); //�����û�����ָ�����������û���
		recv(pclient->sClient, pclient->ChatName, sizeof(pclient->ChatName), 0);

		memcpy(pclient->IP, inet_ntoa(g_ClientAddr.sin_addr), sizeof(pclient->IP)); //��¼�ͻ���IP
		pclient->flag = pclient->sClient; //��ͬ��socke�в�ͬUINT_PTR���͵���������ʶ
		pclient->Port = htons(g_ClientAddr.sin_port);
		AddClient(pclient);  //���µĿͻ��˼���������
		fprintf(fp,"Successfuuly got a connection from IP:%s ,Port: %d,UerName: %s , ChatName: %s\n",
			pclient->IP, pclient->Port, pclient->userName, pclient->ChatName);
		printf("Successfuuly got a connection from IP:%s ,Port: %d,UerName: %s , ChatName: %s\n",
			pclient->IP, pclient->Port, pclient->userName, pclient->ChatName);

		//if (CountCon() >= 2)                     //�����������û��������Ϸ�������Ž�����Ϣת��
		//	StartRecv();
		_beginthreadex(NULL, 0, ThreadRecv, pclient, 0, NULL);

		Sleep(2000);
	}
	fclose(fp);
	return 0;
}

//����������
int StartServer()
{
	//����׽�����Ϣ�Ľṹ
	WSADATA wsaData = { 0 };
	SOCKADDR_IN ServerAddr = { 0 };             //����˵�ַ
	USHORT uPort = 18000;                       //�����������˿�

	//��ʼ���׽���
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	//�жϰ汾
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("wVersion was not 2.2\n");
		return -1;
	}
	//�����׽���
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_ServerSocket == INVALID_SOCKET)
	{
		printf("socket failed with error code: %d\n", WSAGetLastError());
		return -1;
	}

	//���÷�������ַ
	ServerAddr.sin_family = AF_INET;//���ӷ�ʽ
	ServerAddr.sin_port = htons(uPort);//�����������˿�
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//�κοͻ��˶����������������

	//�󶨷�����
	if (SOCKET_ERROR == bind(g_ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)))
	{
		printf("bind failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		return -1;
	}
	//���ü����ͻ���������
	if (SOCKET_ERROR == listen(g_ServerSocket, 20000))
	{
		printf("listen failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		WSACleanup();
		return -1;
	}

	_beginthreadex(NULL, 0, ThreadAccept, NULL, 0, 0);
	for (int k = 0; k < 100; k++) //�����߳����ߣ��������ر�TCP����.
		Sleep(10000000);

	//�ر��׽���
	ClearClient();
	closesocket(g_ServerSocket);
	WSACleanup();
	return 0;
}

int main()
{
	time_t t;
	time(&t);
	FILE *fp = NULL;
	fp = fopen("log_server.txt", "a");
	printf("%sHarry & Kelvin Server start \n",ctime(&t));
	fprintf(fp,"%sHarry & Kelvin Server start \n", ctime(&t));
	fclose(fp);
	
	StartServer(); //����������

	return 0;
}
