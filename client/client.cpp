#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <stdio.h>
#include <string.h>
#define MAX 1024

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#define RECV_OVER 1
#define RECV_YET 0
char userName[16] = { 0 };
char chatName[16] = { 0 };
int iStatus = RECV_YET;


//#include <bits/stdc++.h>
#include <iostream> 
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <deque>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <map>
#include <stack>
#include <set>
#include<time.h>
using namespace std;
//string k = "5DC67B8603235AAE";
string k = "123456789012345";
char temp2[1000] = { 0 };
int n;
struct node
{
	int c[80];
	int d[80];
	int cd[80];
	int k_n[80];
	int l[80];
	int r[80];
	node()
	{
		memset(c, 0, sizeof(c));
		memset(d, 0, sizeof(d));
		memset(k_n, 0, sizeof(k_n));
		memset(l, 0, sizeof(l));
		memset(r, 0, sizeof(r));
	}
};

char mikey[18] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };



//IP��ʼ�û���
int pc_ip[80] = { 0,58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7 };

//�û�ѡ��PC-1
int pc_1[60] = { 0,57,49,41,33,25,17,
9,1,58,50,42,34,26,
18,10,2,59,51,43,35,
27,19,11,3,60,52,44,
36,63,55,47,39,31,23,
15,7,62,54,46,38,30,
22,14,6,61,53,45,37,
29,21,13,5,28,20,12,4 };

//�û�ѡ��PC-2
int pc_2[60] = { 0,14,17,11,24,1,5,
3,28,15,6,21,10,
23,19,12,4,26,8,
16,7,27,20,13,2,
41,52,31,37,47,55,
30,40,51,45,33,48,
44,49,39,56,34,53,
46,42,50,36,29,32 };

//E����չ�任
int pc_e[80] = { 0,32,1,2,3,4,5,
4,5,6,7,8,9,
8,9,10,11,12,13,
12,13,14,15,16,17,
16,17,18,19,20,21,
20,21,22,23,24,25,
24,25,26,27,28,29,
28,29,30,31,32,1 };

//P���û���
int pc_p[80] = { 0,16,7,20,21,
29,12,28,17,
1,15,23,26,
5,18,31,10,
2,8,24,14,
32,27,3,9,
19,13,30,6,
22,11,4,25 };

//���ʼ�û���
int pc_ip_1[80] = { 0,40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25 };

//S��
int s_box[8][4][16] = {
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,

	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,10,5,14,9,

	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,

	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,

	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,

	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,

	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,

	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 };

///F����ʵ��E����չ
int f(int r[80], int kn[80])
{
	int x = 0;
	int e[80] = { 0 };
	int h = 0, l = 0, idx = 0;
	//E����չ�õ�e(Ri);
	for (int i = 1; i <= 48; i++)
		e[i] = r[pc_e[i]];
	//�������õ�k^(Ri);
	for (int i = 1; i <= 48; i++)
		r[i] = e[i] ^ kn[i];
	//S��ѹ����48λ��Ϊ8�飬ÿ��6λ���õ�S(k^(Ri));
	for (int i = 1; i <= 48; i += 6)
	{
		h = r[i] * 2 + r[i + 5] * 1;
		l = r[i + 1] * 8 + r[i + 2] * 4 + r[i + 3] * 2 + r[i + 4] * 1;
		e[++idx] = (s_box[x][h][l] >> 3) & 1;
		e[++idx] = (s_box[x][h][l] >> 2) & 1;
		e[++idx] = (s_box[x][h][l] >> 1) & 1;
		e[++idx] = s_box[x][h][l] & 1;
		x++;
	}
	//P���û�
	for (int i = 1; i <= 32; i++)
		r[i] = e[pc_p[i]];
	return 0;
}

