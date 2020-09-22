#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include<time.h>

void die(char *message)
{
    printf("%s",message);
    exit(1);
}

int main()
{
    int socket_image,n,seconds;
    char buffer[1024] ="";
    FILE *img;

    socket_image = socket(AF_INET,SOCK_STREAM,0);
    if(socket_image==-1)
        die("Error! Socket not opened.\n");

    struct sockaddr_in Serve_Port;
    Serve_Port.sin_family = AF_INET;
    Serve_Port.sin_port = htons(8080);
    Serve_Port.sin_addr.s_addr = inet_addr("0");

    if(connect(socket_image,(const struct sockaddr*)&Serve_Port,sizeof(Serve_Port)))
        die("Error! Unable to connect to server.\n");

    img=fopen("Image.bmp","rb"); //As the media file is stored in binary, thus the file is opened in rb mode i.e. read binary mode
    
    if(!img)
    	printf("Error! File does not exist.\n");
    else
    	printf("File opened successfully \n");

    while ((n = fread(buffer, 1, 1024, img)) > 0) //Store the file into the buffer
    {
        write(socket_image,&buffer,sizeof(buffer));
        write(socket_image,&n,sizeof(n));
        usleep(500); 
        /*
        In case the transmission rate is faster than the rate at which packets can be received, data might be lost
        Thus usleep is used to slow down the transmission to prevent loss of data
        */
    }

    strcpy(buffer,"END"); //To signify the end of the file
    n=0;
    write(socket_image,&buffer,sizeof(buffer));
    write(socket_image,&n,sizeof(n));
    fclose(img);
  
    close(socket_image);    
    
    printf("Socket closed \n");
    
    return 0;
}


