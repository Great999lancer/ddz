#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>	//atoi�����õ�
#include <arpa/inet.h>	//�ֽ���ת��
#include <sys/socket.h>	//socket
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <fstream>
#include <time.h>

#define ERR_EXIT(m) \
do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while (0);\
void creat_daemon(void);

#define client_num 30
#define PORT 6666
#define BACKLOG 2
#define reuseaddr 1
#define Max_pot 10
#define Max_Len 4096

#define CLIENT_LOGIN		0x01	//�����¼
#define CLIENT_INFORMATION	0x02	//���������Ϣ
#define CLIENT_PAIJU		0x03	//�����ƾ�
#define CLIENT_PAIZHUO		0x04	//�����������
#define CLIENT_START		0x05	//��ʼ��Ϣ
#define CLIENT_JIAODIZHU	0x06	//�Ƿ�е���
#define CLIENT_CHUPAI		0x07	//����
#define CLIENT_BUCHU		0x08	//������
#define CLIENT_LOGOUT		0x09	//�ͻ����˳�
#define CLIENT_PAISHU		0x0a	//����ǰ����ÿ�����ʣ������
#define CLIENT_XINTIAO		0x0b	//�����ź�
#define	CLIENT_PAIZHUOINFOR	0x0c	//����ǰ������Ϣ

#define SERVER_LOGINSS		0x51	//��ͻ��˷��͵� ȷ�ϵ�¼�ɹ�
#define SERVER_LOGINDF		0x52	//��ͻ��˷��͵ĵ�¼ʧ��
#define SERVER_USERINFOR	0x53	//�����û���Ϣ
#define SERVER_PAIJU		0x54	//���ص�ǰ�ƾ���Ϣ
#define SERVER_PAIZHUO		0x55	//����ĳ������
#define SERVER_NOPAIZHUO	0x56	//�ܾ�����ĳ������
#define	SERVER_PLAYING		0x57	//���ص�ǰ��Ϸ����������Ϣ
#define SERVER_FAPAI		0x58	//����ҷ��������17����
#define SERVER_DIZHU		0x59	//ѯ���Ƿ�е���
#define SERVER_WHODIZHU		0x5a	//�����������˭�ǵ���
#define SERVER_CHUPAI		0x5b	//ѯ�ʳ���
#define SERVER_SHENGFU		0x5c	//ʤ����Ϣ
#define SERVER_TIREN		0x5d	//���������߳�ȥ
#define SERVER_PAI			0x5e	//ÿ�����ʣ�����
#define SERVER_DIAOXIAN		0x5f	//ĳ���û�����
#define	SERVER_TAOPAO		0x60	//ĳ���û�����
#define SERVER_CHUPAI__		0x70	//��ҳ���
#define SERVER_DIZHUPAI		0x80	//��������

const unsigned char data_0x00=0x00;
const unsigned char data_0x01=0x01;
const unsigned char data_0x02=0x02;
const unsigned char data_0x03=0x03;

using namespace std;

struct node
{
	int sc,id;
	char name[16];
};
    
int ss,sc,pot,id;
int err;
int reua=reuseaddr;
string cip;
int cpot;
fd_set rfds;
pid_t pid;
int s_s[Max_pot];
node pepo_sc[client_num];
char ccc[Max_Len],ccc2[Max_Len];
char cc[4];
char buf[Max_Len],buff[Max_Len];
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
struct head
{
	unsigned char flag;			//��ͷ��ʶ
	unsigned short len;			//���ĳ���
//	unsigned char cc;			//��ͷ�ļ������ֽ�
}info,info1;
int f_cc=0,retval;
char test[100];
timeval tv;
int tot=0;

clock_t wintime=0;				//����������ʱ��

struct My_user_keep
{
	unsigned short playing=0;	//״̬ 0���� 1���߻�����
	unsigned short ZHUO=0;		//�û��������� 0Ϊû������
	unsigned short set=-1;		//�����ڼ�����
	unsigned short ready=0;		//�Ƿ�׼���� 0δ׼���� 1׼����
	clock_t time;				//ʱ�䵥Ԫ 4�ֽ�
}My_user_keep[30];				//�û��ڷ������е�����

