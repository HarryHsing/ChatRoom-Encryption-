#include "ClientLinkList.h"
#pragma warning(disable:4996)
pClient head = (pClient)malloc(sizeof(_Client)); //����һ��ͷ���
pgClient ghead = (pgClient)malloc(sizeof(_gClient));

												 /*
												 * function  ��ʼ������
												 * return    �޷���ֵ
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
* function  ��ȡͷ�ڵ�
* return    ����ͷ�ڵ�
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
* function  ���һ���ͻ���
* param     client��ʾһ���ͻ��˶���
* return    �޷���ֵ
*/
void AddClient(pClient client)
{
	client->next = head->next;  //���磺head->1->2,Ȼ�����һ��3��������
	head->next = client;        //3->1->2,head->3->1->2
}

/*
* function  ɾ��һ���ͻ���
* param     flag��ʶһ���ͻ��˶���
* return    ����true��ʾɾ���ɹ���false��ʾʧ��
*/
bool RemoveClient(UINT_PTR flag)
{
	//��ͷ������һ�����Ƚ�
	pClient pCur = head->next;//pCurָ���һ�����
	pClient pPre = head;      //pPreָ��head 
	while (pCur)
	{
		// head->1->2->3->4,Ҫɾ��2����ֱ����1->3
		if (pCur->flag == flag)
		{
			pPre->next = pCur->next;
			closesocket(pCur->sClient);  //�ر��׽���
			free(pCur);   //�ͷŸý��
			return true;
		}
		pPre = pCur;
		pCur = pCur->next;
	}
	return false;
}

/*
* function  ����ָ���ͻ���
* param     name��ָ���ͻ��˵��û���
* return    ����socket��ʾ���ҳɹ�������INVALID_SOCKET��ʾ�޴��û�
*/
SOCKET FindClient(char* name)
{
	//��ͷ������һ�����Ƚ�
	pClient pCur = head;
	while (pCur = pCur->next)
	{
		if (strcmp(pCur->userName, name) == 0)
			return pCur->sClient;
	}
	return INVALID_SOCKET;
}

/*
* function  ����SOCKET����ָ���ͻ���
* param     client��ָ���ͻ��˵��׽���
* return    ����һ��pClient��ʾ���ҳɹ�������NULL��ʾ�޴��û�
*/
pClient FindClient(SOCKET client)
{
	//��ͷ������һ�����Ƚ�
	pClient pCur = head;
	while (pCur = pCur->next)
	{
		if (pCur->sClient == client)
			return pCur;
	}
	return NULL;
}

/*
* function  ����ͻ���������
* param     client��ʾһ���ͻ��˶���
* return    ����������
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
* function  �������
* return    �޷���ֵ
*/
void ClearClient()
{
	pClient pCur = head->next;
	pClient pPre = head;
	while (pCur)
	{
		//head->1->2->3->4,��ɾ��1��head->2,Ȼ��free 1
		pClient p = pCur;
		pPre->next = p->next;
		free(p);
		pCur = pPre->next;
	}
}

/*
* function �������״̬���ر�һ������
* return ����ֵ
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

				char error[256] = { 0 };   //����������Ϣ������Ϣ����
				sprintf(error, "The %s was downline.\n", pclient->userName);
				fprintf(fp,"The %s was downline.\n", pclient->userName);
				send(FindClient(pclient->ChatName), error, sizeof(error), 0);
				closesocket(pclient->sClient);   //����򵥵��жϣ���������Ϣʧ�ܣ�����Ϊ�����ж�(��ԭ���ж���)���رո��׽���
				RemoveClient(pclient->flag);
				break;
			}
		}
	}
	fclose(fp);
}

/*
* function  ָ�����͸��ĸ��ͻ���
* param     FromName��������
* param     ToName,   ������
* param     data,     ���͵���Ϣ
*/
void SendData(char* FromName, char* ToName, char* data)
{
	SOCKET client = FindClient(ToName);   //�����Ƿ��д��û�
	char error[256] = { 0 };
	char buf[256] = { 0 };
	int ret = 0;
	if (client != INVALID_SOCKET && strlen(data) != 0)
	{
		sprintf(buf, "%s: %s", FromName, data);   //��ӷ�����Ϣ���û���
		ret = send(client, buf, sizeof(buf), 0);
	}
	else//���ʹ�����Ϣ������Ϣ����
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
	if (ret == SOCKET_ERROR)//����������Ϣ������Ϣ����
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