char jiami(string m, int choice)
{
	/*
	ת������
	���ܣ���16��16����ת����16�������Ʋ��浽two����
	k��Կת���ɶ����Ʋ��浽two_k����
	*/
	int two[66] = { 0 }, num, idx = 0, two_k[66] = { 0 }, two_kup[66] = { 0 };
	for (int i = 0; i<16; i++) //��16��16����ת����16��������:���64λ
	{
		if (m[i] >= '0'&& m[i] <= '9')
			num = m[i] - '0';
		else num = m[i] - 'A' + 10;
		two[++idx] = (num >> 3) & 1;
		two[++idx] = (num >> 2) & 1;
		two[++idx] = (num >> 1) & 1;
		two[++idx] = num & 1;
	}
	idx = 0;
	for (int i = 0; i<16; i++) //k��Կת2����
	{
		if (k[i] >= '0'&&k[i] <= '9')
			num = k[i] - '0';
		else num = k[i] - 'A' + 10;
		two_k[++idx] = (num >> 3) & 1;
		two_k[++idx] = (num >> 2) & 1;
		two_k[++idx] = (num >> 1) & 1;
		two_k[++idx] = num & 1;
	}


	///��Կ����///
	/*
	��16������Կ
	���ܣ�����Կ����16�ּ������õ�����Կ
	*/
	///��4-5��pc_1�õ�k+(����Կ��pc_1�û�)
	for (int i = 1; i <= 56; i++)
		two_kup[i] = two_k[pc_1[i]];
	//���ҷ���
	node c_and_d[20];
	for (int i = 1; i <= 28; i++) //�õ�c[0];
		c_and_d[0].c[i] = two_kup[i];
	for (int i = 1, j = 29; j <= 56; i++, j++) //�õ�d[0];
		c_and_d[0].d[i] = two_kup[j];

	///16������ÿ������Կ
	for (int i = 1; i <= 16; i++) //�õ�c[1]-c[15],d[1]-d[15],CnDn;
	{
		///��4-6�İ���һλ������λ�ķֿ�

		//���Ϊ��1��2��9��16�֣���dѭ������1λ
		if (i == 1 || i == 2 || i == 9 || i == 16)
		{
			for (int j = 1; j <= 27; j++)
				c_and_d[i].c[j] = c_and_d[i - 1].c[j + 1];
			c_and_d[i].c[28] = c_and_d[i - 1].c[1];
			for (int j = 1; j <= 27; j++)
				c_and_d[i].d[j] = c_and_d[i - 1].d[j + 1];
			c_and_d[i].d[28] = c_and_d[i - 1].d[1];
		}
		//����������ִΣ���dѭ������2λ
		else
		{
			for (int j = 1; j <= 26; j++)
				c_and_d[i].c[j] = c_and_d[i - 1].c[j + 2];
			c_and_d[i].c[27] = c_and_d[i - 1].c[1];
			c_and_d[i].c[28] = c_and_d[i - 1].c[2];
			for (int j = 1; j <= 26; j++)
				c_and_d[i].d[j] = c_and_d[i - 1].d[j + 2];
			c_and_d[i].d[27] = c_and_d[i - 1].d[1];
			c_and_d[i].d[28] = c_and_d[i - 1].d[2];
		}
		for (int j = 1; j <= 28; j++)
			c_and_d[i].cd[j] = c_and_d[i].c[j];
		for (int j = 29, t = 1; t <= 28; j++, t++)
			c_and_d[i].cd[j] = c_and_d[i].d[t];
	}

	///PC_2�û����ӡ16������Կ
	node k_16[20];
	for (int i = 1; i <= 16; i++) //�õ�k1-kn;
		for (int j = 1; j <= 48; j++)
			k_16[i].k_n[j] = c_and_d[i].cd[pc_2[j]];


	///���ļ��� 71ҳͼ4-8���ļ��ܹ���///
	/*
	DES�㷨������
	���ܣ�ʵ��DES�㷨��16�ּ���
	*/
	int ip[80] = { 0 };
	//��ʼ�任ip
	for (int i = 1; i <= 64; i++)
		ip[i] = two[pc_ip[i]];

	//��ʼ���õ� l[0],r[0]
	node l_r[20];
	for (int i = 1; i <= 32; i++) //�õ�l;
		l_r[0].l[i] = ip[i];
	for (int i = 1, j = 33; j <= 64; i++, j++) //�õ�r;
		l_r[0].r[i] = ip[j];

	///choice=0ʱ�Ǽ��ܲ�����choice=1ʱ�ǽ��ܲ���
	//����16������
	if (choice == 0)
	{
		for (int i = 1; i <= 16; i++) //����L1-L16��R1-R16;
		{
			for (int j = 1; j <= 32; j++)
				//���ҵ���������ҵ���
				l_r[i].l[j] = l_r[i - 1].r[j];
			///F��������E����չ�����S��ѹ����P���û�
			f(l_r[i - 1].r, k_16[i].k_n);
			///���Һ���һ�����߽������հ�λ���õ�r[i]
			for (int j = 1; j <= 32; j++)
				l_r[i].r[j] = l_r[i - 1].l[j] ^ l_r[i - 1].r[j];
		}
	}
	else
	{
		for (int i = 1; i <= 16; i++) //����L1-L16��R1-R16;
		{
			for (int j = 1; j <= 32; j++)
				l_r[i].l[j] = l_r[i - 1].r[j];
			f(l_r[i - 1].r, k_16[16 - i + 1].k_n);
			//^��������
			for (int j = 1; j <= 32; j++)
				l_r[i].r[j] = l_r[i - 1].l[j] ^ l_r[i - 1].r[j];
		}
	}

	int R16L16[80] = { 0 }; //�õ�R16L16;
							//F�����������Һ���һ��
	for (int i = 1; i <= 32; i++)
		R16L16[i] = l_r[16].r[i];
	for (int i = 33, j = 1; j <= 32; j++, i++)
		R16L16[i] = l_r[16].l[j];

	int ans[80] = { 0 }; //�õ����ձ任;
						 //����ip���û�
	for (int i = 1; i <= 64; i++)
		ans[i] = R16L16[pc_ip_1[i]];

	///choice=0ʱ�Ǽ��ܲ�����choice=1ʱ�ǽ��ܲ���
	int tem_num;
	if (choice == 0)
	{

		//�ֽ�ת�����ַ��������
		for (int i = 1; i <= 64; i += 4)
		{
			tem_num = ans[i] * 8 + ans[i + 1] * 4 + ans[i + 2] * 2 + ans[i + 3] * 1;
			if (tem_num >= 10) {
				//printf("%c", (tem_num - 10) + 'A');
				temp2[n] = ((tem_num - 10) + 'A');
				//printf("\nXZH: %c I: %d\n", temp[i],i);
				//return (tem_num - 10) + 'A';
			}
			else {
				//printf("%c", tem_num + '0');
				temp2[n] = (tem_num + '0');
				//printf("\nXZH: %c I: %d\n", temp[i], i);
				//return tem_num + '0';
			}
			n++;
		}
	}
	else
	{
		int change[1000];
		int pos = 0;
		FILE *fp = NULL;
		fp = fopen("log_client.txt", "a");
		for (int i = 1; i <= 64; i += 4)
		{
			tem_num = ans[i] * 8 + ans[i + 1] * 4 + ans[i + 2] * 2 + ans[i + 3] * 1;
			change[++pos] = tem_num;
		}
		int jieans = 0;
		//fprintf(fp, "PlainText: \n");
		for (int i = 1; i <= 16; i += 2)
		{
			jieans += change[i];
			jieans *= 16;
			jieans += change[i + 1];
			//printf("%d %d\n",change[i],change[i+1]);
			fprintf(fp,"%c", jieans);
			printf("%c", jieans);
			jieans = 0;
		}
		//fprintf(fp, "\n");
	}
	return 0;
}


