#include<stdio.h>      // for IO
#include<string.h>     //for strlen
#include<stdlib.h>
#include<sys/socket.h> // for socket
#include<arpa/inet.h>  //for inet_addr
#include<unistd.h>     //for write
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NAME_SIZE 100
#define DEPT_SIZE 50
#define MESSAGE_SIZE 2000

void *handle_client(void *);
void error_redirect(char *);
void success_redirect(char *);

char success_message[100];
int main(int argc , char *argv[])
{
    int s; // socket descriptor
    int cs; // Client Socket
    int connSize; // Size of struct 
    int READSIZE;  // Size of sockaddr_in for client connection

    struct sockaddr_in server , client;
    char message[MESSAGE_SIZE];
     
    //Create socket
    s = socket(AF_INET , SOCK_STREAM , 0);
    if (s == -1)
    {
        error_redirect("\nCould not create socket");
    } else {
        success_redirect("\nSocket Successfully Created!!");
    } 

    // set sockaddr_in variables
    server.sin_port = htons( 8082 ); // Set the prot for communication
    server.sin_family = AF_INET; // Use IPV4 protocol
    server.sin_addr.s_addr = INADDR_ANY; 
    // When INADDR_ANY is specified in the bind call, the  socket will  be bound to all local interfaces. 
    
     
    //Bind
    if( bind(s,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        error_redirect("Bind issue!!");
        return 1;
    } else {
        success_redirect("\nBind Complete!!");
    }
     
    //Listen for a conection
    listen(s,5); 

    //Accept and incoming connection
    success_redirect("\nWaiting for incoming client connections.................");
    connSize = sizeof(struct sockaddr_in);

    while(1) {
        cs = accept(s, (struct sockaddr *)&client, (socklen_t*)&connSize);
        if (cs < 0)
        {
            error_redirect("\nCan't establish connection");
            continue;
        } else {
            success_redirect("\nClient connection established!!");
        }

        pthread_t thread;
        //uses malloc to create a block for a socket
        int *new_sock = malloc(sizeof(int));
        *new_sock = cs;

        // creates thread to multi-thread users
        if (pthread_create(&thread, NULL, handle_client, (void*) new_sock) < 0) {
            error_redirect("\nCould not create thread");
            return 1;
        } else {
            success_redirect("\nThread created for client");
        }
    }
    return 0;
}

void *handle_client(void *sock_ptr) {
    int sock = *(int*)sock_ptr;
    char name[NAME_SIZE] = {'\0'};
    int read_size;
    char option[2] = {'\0', '\0'}; // initialize with null characters
    if(recv(sock , name , NAME_SIZE , 0) > 0){
        sprintf(success_message, "\n%s admin account successfully setup.\n",name);
        success_redirect(success_message);
    }else{
        error_redirect("\nSetting Admin name failed\n");
    }
    int pid = fork();
    if (pid > 0) {
        success_redirect("\nParent process");
        sleep(10);  // uncomment to wait 10 seconds before process ends
        exit(EXIT_SUCCESS); // Kill the parent, needed to make orphan
    } else if (pid == 0) {
        memset(name, 0, NAME_SIZE);
        memset(option, 0, 2);
        // Elevate the orphan process to session leader, to loose controlling TTY
        if (setsid() < 0)
            exit(EXIT_FAILURE);
        // umask() to set the file mode creation mask to 0
        umask(0);
        // Change the current working dir to root.
        if (chdir("/") < 0 )
            exit(EXIT_FAILURE);
        while(1) {
            //The daemon's execution code
            read_size = recv(sock ,option , 1, 0);
            if (read_size <= 0) {
                break;  // Break the loop if option read failed
            }
            char opt = option[0];
            // This option handles the Shared Directory implementation
            if(opt == '1'){
                int optPid =  fork();
                if (optPid == -1) {
                    error_redirect("\nbad fork");
                } else if (optPid == 0) {
                    //Not Implemented
                }  
            // This option handles the BackUp implementation
            }else if(opt == '2'){
                int optPid =  fork();
                if (optPid == -1) {
                    error_redirect("\nbad fork");
                } else if (optPid == 0) {
                    //Not Implemented
                }  
            }
        }
    }else{
        error_redirect("\nForking failed");
    }
    sprintf(success_message, "%s has been disconnected", name);
    success_redirect(success_message);
    free(sock_ptr);
    close(sock);
    pthread_exit(NULL);
}

//redirects the perror() instead of printing to the terminal
//the output is redirected to a file
void error_redirect(char *err) {
    int file_fd = open("success_error.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (file_fd == -1) {
        perror("\nFile opening failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(file_fd, STDERR_FILENO) == -1) {
        perror("\ndup2() failed");
        exit(EXIT_FAILURE);
    }
    perror(err);
    close(file_fd);
    close(STDERR_FILENO);
}

//redirects the printf() instead of printing to the terminal
//the output is redirected to a file
void success_redirect(char *success) {
    int file_fd = open("success_error.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (file_fd == -1) {
        perror("\nFile opening failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(file_fd, STDOUT_FILENO) == -1) {
        perror("\ndup2() failed");
        exit(EXIT_FAILURE);
    }
    printf("\n%s", success);
    close(file_fd);
}

//Used to search for modified files
int modified_files(char * file) {
    struct stat st;
    const char* filename = file;
    int result = stat(filename, &st);

    if (result == 0) {
        time_t modified_time = st.st_mtime;
        printf("%s was last modified at %s", filename, ctime(&modified_time));
    } else {
        printf("Failed to get file status for %s", filename);
    }
}


