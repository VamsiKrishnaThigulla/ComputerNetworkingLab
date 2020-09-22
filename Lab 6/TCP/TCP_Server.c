#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/time.h> 
#include <string.h> 
#include<time.h>

void die(char *message)
{
    printf("%s",message);
    exit(1);
}
 
int main()
{
    int socket_img,conn_socket_img,num_pack=0;
    struct timeval begin,end; //To measure latency
    double tmp=0.0,avg=0.0;
    char buffer[1024] = ""; 
    FILE *img;
    img = fopen("ImageCopyTCP.bmp","wb");    

    if(!img)
       { printf("Error! Unable to open file.\n");
        return 0;}
    else
    	printf("File opened successfully \n");


    socket_img = socket(AF_INET,SOCK_STREAM,0);
    if(socket_img==-1)
        die("Socket creation unsuccessful.\n");
    else
    	printf("Socket created succesfully \n");

    
    struct sockaddr_in Serve_Port,Client_Port;
    memset(&Serve_Port,0,sizeof(Serve_Port));

    Serve_Port.sin_family = AF_INET;
    Serve_Port.sin_port = htons(8080);
    Serve_Port.sin_addr.s_addr = inet_addr("0");

    if(bind(socket_img,(const struct sockaddr*)&Serve_Port,sizeof(Serve_Port)))
        die("Error! Binding unsuccessful.\n");
    else
    	printf("Socket bound \n");

    if(listen(socket_img,5))
        die("Error! Connections queue exceeding the limit.\n");

    socklen_t cli_len = sizeof(Client_Port);
    conn_socket_img = accept(socket_img,(struct sockaddr*)&Client_Port,&cli_len);

    if(conn_socket_img < 1)
        die("Error! Cannot accept the connection.\n");
        
    printf("Connection successful\n");

    int n;

    while(1)
    {
        gettimeofday(&begin,NULL);
        
        read(conn_socket_img,&buffer,sizeof(buffer));
        read(conn_socket_img,&n,sizeof(n));
        
        gettimeofday(&end,NULL);

        num_pack++;
        tmp = end.tv_usec - begin.tv_usec; //Take the time taken in microseconds
        if(tmp>0)
            avg = avg - avg/num_pack +tmp/num_pack;

        if(!strcmp(buffer,"END") && n==0)
            break;

        fwrite(buffer,1, n, img);
    }

    printf("Average latency:%lf microseconds\n",avg);

    close(socket_img);
    close(conn_socket_img);
    fclose(img);   
  
    printf("All sockets closed \n");
    return 0;
}
