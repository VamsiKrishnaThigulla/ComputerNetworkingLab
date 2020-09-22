#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h> 

int main() 
{ 
    int socket_calc,len;
    char buffer[1024]="hello server!";
	socket_calc = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in serveraddr; 
	
	if (socket_calc == -1)
	{
		printf("Could not create socket");
	}
	
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_port = htons(8080); 
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.9"); 
	
	struct input{    
    int choice;
    double num1,num2;
    } Input;
    double Output;

	while(1)
	{
		sendto(socket_calc,buffer,sizeof(buffer),0,(const struct sockaddr*)&serveraddr,sizeof(serveraddr));
		recvfrom(socket_calc,buffer,sizeof(buffer),0,NULL,NULL);
        puts(buffer);
        scanf("%d%lf%lf",&Input.choice,&Input.num1,&Input.num2);
        if(Input.choice > 6)
        {
            printf("pls select from given options!\n");
            continue;
        }
		sendto(socket_calc,(struct Input*)&Input,sizeof(Input),0,(const struct sockaddr*)&serveraddr,sizeof(serveraddr));
        if(Input.choice == 5)
            break;

		recvfrom(socket_calc,(struct input*)&Output,sizeof(Output),0,NULL,NULL);
        printf("Output:%lf\n",Output);
	}
	close(socket_calc);
	return 0;
} 
