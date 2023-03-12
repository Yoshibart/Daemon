#include<stdio.h>
#include<string.h>    //for strlen..
#include<sys/socket.h>
#include<arpa/inet.h> //for inet_addr
#include<unistd.h>    //for write
 
int main(int argc , char *argv[])
{
    int SFD;
    struct sockaddr_in server;
    char clientMessage[500];
    char serverMessage[500];
     
    //Create socket
    SFD = socket(AF_INET , SOCK_STREAM , 0);
    if (SFD == -1)
    {
        printf("Error creating socket");
    } {
    	printf("socket created");
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
    }
     
    printf("Connected to server ok!!\n");
     
    //keep communicating with server
    while(1)
    {
        printf("\nEnter your Name: ");
        scanf("%s" , clientMessage);
         
        //Send some data
        if( send(SFD , clientMessage , strlen(clientMessage) , 0) < 0)
        {
            printf("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(SFD , serverMessage , 500 , 0) < 0)
        {
            printf("IO error");
            //break;
        }
         
        puts("\nServer sent: ");
        puts(serverMessage);

    }
     
    close(SFD);
    return 0;
}
