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
    int socket_img,len,n;
    char buffer[1024]="Hello server!";
	socket_img = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in Serve_Addr; 
	FILE *img;

	img=fopen("Image.bmp","rb"); //Open file in read binary mode as the mp3 file is stored in binary
	
	if(!img)
		printf("Error! File does not exist.\n");
	else
    		printf("File opened successfully \n");
    	
	if (socket_img == -1)
	{
		printf("Socket could not be created.\n");
	}
	else
		printf("Socket creation successful\n");
	
	
	Serve_Addr.sin_family = AF_INET; 
	Serve_Addr.sin_port = htons(8080); 
	Serve_Addr.sin_addr.s_addr = inet_addr("127.0.0.9"); 


	while ((n = fread(buffer, 1, 1024, img)) > 0)
    {
		sendto(socket_img,buffer,sizeof(buffer),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
		sendto(socket_img,&n,sizeof(n),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
        usleep(100);
    }
    strcpy(buffer,"END"); //Signify end of file
    n=0;
	sendto(socket_img,buffer,sizeof(buffer),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));
	sendto(socket_img,&n,sizeof(n),0,(const struct sockaddr*)&Serve_Addr,sizeof(Serve_Addr));

	fclose(img);
	
	close(socket_img);
	
	return 0;
} 
