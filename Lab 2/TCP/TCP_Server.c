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
    int sd,conn_sd;
    char buffer[1024];       

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        die("Socket creation unsucessfull\n");
    struct sockaddr_in Server,Client;
    memset(&Server,0,sizeof(Server));

    Server.sin_family = AF_INET;
    Server.sin_port = htons(8080);
    Server.sin_addr.s_addr = inet_addr("0");

    if(bind(sd,(const struct sockaddr*)&Server,sizeof(Server)))
        die("Could not bind!\n");

    if(listen(sd,5))
        die("Too many connections!!!");

    socklen_t cli_len = sizeof(Client);
    conn_sd = accept(sd,(struct sockaddr*)&Client,&cli_len);

    if(conn_sd < 1)
        die("Error, can't accept connection!");

struct input{    
    int choice;
    double num1,num2;
    } Input;
double Output;

    while(1)
    {
        read(conn_sd,&Input,sizeof(Input));
        if(Input.choice == 5)
            break;
        Output = calculate(Input.choice,Input.num1,Input.num2);
        write(conn_sd,&Output,sizeof(Output));
    }

    close(sd);
}

/*
    // recieve data from client
    int n = read(conn_sd,buffer,1024);
    buffer[n] = '\0';
    puts(buffer);

    // send data to client
    write(conn_sd,"Hello client!",sizeof("Hello client!"));
*/
