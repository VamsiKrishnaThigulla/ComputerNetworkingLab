#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

void die(char *message)
{
    printf("%s",message);
    exit(1);
}

int main()
{
    int sd;
    char buffer[1024] ="HELLO!\n";

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        die("Error socket not opened!\n");

    struct sockaddr_in Server;
    Server.sin_family = AF_INET;
    Server.sin_port = htons(8080);
    Server.sin_addr.s_addr = inet_addr("0");

    if(connect(sd,(const struct sockaddr*)&Server,sizeof(Server)))
        die("Unable to connect to server!\n");

    struct input{    
    int choice;
    double num1,num2;
    } Input;
    double Output;

    while(1)
    {
        printf("1)Add\n2)Subtract\n3)Multiply\n4)Divide\n5)Exit(pls enter 5 5 5)\nEnter Operation\n num1\n num2\n");
        scanf("%d%lf%lf",&Input.choice,&Input.num1,&Input.num2);
        if(Input.choice > 6)
        {
            printf("pls select from given options!\n");
            continue;
        }
        write(sd,&Input,sizeof(Input));

        if(Input.choice == 5)
            break;

        read(sd,&Output,sizeof(Output));
        printf("Output:%lf\n",Output);
    }

    close(sd);    
}

/*
    write(sd,"Hello server!",sizeof("Hello server!"));
    int n = read(sd,buffer,sizeof(buffer));
    buffer[n]='\0';
    puts(buffer);
*/
