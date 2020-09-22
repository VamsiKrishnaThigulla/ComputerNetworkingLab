#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

int main() 
{ 
    int socket_chat;
    char buffer[1024] = "";
	socket_chat = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in servaddr,cliaddr; 
	
	if (socket_chat == -1)
	{
		printf("Could not create socket");
	}
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(8080); 
	servaddr.sin_addr.s_addr = inet_addr("0"); 
	
	bind(socket_chat, (const struct sockaddr *)&servaddr,sizeof(servaddr));
	
	int len = sizeof(cliaddr);
	
	while(1)
	{
		printf("Input Ctrl+C to end chat\n");
	   	int n=recvfrom(socket_chat, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
	   	buffer[n]='\0';
	   	
	   	printf("Client : %s", buffer); 
		printf("Server : "); 
		
		fflush(stdin);			
		fgets(buffer,1024,stdin);
	    	
	   	sendto(socket_chat, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &cliaddr,sizeof(cliaddr)); 
	} 
	close(socket_chat);
	return 0;
}