struct My_user_send
{
	char name[16];				//�û���
	int score=50000;			//����
	unsigned short winnum=0;	//��ʤ����
	unsigned short playnum=0;	//��ˣ����
	unsigned short run=0;		//���ܴ���
	unsigned short disconl=0;	//���ߴ���	
}My_user_send[30];				//���͵��ͻ��˵�����

struct PAIZHUO
{
	char name[3][16];			//�û���
	unsigned char id[3]={0,0,0};//�û�ID 0Ϊû����
	unsigned char isstat=0;		//�Ƿ񿪾�
	unsigned short pnum=0;		//��������
}PAIZHUO[10];					//������������Ϣ

struct PAI_Playing
{
	unsigned char PAI[3][21];	//�����û�����
	unsigned char CN[3];		//�����û��Ƶ�����
	unsigned short point;		//����
	unsigned char Ming[3];		//������Ϣ
}PAI_Playing[10];

struct PAI_keep
{
	short Fa_Pai=0;				//�Ƿ�����
	short DIZHU=-1;				//������˭ -1�ǻ�δ�г���
	short mark_[3][2];			//��¼�е��������
	short st,nn;				//�����￪ʼ��
	char PAIT[3];
}PAI_keep[10];

char name_set[30][20];

char bufu[200];

void Game_FAPAI(int PZ)

{
	unsigned char pp[54]={0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x5e,0x5f};

	int arr[54],f[54],i,j,z;

	srand((unsigned)time(0));
	memset(f,0,sizeof(f));
	
	for(i=0;i<54;++i)
	{
		z=rand()%54;
		bool l=false;
		for(j=z;j<54;++j)
			if(!f[j])
			{
				f[j]=1;
				arr[j]=i;
				l=true;
				break;
			}
		if(l)continue;
		for(j=z;j>=0;--j)
			if(!f[j])
			{
				f[j]=1;
				arr[j]=i;
				break;
			}
	}
	z=rand()%3; j=0;
	for(int i=0;i<17;++i)						//����
	{
		PAI_Playing[PZ].PAI[z][i]=pp[arr[j++]];
		z=(z+1)%3;
		PAI_Playing[PZ].PAI[z][i]=pp[arr[j++]];
		z=(z+1)%3;
		PAI_Playing[PZ].PAI[z][i]=pp[arr[j++]];
		z=(z+1)%3;
	}
	
	PAI_keep[PZ].PAIT[0]=pp[arr[51]];		//������
	PAI_keep[PZ].PAIT[1]=pp[arr[52]];
	PAI_keep[PZ].PAIT[2]=pp[arr[53]];
	
	PAI_Playing[PZ].CN[0]=17;
	PAI_Playing[PZ].CN[1]=17;
	PAI_Playing[PZ].CN[2]=17;
	
	unsigned char t;
	
	for(int k=0;k<3;++k)
		for(i=0;i<17;++i)
			for(j=i+1;j<17;++j)
				if(((PAI_Playing[PZ].PAI[k][i]%16)<(PAI_Playing[PZ].PAI[k][j]%16))||(((PAI_Playing[PZ].PAI[k][i]%16)==(PAI_Playing[PZ].PAI[k][j]%16))&&((PAI_Playing[PZ].PAI[k][i]/16)>(PAI_Playing[PZ].PAI[k][j]/16))))
				{
					t=PAI_Playing[PZ].PAI[k][i];
					PAI_Playing[PZ].PAI[k][i]=PAI_Playing[PZ].PAI[k][j];
					PAI_Playing[PZ].PAI[k][j]=t;
				}
}


