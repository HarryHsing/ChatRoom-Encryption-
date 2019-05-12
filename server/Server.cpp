

#include <WinSock2.h>
#include <process.h>	
#include <stdlib.h>
#include "ClientLinkList.h"
#include<time.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

SOCKET g_ServerSocket = INVALID_SOCKET;      //服务端套接字
SOCKADDR_IN g_ClientAddr = { 0 };            //客户端地址
int g_iClientAddrLen = sizeof(g_ClientAddr);
pgClient pd = (pgClient)malloc(sizeof(_gClient));
typedef struct _Send
{
	char FromName[16];
	char ToName[16];
	char data[2048];
}Send, *pSend;



//发送数据线程
unsigned __stdcall ThreadSend(void* param)
{
	pSend psend = (pSend)param;  //转换为Send类型
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

	SendData(psend->FromName, psend->ToName, psend->data); //发送数据
	fclose(fp);
	return 0;
}


//接受数据
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
		//printf("接受: %s, %d\n", pclient->buf, sizeof(pclient->buf));


		//printf("1\n");

		if (ret == SOCKET_ERROR)
			return 1;

		//printf("2\n");


		if (pclient->buf[0] == '#' && pclient->buf[1] != '#') //#表示用户要指定另一个用户进行聊天
		{
			SOCKET socket = FindClient(&pclient->buf[1]);    //验证一下客户是否存在
			if (socket != INVALID_SOCKET)
			{
				pClient c = (pClient)malloc(sizeof(_Client));
				c = FindClient(socket);                        //只要改变ChatName,发送消息的时候就会自动发给指定的用户了
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
			//把发送人的用户名和接收消息的用户和消息赋值给结构体，然后当作参数传进发送消息进程中
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
				//把发送人的用户名和接收消息的用户和消息赋值给结构体，然后当作参数传进发送消息进程中

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

//开启接收消息线程
void StartRecv()
{
	/*pClient pclient = GetHeadNode();
	while (pclient = pclient->next)
		_beginthreadex(NULL, 0, ThreadRecv, pclient, 0, NULL);*/
}

//管理连接
unsigned __stdcall ThreadManager(void* param)
{
	while (1)
	{
		CheckConnection();  //检查连接状况
		Sleep(2000);        //2s检查一次
	}

	return 0;
}

//接受请求
unsigned __stdcall ThreadAccept(void* param)
{
	_beginthreadex(NULL, 0, ThreadManager, NULL, 0, NULL);
	Init(); //初始化一定不要再while里面做，否则head会一直为NULL！！！
	InitGroup();
	FILE *fp = NULL;
	fp = fopen("log_server.txt", "a");
	while (1)
	{
		//创建一个新的客户端对象
		pClient pclient = (pClient)malloc(sizeof(_Client));

		//如果有客户端申请连接就接受连接
		if ((pclient->sClient = accept(g_ServerSocket, (SOCKADDR*)&g_ClientAddr, &g_iClientAddrLen)) == INVALID_SOCKET)
		{
			fprintf(fp,"accept failed with error code: %d\n", WSAGetLastError());
			printf("accept failed with error code: %d\n", WSAGetLastError());
			closesocket(g_ServerSocket);
			WSACleanup();
			return -1;
		}
		recv(pclient->sClient, pclient->userName, sizeof(pclient->userName), 0); //接收用户名和指定聊天对象的用户名
		recv(pclient->sClient, pclient->ChatName, sizeof(pclient->ChatName), 0);

		memcpy(pclient->IP, inet_ntoa(g_ClientAddr.sin_addr), sizeof(pclient->IP)); //记录客户端IP
		pclient->flag = pclient->sClient; //不同的socke有不同UINT_PTR类型的数字来标识
		pclient->Port = htons(g_ClientAddr.sin_port);
		AddClient(pclient);  //把新的客户端加入链表中
		fprintf(fp,"Successfuuly got a connection from IP:%s ,Port: %d,UerName: %s , ChatName: %s\n",
			pclient->IP, pclient->Port, pclient->userName, pclient->ChatName);
		printf("Successfuuly got a connection from IP:%s ,Port: %d,UerName: %s , ChatName: %s\n",
			pclient->IP, pclient->Port, pclient->userName, pclient->ChatName);

		//if (CountCon() >= 2)                     //当至少两个用户都连接上服务器后才进行消息转发
		//	StartRecv();
		_beginthreadex(NULL, 0, ThreadRecv, pclient, 0, NULL);

		Sleep(2000);
	}
	fclose(fp);
	return 0;
}

//启动服务器
int StartServer()
{
	//存放套接字信息的结构
	WSADATA wsaData = { 0 };
	SOCKADDR_IN ServerAddr = { 0 };             //服务端地址
	USHORT uPort = 18000;                       //服务器监听端口

	//初始化套接字
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	//判断版本
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("wVersion was not 2.2\n");
		return -1;
	}
	//创建套接字
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_ServerSocket == INVALID_SOCKET)
	{
		printf("socket failed with error code: %d\n", WSAGetLastError());
		return -1;
	}

	//设置服务器地址
	ServerAddr.sin_family = AF_INET;//连接方式
	ServerAddr.sin_port = htons(uPort);//服务器监听端口
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//任何客户端都能连接这个服务器

	//绑定服务器
	if (SOCKET_ERROR == bind(g_ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)))
	{
		printf("bind failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		return -1;
	}
	//设置监听客户端连接数
	if (SOCKET_ERROR == listen(g_ServerSocket, 20000))
	{
		printf("listen failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		WSACleanup();
		return -1;
	}

	_beginthreadex(NULL, 0, ThreadAccept, NULL, 0, 0);
	for (int k = 0; k < 100; k++) //让主线程休眠，不让它关闭TCP连接.
		Sleep(10000000);

	//关闭套接字
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
	
	StartServer(); //启动服务器

	return 0;
}
