#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h> 

void die(char *message)
{
    printf("%s",message);
    exit(1);
}

int main() 
{ 
    int socket_music,len,n;
    char buffer[1024]="Hello server!";
	socket_music = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in Serve_Addr; 
	FILE *track;

	track=fopen("Camelot.mp3","rb"); //Open file in read binary mode as the mp3 file is stored in binary
	
	if (socket_music == -1)
	{
		printf("Socket could not be created.\n");
	}
	
	Serve_Addr.sin_family = AF_INET; 
	Serve_Addr.sin_port = htons(8080); 
	Serve_Addr.sin_addr.s_addr = inet_addr("127.0.0.9"); 


	while ((n = fread(buffer, 1, 1024, track)) > 0)
    {
		sendto(socket_music,buffer,sizeof(buffer),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
		sendto(socket_music,&n,sizeof(n),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
        usleep(100);
    }
    strcpy(buffer,"END"); //Signify end of file
    n=0;
	sendto(socket_music,buffer,sizeof(buffer),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
	sendto(socket_music,&n,sizeof(n),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));

	close(socket_music);
	return 0;
} 