void Game_End(int PZ,int i)
{
	bool l;
	if(i==PAI_keep[PZ].DIZHU)l=true;else l=false;
	info1.flag=SERVER_SHENGFU;
	info1.len=1;
	memset(bufu,0,sizeof(bufu));
	memcpy(bufu,(char*)&info1,3);
	switch(i)
	{
		case 0:memcpy(bufu+3,(char*)&data_0x00,1);break;
		case 1:memcpy(bufu+3,(char*)&data_0x01,1);break;
		case 2:memcpy(bufu+3,(char*)&data_0x02,1);break;
	}
	for(int j=0;j<3;++j)
		send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[j]].set].sc,bufu,info1.len+3,0);
	PAIZHUO[PZ].isstat=0;
	for(int j=0;j<3;++j)
	{
		My_user_keep[PAIZHUO[PZ].id[j]].ready=0;
		My_user_send[PAIZHUO[PZ].id[j]].playnum++;
		if(i==PAI_keep[PZ].DIZHU)
			if(l)
			{
				My_user_send[PAIZHUO[PZ].id[j]].score+=(PAI_Playing[PZ].point*2);
				My_user_send[PAIZHUO[PZ].id[j]].winnum++;
			}
			else My_user_send[PAIZHUO[PZ].id[j]].score-=(PAI_Playing[PZ].point*2);
		else
			if(l)My_user_send[PAIZHUO[PZ].id[j]].score-=PAI_Playing[PZ].point;
			else 
			{
				My_user_send[PAIZHUO[PZ].id[j]].score+=PAI_Playing[PZ].point;
				My_user_send[PAIZHUO[PZ].id[j]].winnum++;
			}
	}
	PAI_keep[PZ].Fa_Pai=0;
}

