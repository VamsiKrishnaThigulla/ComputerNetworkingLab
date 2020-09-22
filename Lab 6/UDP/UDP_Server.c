#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <arpa/inet.h>
#include <string.h> 

void die(char *message)
{
    printf("%s",message);
    exit(1);
}

int main() 
{ 
   	int socket_img,n,num_pack=0;
	struct timeval begin,end;
    	double tmp=0.0,avg=0.0;
   	char buffer[1024] =""; 
	socket_img = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in Serve_Addr,Client_Addr; 
	
	FILE *img;
    	img = fopen("ImageCopyUDP.bmp","wb");    
	
	if (socket_img == -1)
	{
		printf("Could not create socket");
	}
	else
		printf("Socket creation successful \n");
	
	Serve_Addr.sin_family = AF_INET; 
	Serve_Addr.sin_port = htons(8080); 
	Serve_Addr.sin_addr.s_addr = inet_addr("0"); 
	
	bind(socket_img, (const struct sockaddr *)&Serve_Addr,sizeof(Serve_Addr));
	
	int len = sizeof(Client_Addr);

	while(1)
    	{	//Write the contents of the file, received through the client, into the buffer
		gettimeofday(&begin,NULL);
		
		recvfrom(socket_img,(char *)buffer,1024,MSG_WAITALL, (struct sockaddr *)&Client_Addr,&len); 
		recvfrom(socket_img,(int *)&n,sizeof(n),MSG_WAITALL, (struct sockaddr *)&Client_Addr,&len);
		
		gettimeofday(&end,NULL);

		num_pack++;
        tmp = end.tv_usec - begin.tv_usec; //Take the time taken in microseconds
        if(tmp>0)
            avg = avg - avg/num_pack +tmp/num_pack;

        if(!strcmp(buffer,"END") && n==0)
            break;
        
        fwrite(buffer,1, n, img); //Write from the buffer to the file
    	}

	printf("Average latency:%lf microseconds\n",avg);
	fclose(img);

	close(socket_img);
	
	printf("Socket closed \n");
	return 0;
}
