#include<stdio.h>      // for IO
#include<string.h>     //for strlen
#include<stdlib.h>
#include<sys/socket.h> // for socket
#include<arpa/inet.h>  //for inet_addr
#include<unistd.h>     //for write
#include <pthread.h>

void *handle_client(void *);

int main(int argc , char *argv[])
{
    int s; // socket descriptor
    int cs; // Client Socket
    int connSize; // Size of struct 
    int READSIZE;  // Size of sockaddr_in for client connection

    struct sockaddr_in server , client;
    char message[500];
     
    //Create socket
    s = socket(AF_INET , SOCK_STREAM , 0);
    if (s == -1)
    {
        printf("Could not create socket");
    } else {
        printf("Socket Successfully Created!!");
    } 

    // set sockaddr_in variables
    server.sin_port = htons( 8082 ); // Set the prot for communication
    server.sin_family = AF_INET; // Use IPV4 protocol
    server.sin_addr.s_addr = INADDR_ANY; 
    // When INADDR_ANY is specified in the bind call, the  socket will  be bound to all local interfaces. 
    
     
    //Bind
    if( bind(s,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Bind issue!!");
        return 1;
    } else {
        printf("Bind Complete!!");
    }
     
    //Listen for a conection
    listen(s,5); 

    //Accept and incoming connection
    printf("Waiting for incoming client connections.................");
    connSize = sizeof(struct sockaddr_in);

    while(1) {
        cs = accept(s, (struct sockaddr *)&client, (socklen_t*)&connSize);
        if (cs < 0)
        {
            perror("Can't establish connection");
            continue;
        } else {
            printf("Connection from client accepted!!\n");
        }

        pthread_t thread;
        int *new_sock = malloc(sizeof(int));
        *new_sock = cs;
        if (pthread_create(&thread, NULL, handle_client, (void*) new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        } else {
            printf("Thread created for client\n");
        }
    }
    return 0;
}

void *handle_client(void *sock_ptr) {
    int sock = *(int*)sock_ptr;
    int read_size;
    char message[500];
    memset(message, 0, 500);
    while ((read_size = recv(sock , message , 2000 , 0)) > 0 ) {
        printf("Client %d said: %s\n", sock, message);
        float num = atof(message) * (9.0/5) + 32.0;
        char mess[500];
        sprintf(mess, "%.2f Fahrenheit", num);
        write(sock, mess, strlen(mess));
        memset(message, 0, 500);
    }
    if (read_size == 0) {
        printf("Client %d disconnected\n", sock);
    } else if (read_size == -1) {
        perror("read error");
    }
    free(sock_ptr);
    close(sock);
    pthread_exit(NULL);
}