void Gaming(int k,int PZ)
{
	if(PAI_keep[PZ].Fa_Pai==0)			//��δ���� ���з��Ʋ���
	{
		PAI_keep[PZ].Fa_Pai=1;
		Game_FAPAI(PZ);
		for(int i=0;i<3;++i)			//û�����ĸ�ʽ
		{
			info1.flag=SERVER_FAPAI;
			info1.len=71;
			memset(bufu,0,sizeof(bufu));
			memcpy(bufu,(char*)&info1,3);
			memcpy(bufu+3,(char*)&PAI_Playing[PZ],71);
			//send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,4,0);
			//send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,bufu,info1.len,0);
			int len = 74;
			send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,bufu,len,0);
		}

		bool l=false;
		
		PAI_keep[PZ].st=0;
		
		info1.flag=SERVER_DIZHU;
		info1.len=0;
		send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[(PAI_keep[PZ].st)%3]].set].sc,(char*)&info1,3,0);
		l=true;
		PAI_keep[PZ].mark_[1][0]=0;
		PAI_keep[PZ].mark_[1][1]=(PAI_keep[PZ].st)%3;
	}
	else
	{
		
		int i,j;
		switch(info.flag)
		{
			case CLIENT_JIAODIZHU:	if(buff[0]==data_0x01)
									{
											for(i=0;i<3;++i)
												if(PAIZHUO[PZ].id[i]==pepo_sc[k].id)
												{
													PAI_keep[PZ].DIZHU=i;
													PAI_Playing[PZ].PAI[i][17]=PAI_keep[PZ].PAIT[0];
													PAI_Playing[PZ].PAI[i][18]=PAI_keep[PZ].PAIT[1];
													PAI_Playing[PZ].PAI[i][19]=PAI_keep[PZ].PAIT[2];
													unsigned char t;
													for(int ii=17;ii<20;++ii)
														for(int jj=ii-1;jj>=0;--jj)
															if(((PAI_Playing[PZ].PAI[i][jj]%16)<(PAI_Playing[PZ].PAI[i][jj+1]%16))||(((PAI_Playing[PZ].PAI[i][jj]%16)==(PAI_Playing[PZ].PAI[i][jj+1]%16))&&((PAI_Playing[PZ].PAI[i][jj]/16)>(PAI_Playing[PZ].PAI[i][jj+1]/16))))
															{
																t=PAI_Playing[PZ].PAI[i][jj];
																PAI_Playing[PZ].PAI[i][jj]=PAI_Playing[PZ].PAI[i][jj+1];
																PAI_Playing[PZ].PAI[i][jj+1]=t;
															}
															else break;
													break;
												}
											if(1)						//������
											{
												info1.flag=SERVER_DIZHUPAI;
												info1.len=3;
												memset(bufu,0,sizeof(bufu));
												memcpy(bufu,(char*)&info1,3);
												memcpy(bufu+3,(char*)&(PAI_keep[PZ].PAIT),3);
												//���µ�������
												for(i=0;i<3;++i)
												{
													//send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,4,0);
													//send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,buff,3,0);
													send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,bufu,6,0);
												}
											}
											
											if(1)						//˭�ǵ���
											{
												info1.flag=SERVER_WHODIZHU;
												info1.len=1;
												//info1.cc=PAI_keep[PZ].DIZHU;
												memset(bufu,0,sizeof(bufu));
												memcpy(bufu,(char*)&info1,3);
												memcpy(bufu+3,(char*)&(PAI_keep[PZ].DIZHU),1);
												for(i=0;i<3;++i)
													//send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,4,0);
													send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,bufu,4,0);
											}
									}
									else
									{
										//bool l=false;
										if(PAI_keep[PZ].mark_[1][0]+1<3)
											{
												info1.flag=SERVER_DIZHU;
												info1.len=0;
												send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[(PAI_keep[PZ].st+PAI_keep[PZ].mark_[1][0]+1)%3]].set].sc,(char*)&info1,3,0);
											//	l=true;
												PAI_keep[PZ].mark_[1][0]++;
												PAI_keep[PZ].mark_[1][1]=(PAI_keep[PZ].st+PAI_keep[PZ].mark_[1][0])%3;
												break;
											}
										else
														//û�˽е��� �ط���
											{
												PAI_keep[PZ].Fa_Pai=0;
												Gaming(k,PZ);
											}
									}
									break;
			case CLIENT_CHUPAI:		//�����޸�
									//info1.flag=SERVER_FAPAI;
									for(i=0;i<3;++i)
										if(PAIZHUO[PZ].id[i]==pepo_sc[k].id)
										{
											info1.flag=SERVER_CHUPAI__+i;
											j=(i+1)%3;
											break;
										}
									for(int ii=0,jj=0,kk=0;ii<PAI_Playing[PZ].CN[i];++ii)
										if(jj<info.len)
											if(PAI_Playing[PZ].PAI[i][ii]==buff[jj])
												{jj++;continue;}										
											else PAI_Playing[PZ].PAI[i][kk++]=PAI_Playing[PZ].PAI[i][ii];
										else PAI_Playing[PZ].PAI[i][kk++]=PAI_Playing[PZ].PAI[i][ii];
									PAI_Playing[PZ].CN[i]-=info.len;
									info1.len=info.len;
									memset(bufu,0,sizeof(bufu));
									memcpy(bufu,(char*)&info1,3);
									memcpy(bufu+3,buff,info1.len);
									for(i=0;i<3;++i)
										if(PAIZHUO[PZ].id[i]!=pepo_sc[k].id)
										{
											//if(info.len!=1)send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,4,0);
											//else send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,4,0);
											//if(info.len>1)send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,buf,info1.len,0);
											int len=strlen(bufu);
											send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,bufu,len,0);
										}
																	
									if(PAI_Playing[PZ].CN[(j+2)%3]==0)Game_End(PZ,(j+2)%3);
									else
									{
										info1.flag=SERVER_CHUPAI;
										info1.len=0;
										send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[j]].set].sc,(char*)&info1,3,0);
									}
									break;
			case CLIENT_BUCHU:		info1.flag=SERVER_FAPAI;
									info1.len=0;
									for(i=0;i<3;++i)
										if(PAIZHUO[PZ].id[i]==pepo_sc[k].id)j=(i+1)%3;
										else send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[i]].set].sc,(char*)&info1,3,0);
									info1.flag=SERVER_CHUPAI;
									info1.len=0;
									send(pepo_sc[My_user_keep[PAIZHUO[PZ].id[j]].set].sc,(char*)&info1,3,0);
									break;
		}
	}
}

