#include "ClientLinkList.h"
#pragma warning(disable:4996)
pClient head = (pClient)malloc(sizeof(_Client)); //创建一个头结点
pgClient ghead = (pgClient)malloc(sizeof(_gClient));

												 /*
												 * function  初始化链表
												 * return    无返回值
												 */
void Init()
{
	head->next = NULL;
}

void InitGroup()
{
	ghead->next = NULL;
}

/*
* function  获取头节点
* return    返回头节点
*/

void addG(_gClient *g) {
	g->next = ghead->next;
	ghead->next = g;
}

void joinG(_gClient *g, char * name) {
	strcpy(g->seq[(g->index) - 1 + 1], name);
}

pgClient checkGID(int ID) {
	pgClient g = ghead;
	while (g = g->next) {
		if (g->gID == ID) {
			return g;
		}
	}
	return NULL;
}


pClient GetHeadNode()
{
	return head;
}

/*
* function  添加一个客户端
* param     client表示一个客户端对象
* return    无返回值
*/
void AddClient(pClient client)
{
	client->next = head->next;  //比如：head->1->2,然后添加一个3进来后是
	head->next = client;        //3->1->2,head->3->1->2
}

/*
* function  删除一个客户端
* param     flag标识一个客户端对象
* return    返回true表示删除成功，false表示失败
*/
bool RemoveClient(UINT_PTR flag)
{
	//从头遍历，一个个比较
	pClient pCur = head->next;//pCur指向第一个结点
	pClient pPre = head;      //pPre指向head 
	while (pCur)
	{
		// head->1->2->3->4,要删除2，则直接让1->3
		if (pCur->flag == flag)
		{
			pPre->next = pCur->next;
			closesocket(pCur->sClient);  //关闭套接字
			free(pCur);   //释放该结点
			return true;
		}
		pPre = pCur;
		pCur = pCur->next;
	}
	return false;
}

/*
* function  查找指定客户端
* param     name是指定客户端的用户名
* return    返回socket表示查找成功，返回INVALID_SOCKET表示无此用户
*/
SOCKET FindClient(char* name)
{
	//从头遍历，一个个比较
	pClient pCur = head;
	while (pCur = pCur->next)
	{
		if (strcmp(pCur->userName, name) == 0)
			return pCur->sClient;
	}
	return INVALID_SOCKET;
}

/*
* function  根据SOCKET查找指定客户端
* param     client是指定客户端的套接字
* return    返回一个pClient表示查找成功，返回NULL表示无此用户
*/
pClient FindClient(SOCKET client)
{
	//从头遍历，一个个比较
	pClient pCur = head;
	while (pCur = pCur->next)
	{
		if (pCur->sClient == client)
			return pCur;
	}
	return NULL;
}

/*
* function  计算客户端连接数
* param     client表示一个客户端对象
* return    返回连接数
*/
int CountCon()
{
	int iCount = 0;
	pClient pCur = head;
	while (pCur = pCur->next)
		iCount++;
	return iCount;
}

/*
* function  清空链表
* return    无返回值
*/
void ClearClient()
{
	pClient pCur = head->next;
	pClient pPre = head;
	while (pCur)
	{
		//head->1->2->3->4,先删除1，head->2,然后free 1
		pClient p = pCur;
		pPre->next = p->next;
		free(p);
		pCur = pPre->next;
	}
}

/*
* function 检查连接状态并关闭一个连接
* return 返回值
*/
void CheckConnection()
{
	pClient pclient = GetHeadNode();
	FILE *fp = NULL;
	fp = fopen("log_linklist.txt", "a");
	while (pclient = pclient->next)
	{
		if (send(pclient->sClient, "", sizeof(""), 0) == SOCKET_ERROR)
		{
			if (pclient->sClient != 0)
			{
				fprintf(fp,"Disconnect from IP: %s,UserName: %s\n", pclient->IP, pclient->userName);
				printf("Disconnect from IP: %s,UserName: %s\n", pclient->IP, pclient->userName);

				char error[256] = { 0 };   //发送下线消息给发消息的人
				sprintf(error, "The %s was downline.\n", pclient->userName);
				fprintf(fp,"The %s was downline.\n", pclient->userName);
				send(FindClient(pclient->ChatName), error, sizeof(error), 0);
				closesocket(pclient->sClient);   //这里简单的判断：若发送消息失败，则认为连接中断(其原因有多种)，关闭该套接字
				RemoveClient(pclient->flag);
				break;
			}
		}
	}
	fclose(fp);
}

/*
* function  指定发送给哪个客户端
* param     FromName，发信人
* param     ToName,   收信人
* param     data,     发送的消息
*/
void SendData(char* FromName, char* ToName, char* data)
{
	SOCKET client = FindClient(ToName);   //查找是否有此用户
	char error[256] = { 0 };
	char buf[256] = { 0 };
	int ret = 0;
	if (client != INVALID_SOCKET && strlen(data) != 0)
	{
		sprintf(buf, "%s: %s", FromName, data);   //添加发送消息的用户名
		ret = send(client, buf, sizeof(buf), 0);
	}
	else//发送错误消息给发消息的人
	{
		if (client == INVALID_SOCKET)
			sprintf(error, "The %s was downline.\n", ToName);
		else
			sprintf(error, "Send to %s message not allow empty, Please try again!\n", ToName);

		int count = 0;
		while (error[count] != NULL) {
			count++;
		}
		count--;
		//send(FindClient(FromName), error, count, 0);
	}
	if (ret == SOCKET_ERROR)//发送下线消息给发消息的人
	{
		sprintf(error, "The %s was downline.\n", ToName);
		int count = 0;
		while (error[count] != NULL) {
			count++;
		}
		count--;
		send(FindClient(FromName), error, count, 0);
	}
}
