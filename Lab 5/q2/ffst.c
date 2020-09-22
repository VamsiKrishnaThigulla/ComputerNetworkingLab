// fastest finger first - client

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

int c_sock;

char arr[100];
int arr_size;

int clientcreate(int port ,int anyip, char IPADDR[])
{
	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	if(anyip == 0) client.sin_addr.s_addr = inet_addr(IPADDR);
	else client.sin_addr.s_addr = INADDR_ANY;
	if(connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		printf("Connect Issue\n");
		return 1;
	}
	else return 0;
}

int GetCurTime(char returnval[], int display)
{
	time_t t;
	struct tm * timeinfo;
	time(&t);
	timeinfo = localtime(&t);

	int year = timeinfo->tm_year + 1900;
	int month = timeinfo->tm_mon + 1;
	int date = timeinfo->tm_mday;
	int hour = timeinfo->tm_hour;
	int minutes = timeinfo->tm_min;
	int seconds = timeinfo->tm_sec;

	if(display == 1) 
		printf("\ncurrent time :%d:%d:%d - %d/%d/%d\n",hour,minutes,seconds,date,month,year );

	if(strcmp(returnval,"year")==0)
		return year;
	else if (strcmp(returnval,"month")==0)
		return month;
	else if (strcmp(returnval,"date")==0)
		return date;
	else if (strcmp(returnval,"hour")==0)
		return hour;
	else if (strcmp(returnval,"minutes")==0)
		return minutes;
	else if (strcmp(returnval,"seconds")==0)
		return seconds;
	else return -1;


}

int main()
{
	int ex2 = 0;
	
	while (ex2 ==0) // loop checker to terminate
	{
		char name[50];
		printf("\nEnter Name :\n");
		scanf("%s",name);

		char ipaddr[30];
		printf("Enter IP Address: \n");
		scanf("%s",ipaddr);

		int port ;
		printf("Enter Port: \n");
		scanf("%d",&port);

		int check = clientcreate(port,0,ipaddr);
		if(check == 1)
		{
			close(c_sock);
			printf("Error in client creation\n");
		}
		else
		{
			int recv_mode = 0;
			char port_str[30];
			int i=0;
			int p = port;

			while(p>0) // converting port to string for send() API
			{
				port_str[i] = (char)(48 + p%10);
				i++;
				p = p/10;
			}
			port_str[i] = '\0';

			send(c_sock,name,sizeof(name),0);
			send(c_sock,ipaddr,sizeof(ipaddr),0);
			send(c_sock,port_str,sizeof(port_str),0);

			char questions[300];
			char answers[300];
			char reply[200];

			printf("\nServer Connected\n");

			int exloop = 0;
			while(exloop == 0)
			{
				char finish[2];
				recv(c_sock,finish,sizeof(finish),0);
				if(finish[0] == '1')
				{
					exloop =1;
				}
				else
				{
					recv(c_sock,questions,sizeof(questions),0);
					printf("\nQuestions : \n");
					printf("Enter Answer : \n");
					fflush(stdout);
					int iresult;
					struct timeval tv;
					fd_set rfds;
					FD_ZERO(&rfds);
					FD_SET(0,&rfds);

					tv.tv_sec = 10;
					iresult = select(1,&rfds,NULL,NULL,&tv);
					if(iresult == 0)
					{
						printf("TIMEOUT!!!\n");
						strcpy(answers,"-666");
					}
					else
					{
						FD_ISSET(0,&rfds);
						scanf("%[^\n]",answers);
					}
					send(c_sock,answers,sizeof(answers),0);
					recv(c_sock,reply,sizeof(reply),0);
					printf("\n%s\n",reply);
					if(strcmp(reply,"Better Luck Next Time...")==0)
					{
						close(c_sock);
						return 0;
					}
				}
			}

			char result[30];
			recv(c_sock,result,sizeof(result),0);
			printf("\nResult : \nCorrect: ");
			for(int i=0;i<strlen(result);i++)
			{
				if(result[i] == 'c')
				{
					printf("\nWrong: ");

				}
				else if(result[i] == 'w')
				{
					printf("\nLate: ");

				}
				else if(result[i] == 'l')
				{
					printf("\nTotal number of questions :");

				}
				else if(result[i] == 'q')
				{
					printf("\n");
				}
				else 
					printf("%c\n",result[i]);
			}


		}
		printf("Do you want to exit : ");
		scanf("%d",&ex2);
	}
	close(c_sock);
	return 0;
}