void Table(int k)
{
	int PZ=My_user_keep[pepo_sc[k].id].ZHUO;
	memset(buff,0,sizeof(buff));
	recv(pepo_sc[k].sc,buff,info.len,0);
	memset(buf,0,sizeof(buf));
	if(PAIZHUO[PZ].isstat==1)Gaming(k,PZ);
	else
		if(info.flag==CLIENT_START)
		{
			if(buff[0]==data_0x01)My_user_keep[pepo_sc[k].id].ready=1;
			else My_user_keep[pepo_sc[k].id].ready=2;
			int cnt=0;
			PAI_Playing[PZ].point=10;
			for(int i=0;i<3;++i)
			{
				if(My_user_keep[PAIZHUO[PZ].id[i]].ready)cnt++;
				if(My_user_keep[PAIZHUO[PZ].id[i]].ready==2)
				{
					PAI_Playing[PZ].point=PAI_Playing[PZ].point<<1;
					PAI_Playing[PZ].Ming[i]=1;
				}
				else PAI_Playing[PZ].Ming[i]=0;
			}
			if(cnt==3){PAIZHUO[PZ].isstat=1;Gaming(k,PZ);}
		}
		else
			if(info.flag==CLIENT_LOGOUT)
			{
				//My_user_keep[pepo_sc[k].id].ZHUO=buff[0];
				for(int j=1;j<9;++j)
				{
					bool ll=false;
					for(int i=0;i<3;++i)
						if(PAIZHUO[j].id[i]==pepo_sc[k].id)
						{
							PAIZHUO[j].id[i]=0;
							memset(PAIZHUO[j].name[i],0,sizeof(PAIZHUO[j].name[i]));
							PAIZHUO[j].pnum--;
							ll=true;
						}
					if(ll)
					{
						memset(bufu,0,sizeof(bufu));
						info1.flag=SERVER_PAIZHUO;
						info1.len=54;
						
						memcpy(bufu,(char*)&info1,3);
						memcpy(bufu+3,(char*)&PAIZHUO[j],54);
						
						for(int i=0;i<3;++i)
							if(PAIZHUO[j].id[i]!=0)
								send(pepo_sc[My_user_keep[PAIZHUO[j].id[i]].set].sc,bufu,info1.len+3,0);
						break;
					}
				}
				My_user_keep[pepo_sc[k].id].ZHUO=0;
				My_user_keep[pepo_sc[k].id].ready=0;
				
				info1.flag=SERVER_PAIJU;
				info1.len=16;
													
				memset(bufu,0,sizeof(bufu));
				memcpy(bufu,(char*)&info1,3);
				//memcpy(bufu+3,(char*)&PAIZHUO[buff[0]],54);
				
				for(int i=1;i<9;++i)
					memcpy(bufu+((i-1)*2+3),(char*)&(PAIZHUO[i].pnum),2);
				
				send(pepo_sc[k].sc,bufu,info1.len+3,0);								
				//send(pepo_sc[k].sc,(char*)&info1,3,0);
				//send(pepo_sc[k].sc,bufu,info1.len,0);
			}
}

