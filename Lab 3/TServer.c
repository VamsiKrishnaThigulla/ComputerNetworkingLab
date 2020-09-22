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
	int server_socket, client_socket;
	double value;
	char buffer[200]= "Welcome to the Chat application created by Vamsi,Srinivasan,Krishna Sai,Hrishikesh,Darshan and Bhargav";
	char res[200];
	printf("Listening for connections \n");
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server, client;
	
	memset(&server,0,sizeof(server));
	memset(&client,0,sizeof(client));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	server.sin_addr.s_addr = INADDR_ANY;
	
	socklen_t csize;
	bind(server_socket,(struct sockaddr *)&server,sizeof(server));
	listen(server_socket,200);
	csize = sizeof(client);
	client_socket = accept(server_socket,(struct sockaddr *)&client,&csize);
	send(client_socket,buffer,sizeof(buffer),0);
	while(1)
	{
		recv(client_socket,buffer,sizeof(buffer),0);
		printf("Client: %s \n",buffer);
		gets(res);
		if(strcmp(res,"stop")==0)
			break;
		send(client_socket,res,sizeof(res),0);
	}
	
	close(server_socket);
	printf("Connection closed\n");
	return 0;
}