unsigned __stdcall ThreadRecv(void* param)
{
	char tensix[256] = { 0 };
	FILE *fp = NULL;
	fp = fopen("log_client", "a");
	while (1)
	{
		int ret = recv(*(SOCKET*)param, tensix, sizeof(tensix), 0);

		if (ret == SOCKET_ERROR)
		{
			Sleep(500);
			continue;
		}
		if (tensix[0] != NULL && tensix[1] != NULL && tensix[2] != NULL)
		{
			//printf("Client Recive: %s\n", tensix);
			fprintf(fp,"Client Recive Cypher: %s\n", tensix);
			int source_size = strlen(tensix);
			//printf("REcive: %s\n", tensix);
			
			char abc[256] = { 0 };
			char thisName[256] = { 0 };


			int countName = 0;
			int count = 0;

//1111111111111111
			int counter = 0;

			for (long i = 0; i < source_size; i++)
			{
				counter++;
				thisName[i] = tensix[i];
				if (tensix[i] == ':') {
					break;
				}
			}

			printf("%s", thisName);
			fprintf(fp,"%s", thisName);

			count = 0;
			counter = counter + 1;
			for(long i = counter; i < source_size; i++){
				abc[count] = tensix[i];
				count++;
			}
			//printf("ABC: %s\n", abc);

//1111111111111111111

			//while (chatName[countName] != NULL) {
			//	countName++;
			//}
			//
			//for (long i = 0; i <= countName + 1; i++)
			//{
			//	thisName[count] = tensix[i];
			//	count++;
			//}
			//if (tensix != NULL) {
			//	printf("%s", thisName);
			//}

			//count = 0;

			//for (long i = countName + 2; i < source_size + countName; i++)
			//{
			//	abc[count] = tensix[i];
			//	count++;
			//}


			int len = strlen(abc);
			int sum = 0;
			string m = "";
			fprintf(fp, "Client Recive Plain: %s\n", tensix);
			for (int i = 0; i<len; i++) //ÿ64λ����һ�Σ�
			{
				sum++;
				m += abc[i];
				if (sum == 16)
				{
					sum = 0;
					jiami(m, 1);
					m = "";
				}
			}
			fprintf(fp,"\n");
			printf("\n");

			iStatus = RECV_OVER;
		}
		else
			Sleep(100);


	}
	fclose(fp);
	return 0;
}

