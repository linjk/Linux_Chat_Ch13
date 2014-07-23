/******************************************************
 * File : Server.c
 * Des  : Chat - Server.c Base On BSD_Socket
 * Date : 2014-07-24
 * By   : LinJK
******************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <erron.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUFF 1024

int main(int argc, char *argv[], char *argv[]){
    int pid;
    int skckfd, new_fd;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;                  //端口 ， 监听队列大小
    char buf[MAXBUFF+1];
    
    if (argv[2]){
        myport = atoi(argv[2]);
    }
    else{
        myport = 7575;
    }
    if (argv[3]){
        lisnum = atoi(argv[3]);
    }
    else{
        lisnum = 3;
    }
    
    ////////////////////////////////////////////////////
    if ( (sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        perror("Create socket failed!\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Create socket Success.\n");
    }
    
    bzero(&my_addr, sizeof(my_addr));
    myaddr.sin_family = AF_INET;                  //地址协议
    myaddr.sin_port   = htons(myport);
    if (argv[1]){
        //将点分十进制字符串转换为网络顺序IP地址
        myaddr.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else{
        myaddr.sin_addr.s_addr = INADDR_ANY;
    }
    // =================== //
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("Bind IP address failed.\n");
        exit(EXIT_FIALURE);
    }
    else{
        printf("Bind IP address Success.\n");
    }
    // =================== //
    if (listen(sockfd, lisnum) == -1){
        perror("Set Server listen failed.\n");
        exit(EXIT_FIALURE);        
    }
    else{
        printf("Set Server listen Success.\n");
    }
    // =================== //
    printf("Server now is wating for client to connect ...\n");
    len = sizeof(struct sockaddr);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &len)) == -1){//阻塞等待连接
        perror("Server Accept failed.\n");
        exit(EXIT_FIALURE);               
    }
    else{
        printf("Server-->\n\8tGot Connection from : %s, port : %d, socked: %d\n", 
               inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
    }
    // ================New Thread To Process Client Msg================ //
    if ((pid=fork()) == -1){
        perror("fork() -> Created new thread for process client's quest failed.\n");
        exit(EXIT_FIALURE);                           
    }//if
    else if(pid == 0){//child process - use to send msg
        while(1){
            bzero(buf, MAXBUFF+1);
            printf("Please input message to send: ");
            fgets(buf, MAXBUFF, stdin);
            if(!strncasecmp(buf, "quit", 4)){
                printf("I will close the connect.\n");
                break;
            }//if
            len = send(new_fd, buf, strlen(buf)-1, 0);
            if (len < 0){
                printf("Message< %s >send failure! Errno code is: %d, errno msg is < %s >\n", 
                       buf, errno, strerror(errno));
                break;
            }//if
        }
    }//else ..pid == 0..
    else{
        while(1){
            bzero(bu MAXBUFF+1);
            len = recv(new_fd, buf, MAXBUFF, 0);
            if (len >0){
                printf("Message recv successful:< %s >, %dBytes recv\n", buf, len);
            }
            else if (len < 0){
                printf("Recv msg failure! Errno Code is: %d, errno msg is: < %s >\n", errno, strerror(errno));
                break;
            }
            else{
                printf("The other one close quit.\n");
                break;
            }
        }
    }//else ..pid != 0..
    close(new_fd);
    close(sockfd);    

    return 0;  
}


