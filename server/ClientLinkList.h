#ifndef _CLIENT_LINK_LIST_H_
#define _CLIENT_LINK_LIST_H_

#include <WinSock2.h>

#include <stdio.h>

//客户端信息结构体
typedef struct _Client
{
	SOCKET sClient;         //客户端套接字
	char buf[2048];          //数据缓冲区
	char userName[16];      //客户端用户名
	char IP[20];            //客户端IP
	unsigned short Port;    //客户端端口
	UINT_PTR flag;          //标记客户端，用来区分不同的客户端
	char ChatName[16];      //指定要和哪个客户端聊天
	_Client* next;          //指向下一个结点
}Client, *pClient;

//客户端qunliao信息结构体
typedef struct _gClient
{
	int gID;
	int index;
	char seq[20][20];
	_gClient* next;
}gClient, *pgClient;

/*
* function  初始化链表
* return    无返回值
*/
void Init();

void InitGroup();

void addG(_gClient *g);

pgClient checkGID(int ID);

/*
* function  获取头节点
* return    返回头节点
*/
pClient GetHeadNode();

/*
* function  添加一个客户端
* param     client表示一个客户端对象
* return    无返回值
*/
void AddClient(pClient client);

/*
* function  删除一个客户端
* param     flag标识一个客户端对象
* return    返回true表示删除成功，false表示失败
*/
bool RemoveClient(UINT_PTR flag);

/*
* function  根据name查找指定客户端
* param     name是指定客户端的用户名
* return    返回一个client表示查找成功，返回INVALID_SOCKET表示无此用户
*/
SOCKET FindClient(char* name);

/*
* function  根据SOCKET查找指定客户端
* param     client是指定客户端的套接字
* return    返回一个pClient表示查找成功，返回NULL表示无此用户
*/
pClient FindClient(SOCKET client);

/*
* function  计算客户端连接数
* param     client表示一个客户端对象
* return    返回连接数
*/
int CountCon();

/*
* function  清空链表
* return    无返回值
*/
void ClearClient();

/*
* function  检查连接状态并关闭一个连接
* return 返回值
*/
void CheckConnection();

/*
* function  指定发送给哪个客户端
* param     FromName，发信人
* param     ToName,   收信人
* param     data,     发送的消息
*/
void SendData(char* FromName, char* ToName, char* data);


#endif //_CLIENT_LINK_LIST_H_