//��������
unsigned __stdcall ThreadSend(void* param)
{
	char buf[256] = { 0 };
	int ret = 0;
	FILE *fp = NULL;
	fp = fopen("log_client.txt", "a");
	while (1)
	{
		int c = getch();
		char *code;

		char temp[256] = { 0 };
		char tempo[256] = { 0 };
		
		if (c == 27)   //ESC ASCII��27
		{
			char choose;
			fprintf(fp,"Please input 1 for 1vs1, 2 for join group, 3 for create group, 4 for change KEY: \n");
			printf("Please input 1 for 1vs1, 2 for join group, 3 for create group: \n");
			scanf("%c",&choose);
			getchar();
			if (choose == '1') {
				memset(buf, 0, sizeof(buf));
				fprintf(fp,"\nPlease input the chat name:");
				printf("Please input the chat name:");
				gets_s(buf);
				char b[17] = { 0 };
				sprintf(b, "#%s", buf);
				
				ret = send(*(SOCKET*)param, b, sizeof(b), 0);
				if (ret == SOCKET_ERROR)
					return 1;
				continue;
			}
			else if (choose == '2') {

				printf("Please input group ID: \n");
				fprintf(fp,"Please input group ID: \n");
				gets_s(tempo);
				char d[17] = { 0 };
				sprintf(d, "*%s", tempo);

				ret = send(*(SOCKET*)param, d, sizeof(d), 0);
				if (ret == SOCKET_ERROR)
					return 1;
				continue;
			}
			else if (choose == '3') {
				fprintf(fp,"Please create group ID: \n");
				printf("Please create group ID: \n");
				gets_s(tempo);

				char d[17] = { 0 };
				sprintf(d, "+%s", tempo);
				ret = send(*(SOCKET*)param, d, sizeof(d), 0);
				if (ret == SOCKET_ERROR)
					return 1;
				continue;
			}
			else if (choose == '4'); {
				printf("Please input new key(Must in 16 bits): \n");
				gets_s(tempo);

				k = tempo;
				continue;
			}
		}
		if (c == 72 || c == 0 || c == 68)//Ϊ����ʾ���ۣ���һ���޻��ԵĶ�ȡ�ַ�����
			continue;                   //getch����ֵ���Ǿ���ʵ��ó�����Ƿ����⼸��ֵ����getch�ͻ��Զ�������������Ҳ������
		printf("%s: ", userName);
		fprintf(fp,"%s: ", userName);
		gets_s(buf);
		fprintf(fp, "Plaintext is ");
		fprintf(fp, buf);
		fprintf(fp, "\n");
		

		//printf("Send_buf:%s\n", buf);//xzh
		

		//int count = 0;
		//while (buf[count] != NULL) {
		//	count++;
		//}
		//count--;

		///*code= encode(buf);*/
		//int source_size = strlen(buf);
		//for (long i = 0; i <= count; i++)
		//{
		//	temp[i] = cipher(buf[i]);//��source_char[i]���м���Ȼ��ŵ�cipher_char[i]��
		//}

		//printf("����Ϊ:");
		//fprintf(fp, "cyphertext is ");

		
		//fprintf(fp, "\n");
		int len = strlen(buf), idx = 0;
		char tensix[2000], tem[1000], now = 0;
		for (int i = 0; i<len; i++) //�ı�ת16����
		{
			while (buf[i])
			{
				if (buf[i] % 16 >= 10) tem[++idx] = ((buf[i] % 16) - 10) + 'A';
				else tem[++idx] = (buf[i] % 16) + '0';
				buf[i] /= 16;
			}
			//�������
			for (int j = idx; j >= 1; j--)
				tensix[++now] = tem[j];
			idx = 0;
		}
		while (now % 16)//����16λ��0��
		{
			++now;
			tensix[now] = '0';
		}
		int sum = 0;
		string m = "";
		
		for (int i = 1; i <= now; i++) //ÿ64λ����һ�Σ�
		{
			sum++;
			m += tensix[i];
			//cout<<m<<endl;
			if (sum == 16)
			{
				sum = 0;
				jiami(m, 0);
				m = "";
			}
		}
		n = 0;

		char b[512] = { 0 };
		//sprintf(b, "%s", temp2);
		fprintf(fp, temp2);

		//printf("\nXZH: %s\n", b);
		printf("\n");
		fprintf(fp,"\n");


		//printf("Send_code:%s\n", temp2);//xzh
		
		if (temp2[0] != NULL && temp2[1] != NULL) {
			ret = send(*(SOCKET*)param, temp2, sizeof(temp2), 0);
			memset(temp2, 0, sizeof(temp2));
			if (ret == SOCKET_ERROR)
				return 1;
		}

	}
	fclose(fp);
	return 0;
}



