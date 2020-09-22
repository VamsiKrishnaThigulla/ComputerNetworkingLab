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
    int socket_music,n,seconds;
    char buffer[1024] ="",ack;
    FILE *track;

    socket_music = socket(AF_INET,SOCK_STREAM,0);
    if(socket_music==-1)
        die("Error! Socket not opened.\n");

    struct sockaddr_in Serve_Port;
    Serve_Port.sin_family = AF_INET;
    Serve_Port.sin_port = htons(8080);
    Serve_Port.sin_addr.s_addr = inet_addr("0");

    if(connect(socket_music,(const struct sockaddr*)&Serve_Port,sizeof(Serve_Port)))
        die("Error! Unable to connect to server.\n");

    track=fopen("Camelot.mp3","rb"); //As the media file is stored in binary, thus the file is opened in rb mode i.e. read binary mode

    while ((n = fread(buffer, 1, 1024, track)) > 0) //Store the file into the buffer
    {
        write(socket_music,&buffer,sizeof(buffer));
        write(socket_music,&n,sizeof(n));
        usleep(500); //To slow down transmittin speed from client as here, client is transmitting faster than server
    }

    strcpy(buffer,"END"); //To signify the end of the file
    n=0;
    write(socket_music,&buffer,sizeof(buffer));
    write(socket_music,&n,sizeof(n));
    fclose(track);
    close(socket_music);    
}

//sleep is used to prevent packet loss due to client transmitting faster than server can transmit
