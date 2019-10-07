#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUFF_SIZE 1024

void *recv_func(void* c_socket){
    int* client_socket = (int*)c_socket;
    char buff[BUFF_SIZE];
    while(1){
        recv(*client_socket,buff,BUFF_SIZE,0);
        printf("Msg:%s\n",buff);
    }
}

int main(int argc,char* argv[])
{
    printf("SOCKET_CLIENT\nIf you want to quit, insert \"quit!\" \n\n");
    if(argc != 3){
        printf("Parameter Error!\n");
        return -1;
    }

    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == client_socket ){
        printf("Socket Error!\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE];
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = (uint16_t)atoi(argv[2]);

    if( -1 == connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))){
        printf("connet() Error! \n");
        return -1;
    }
    pthread_t p_thread;
    pthread_create(&p_thread,NULL,recv_func,(void*)&client_socket);


    while(1){
        //printf("Sending Msg :");
        scanf("%s",buff);
        if( !strncmp(buff,"quit!",5)){
            break;
        }
        send(client_socket,buff,BUFF_SIZE,0);

    }

    printf("Program End !\n");
    //close(client_socket);
    return 0;



}
