#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

double calculate(int choice, double num1, double num2)
{
    switch (choice){
    case 1:
        return(num1+num2);

    case 2:
        return(num1-num2);
    
    case 3:
        return(num1*num2);

    case 4:
        return(num1/num2);
    }
}


int main() 
{ 
    int socket_calc;
    char buffer[1024] ="1)Add\n2)Subtract\n3)Multiply\n4)Divide\n5)Exit(pls enter 5 5 5)\nEnter Operation\n num1\n num2\n";
	char tmp[100];
	socket_calc = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in serveraddr,clientaddr; 
	
	if (socket_calc == -1)
	{
		printf("Could not create socket");
	}
	
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_port = htons(8080); 
	serveraddr.sin_addr.s_addr = inet_addr("0"); 
	
	bind(socket_calc, (const struct sockaddr *)&serveraddr,sizeof(serveraddr));
	
	int len = sizeof(clientaddr);

	struct input{    
    int choice;
    double num1,num2;
    } Input;
	double Output;

	while(1)
	{
		recvfrom(socket_calc, (char *)tmp, 100, MSG_WAITALL, ( struct sockaddr *) &clientaddr, &len); 
		sendto(socket_calc,(const char*)buffer,sizeof(buffer),0,(const struct sockaddr*)&clientaddr,len);
		recvfrom(socket_calc,(struct input*)&Input,sizeof(Input),0,NULL,NULL);
        if(Input.choice == 5)
            break;
        Output = calculate(Input.choice,Input.num1,Input.num2);
		sendto(socket_calc,(double*)&Output,sizeof(Output),0,(const struct sockaddr*)&clientaddr,len);
	} 
	close(socket_calc);
	return 0;
}