//���ӷ�����
int ConnectServer()
{
	WSADATA wsaData = { 0 };//����׽�����Ϣ
	SOCKET ClientSocket = INVALID_SOCKET;//�ͻ����׽���
	SOCKADDR_IN ServerAddr = { 0 };//����˵�ַ
	USHORT uPort = 18000;//����˶˿�
						 //��ʼ���׽���
	FILE *fp = NULL;
	fp = fopen("log_client.txt", "a");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	//�ж��׽��ְ汾
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("wVersion was not 2.2\n");
		return -1;
	}
	//�����׽���
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("socket failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	//���������IP
	printf("***************************************************\n");
	fprintf(fp,"\nPlease input server IP:");
	printf("Please input server IP:");
	char IP[32] = { 0 };
	gets_s(IP);
	fprintf(fp,IP);
	//���÷�������ַ
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(uPort);//�������˿�
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(IP);//��������ַ
	fprintf(fp,"connecting......\n");
	printf("connecting......\n");
	//���ӷ�����
	if (SOCKET_ERROR == connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)))
	{
		printf("connect failed with error code: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return -1;
	}

	time_t t;
	time(&t);
	fprintf(fp,"%s\nConnecting server successfully IP:%s Port:%d\n",
		ctime(&t),IP, htons(ServerAddr.sin_port));
	printf("%s\nServer successfully Connected and IP:%s Port:%d\n",
		ctime(&t),IP, htons(ServerAddr.sin_port));
	fprintf(fp,"Please input your UserName: ");
	printf("UserName: ");


	gets_s(userName);
	fprintf(fp, userName);
	send(ClientSocket, userName, sizeof(userName), 0);
	printf("ChatName: ");
	fprintf(fp,"Please input the ChatName: ");
	gets_s(chatName);
	printf("***************************************************\n");
	fprintf(fp, chatName);
	send(ClientSocket, chatName, sizeof(chatName), 0);
	fprintf(fp,"\n\n");
	printf("\n\n");

	_beginthreadex(NULL, 0, ThreadRecv, &ClientSocket, 0, NULL); //�������պͷ�����Ϣ�߳�
	_beginthreadex(NULL, 0, ThreadSend, &ClientSocket, 0, NULL);
	for (int k = 0; k < 1000; k++)
		Sleep(10000000);
	closesocket(ClientSocket);
	WSACleanup();
	fclose(fp);
	return 0;
}

int main()
{
	ConnectServer(); //���ӷ�����
	return 0;
}
