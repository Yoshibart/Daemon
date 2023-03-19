#include<stdio.h>
#include<string.h>    //for strlen..
#include<sys/socket.h>
#include<arpa/inet.h> //for inet_addr
#include<unistd.h>    //for write

#define NAME_SIZE 100
#define DEPT_SIZE 50
#define MESSAGE_SIZE 2000
#define CLIENT_MESSAGE 2000
#define SERVER_MESSAGE 2000

int main(int argc , char *argv[])
{
    int SFD;
    struct sockaddr_in server;
    char clientMessage[CLIENT_MESSAGE];
    char serverMessage[SERVER_MESSAGE];
    char name[NAME_SIZE] = {'\0'};
    char department[DEPT_SIZE] = {'\0'};
    char option[2];
    //Create socket
    SFD = socket(AF_INET , SOCK_STREAM , 0);
    if (SFD == -1)
    {
        printf("Error creating socket");
    }
    
    // set sockaddr_in variables
    server.sin_port = htons( 8082 ); // Port to connect on
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP
    server.sin_family = AF_INET; // IPV4 protocol
    
 
    //Connect to server
    if (connect(SFD , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connect failed. Error");
        return 1;
    }else{
        printf("Server connection successfully..............\n");
    }
    printf("\nEnter admin name: ");
    scanf("%s" , name);

    //Send name of the admin
    if( send(SFD , name , strlen(name) , 0) < 0)
    {
        printf("Sending name failed.");
    }
     
    //keep communicating with server
    while(1)
    {
        printf("%s","\nChoose an option:\n\t1: Shared Directory\n\t2: BackUp\n");
        scanf("%s" , option);
         
        if( send(SFD , option , strlen(option) + 1, 0) < 0)
        {
            printf("Sending option failed.");
        }

    }
     
    close(SFD);
    return 0;
}
