#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
int main()
{
	int client_socket ;
	char buffer[200];
	char res[200];
	
	client_socket = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in client;
	
	memset(&client,0,sizeof(client));
	
	client.sin_family = AF_INET;
	client.sin_port = htons(8080);
	client.sin_addr.s_addr = INADDR_ANY;
	
	if (connect(client_socket,(struct sockaddr*)&client,sizeof(client))==-1)
	{
		printf("connection not established\n");
		return 0;
	}
	else
		printf("Connection establised to server.\n");
	
	recv(client_socket,buffer,sizeof(buffer),0);
	
	printf("%s\n",buffer);
	
	printf("Enter 'stop' to end the chat \n");
	
	while(1)
	{
		gets(buffer);
		
		send(client_socket,buffer,sizeof(buffer),0);
		
		if(strcmp(buffer,"stop")==0)
		{
			break;
		}
		
		recv(client_socket,res,sizeof(res),0);
		
		printf("Server : %s \n",res );
	}
	
	close(client_socket);
	printf("Connection closed\n");
	return 0;
}
