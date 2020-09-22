#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h> 

int main() 
{ 
    int socket_chat,len;
    char buffer[1024]="hello";
	socket_chat = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in servaddr; 
	
	if (socket_chat == -1)
	{
		printf("Could not create socket");
	}
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(8080); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.9"); 
	
	while(1)
	{
		printf("Input Ctrl+C to end chat\n");
		printf("Client : "); 
		fflush(stdin);
		fgets(buffer,1024,stdin);
		sendto(socket_chat, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr,sizeof(servaddr)); 
			
		int n=recvfrom(socket_chat, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &servaddr, &len); 
		buffer[n]='\0';
	
		printf("Server : %s", buffer); 
		
	}
	close(socket_chat);
	return 0;
} 
