#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char* argv[])
{
    int serv_sock;
    int clint_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clint_addr;
    socklen_t clnt_addr_size;

    if(argc != 2)  
    {
        printf("%s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM,0); //1번
    if(serv_sock == -1)
        printf("socket error\n");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) //2번
        printf("bind error\n");
    if(listen(serv_sock,5)==-1) //3번
        printf("listen error\n");
    
    clnt_addr_size = sizeof(clint_addr);
    clint_sock = accept(serv_sock,(struct sockaddr*)&clint_addr,&clnt_addr_size); //4번
    if(clint_sock == -1)
        printf("accept error\n");

    while(1){
        char temp[15];//명령어 1
        char temp2[15];//명령어 2
        char name[100];//경로
        int str_len;
        int str_len2;
        int str_len3;
        int writen;
        pid_t pid1,ret;
        int status;
        int fd1, ret2;
        //받기
        str_len = read(clint_sock,temp,sizeof(temp)-1);
        
        if(str_len==-1)
            printf("실패\n");
        printf("클라이언트에서 받은 명령어 1 : %s \n", temp);

        str_len2 = read(clint_sock,temp2,sizeof(temp2)-1);
        if(str_len2==-1)
            printf("실패\n");
        printf("클라이언트에서 받은 명령어 2 : %s \n", temp2);


        str_len3 = read(clint_sock,name,sizeof(name)-1);
        if(str_len3==-1)
            printf("실패\n");
        printf("클라이언트에서 받은 경로: %s \n", name);

        if(strcmp(temp,"cd")!=0){
            if((pid1=fork()) > 0){
                printf("\npid1 : %d\n",pid1);
                ret = waitpid(pid1,&status,0);
                char re[] = "GO";
                write(clint_sock,re,sizeof(re)-1);
                if(WEXITSTATUS(status)==0){
                    printf("성공\n");
                }else{
                    printf("실패\n");
                }
            }else if (pid1 == 0){
                getcwd(name,200);
                char *argv[] = {temp,temp2,name,NULL};
                fd1 = open("made_by_fd1",O_RDWR | O_CREAT| S_IROTH);
                dup2(fd1,1);
                execv("/Users/ogyutae/Desktop/1926007/my",argv);
                close(fd1);
            }
        else
            printf("fail\n");
        }
    }
    //write(clint_sock, message, sizeof(message)); //5번
    //close(serv_sock); //6번
    //close(clint_sock);
    //return 0;
}