int main(int argc , char **argv)
{
	for(int i=0;i<30;++i)
		memset(name_set[i],0,sizeof(name_set[i]));
	memcpy(name_set[1],"CUI1",4);
	memcpy(name_set[2],"CUI2",4);
	memcpy(name_set[3],"CUI3",4);
	for(int i=0;i<30;++i)
	{
		memset(My_user_send[i].name,0,sizeof(My_user_send[i].name));
		memcpy(My_user_send[i].name,name_set[i],16);
	}
	if(daemon(0,1) == -1) ERR_EXIT("daemon error");
	signal(SIGCHLD,SIG_IGN);
	ssize_t siz = 0;
	for(int i=0;i<client_num;i++) pepo_sc[i].sc=-1;

	if(1)												//��������
	{
	ss = socket(AF_INET , SOCK_STREAM , 0);
	fcntl(ss,F_SETFL,O_NONBLOCK);
	
	if(ss < 0)
	{
		cout<<"socket error!"<<endl;
		return -1;
	}
	
	err = setsockopt(ss, SOL_SOCKET, SO_REUSEADDR,&reua,sizeof(reua));
	if(err)cout<<"set reuseaddr error!"<<endl;

	bzero(&server_addr , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8181);

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	err = bind(ss , (struct sockaddr*)&server_addr,sizeof(server_addr));
	if(err < 0)
	{
		cout<<"bind error!"<<endl;
		return -1;
	}
	
	err = listen(ss , BACKLOG);
	if(err < 0)
	{
		cout<<"listen error!"<<endl;
		return -1;
	}
	
	cout<<"����������������IP��"<<inet_ntoa(server_addr.sin_addr)<<endl<<"����PORT��"<<8181<<endl;		
	}
	unsigned int addrlen = sizeof(struct sockaddr);
	
	while(1)
	{
		if(1){											//select����
		FD_ZERO(&rfds);
		FD_SET(ss,&rfds);
		int max_sc=ss;
		for(int k=0;k<client_num;k++)
		{
			if(pepo_sc[k].sc!=-1)
			{
				FD_SET(pepo_sc[k].sc,&rfds);
				if(pepo_sc[k].sc>max_sc)max_sc=pepo_sc[k].sc;
			}
		}
        /* �������ȴ�ʱ�� */
        tv.tv_sec = 1;
        tv.tv_usec = 0;
		retval = select(max_sc + 1, &rfds, NULL, NULL, &tv);
		}
		if (retval == -1)
		{
			printf("���˳���select���� %s", strerror(errno));
			break;
		} 
		else 
			if (retval == 0) continue;                
            else 
			{
			if(FD_ISSET(ss,&rfds)){
				FD_CLR(ss,&rfds);
			sc = accept(ss,(struct sockaddr*)&client_addr,&addrlen);
			if(sc<=0)sc=sc;
			else					//ȷ������
			{
				char name[30];
					
				char inf[100];
					
				recv(sc,(head*)&info,3,0);
				
				if(info.flag==CLIENT_LOGIN)
				{
					
					memset(name,0,sizeof(name));
					
					recv(sc,name,info.len,0);
					
					bool l=false;
						
					for(int i=1;i<=30;++i)
					{
						if(strcmp(name_set[i],name)==0)
						{
							id=i; l=true; break;
						}
					}
					
					if(!l)								//�û�������
					{
						info1.flag=SERVER_LOGINDF;
						info1.len=1;
						
						memset(buf,0,sizeof(buf));
						memcpy(buf,(char*)&info1,3);
						memcpy(buf+3,(char*)&data_0x01,1);
						
						send(sc,buf,4,0);
						
						close(sc);
					}
					else 
					{
						info1.flag=SERVER_LOGINSS;
						//info1.len=0;
						//send(sc,(char*)&info1,4,0);					//�û���¼����������Ϣ
						
						memset(bufu,0,sizeof(bufu));

						info1.len=28;
						
						memcpy(bufu,(char*)&info1,3);
						memcpy(bufu+3,(char*)&My_user_send[id],28);
						send(sc,bufu,31,0);
						
						
						memset(bufu,0,sizeof(bufu));
						for(int j=1;j<9;++j)								//�û���¼
						{
							memcpy(bufu+((j-1)*2),(char*)&(PAIZHUO[j].pnum),2);		//ѹ��������Ϣ 3��λ�õ�id��
							
					//		memcpy(bufu+strlen(bufu),(char*)&PAIZHUO[j].isstat,1);	//�����Ƿ��ѿ���
						}
						
						info1.flag=SERVER_PAIJU;
						info1.len=16;
						
						memset(buf,0,sizeof(buf));
						memcpy(buf,(char*)&info1,3);
						memcpy(buf+3,bufu,info1.len);
						
						send(sc,buf,19,0);
						
					bool lc=false;
					
					for(int j=0;j<client_num;++j)
						if(pepo_sc[j].sc!=-1)
						if(strcmp(pepo_sc[j].name,name)==0)
						{
							close(pepo_sc[j].sc);
							lc=true;
							pepo_sc[j].sc=sc;
							if(My_user_keep[id].playing==1)			//��������
							{
								memset(bufu,0,sizeof(bufu));
								memcpy(bufu,(char*)&PAIZHUO[My_user_keep[id].ZHUO],54);
								memcpy(bufu+54,(char*)&PAI_Playing[My_user_keep[id].ZHUO],71);
								
								info.flag=SERVER_PLAYING; 
								info.len=strlen(bufu); 
								
								memset(buf,0,sizeof(buf));
								
								memcpy(buf,(char*)&info,3);
								memcpy(buf+3,bufu,info.len);
								
								send(sc,buf,info.len+3,0);
								
								My_user_keep[id].playing=0;
								My_user_send[id].disconl++;
							}
							My_user_keep[id].set=j;
							break;
						}
					if(!lc)
						for(int j=0;j<client_num;j++)
							if(pepo_sc[j].sc==-1) 
							{
								pepo_sc[j].sc=sc;
								pepo_sc[j].id=id;
								My_user_keep[id].set=j;
								memset(pepo_sc[j].name,0,sizeof(pepo_sc[j].name));
								memcpy(pepo_sc[j].name,name,16);
								
								printf("%s%s\n",name," login in");
								
								unsigned int addrlen = sizeof(struct sockaddr);
								break;
							}					
					}
				}				
			}
			}
//		}
				for(int k=0;k<client_num;++k)
				if(pepo_sc[k].sc!=-1)
				if(FD_ISSET(pepo_sc[k].sc,&rfds))
				{
					FD_CLR(pepo_sc[k].sc,&rfds);
					siz=recv(pepo_sc[k].sc,(head*)&info,3,0);
					if(siz<0)
					{
						if(errno==EAGAIN) continue;
						else										//�˳�
						{						
							printf("%s%s\n",pepo_sc[k].name," login out!");
//������ǿ��****						
							pepo_sc[k].sc=-1;
						}
					}
					else
						if(siz>0)
					{	
//						sprintf(test,"%c %c %d ",info.flag,info.flag2,info.len);
//							cout<<test<<endl;
						bool ll=false;
						My_user_keep[pepo_sc[k].id].time=wintime;
						if(My_user_keep[pepo_sc[k].id].ZHUO>0){Table(k);continue;}								//�û�������
						switch(info.flag)
						{
							case CLIENT_INFORMATION:info1.flag=SERVER_USERINFOR; info1.len=28;					//�û�����������Ϣ
													send(pepo_sc[k].sc,(char *)&info1,4,0);
													send(pepo_sc[k].sc,(char *)&My_user_send[pepo_sc[k].id],28,0);
													break;
							case CLIENT_PAIJU:		memset(bufu,0,sizeof(bufu));						//�ͻ����������������
													
													info1.flag=SERVER_PAIJU;
													info1.len=16;
													
													memcpy(bufu,(char*)&info1,3);
													
													for(int j=1;j<9;++j)
													{
														memcpy(bufu+(3+(j-1)*2),(char*)&(PAIZHUO[j].pnum),2);		//ѹ��������Ϣ 3��λ�õ�id��
														//memcpy(bufu+strlen(bufu),(char*)&(PAIZHUO[j].isstat),1);	//�����Ƿ��ѿ���
													}
													//send(pepo_sc[k].sc,(char*)&info1,4,0);
													//send(pepo_sc[k].sc,bufu,info1.len,0);
													send(pepo_sc[k].sc,bufu,info1.len+3,0);
													break;
							case CLIENT_PAIZHUO:	recv(pepo_sc[k].sc,buff,1,0);
													if(PAIZHUO[buff[0]].pnum==3)
													{
														info1.flag=SERVER_NOPAIZHUO;
														info1.len=0;
												//		cout<<"NO"<<endl;
														//info1.cc=1;
														memset(bufu,0,sizeof(bufu));
														memcpy(bufu,(char*)&info1,3);
														memcpy(bufu+3,(char*)&data_0x01,1);
														send(pepo_sc[k].sc,bufu,4,0);
													}
													else
													{
														My_user_keep[pepo_sc[k].id].ZHUO=buff[0];
														
											//			cout<<"YES"<<endl;
														for(int i=0;i<3;++i)
															if(PAIZHUO[buff[0]].id[i]==0)
															{
																PAIZHUO[buff[0]].id[i]=pepo_sc[k].id;
																memcpy(PAIZHUO[buff[0]].name[i],pepo_sc[k].name,16);
																break;
															}
															
														
														info1.flag=SERVER_PAIZHUO;
														info1.len=54;
														
														memset(bufu,0,sizeof(bufu));
														memcpy(bufu,(char*)&info1,3);
														//cout<<strlen(bufu)<<endl;
													//	cout<<bufu<<endl;
														memcpy(bufu+3,(char*)&PAIZHUO[buff[0]],54);
													
														//send(pepo_sc[k].sc,(char*)&info1,3,0);
														//send(pepo_sc[k].sc,bufu,info1.len,0);
														//cout<<info1.len<<endl;
														//cout<<strlen(bufu)<<endl;
														for(int i=0;i<3;++i)
															if(PAIZHUO[buff[0]].id[i]!=0)
															{
																send(pepo_sc[My_user_keep[PAIZHUO[buff[0]].id[i]].set].sc,bufu,info1.len+3,0);
															}
										
														PAIZHUO[buff[0]].pnum++;
													}
						}
					}	
				}
			}
		
								
	}
	cout<<"***";
	return 0;
}