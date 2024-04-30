#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[])
{
    int my_sock;
    struct sockaddr_in serv_addr;
    
    if(argc != 3)
    {
        printf("%s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    my_sock = socket(PF_INET,SOCK_STREAM,0); //1번
    if(my_sock == -1)
        printf("socket error \n");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(my_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) //2번
        printf("connect error\n");
    int str_len;
    while (1)
    {
        
        char temp[15];
        char temp2[15];
        char name[50];
        int writen;

        printf("$");
        scanf("%s %s",temp,temp2);
        if(strcmp(temp,"cd")==0){
            if(strlen(temp2) == 2)
                if(chdir(temp2)!=0){
                    printf("경로 변경 실패\n");
                    printf("%s\n",getcwd(name,200));
                }else{
                    printf("경로 변경 성공\n");
                    printf("%s\n",getcwd(name,200));
                }
            else{
                if(chdir(temp2)!=0){
                    printf("경로 변경 실패\n");
                    printf("%s\n",getcwd(name,200));
                }else{
                    printf("경로 변경 성공\n");
                    printf("%s\n",getcwd(name,200));
                }
            }
        }
        else if(strcmp(temp,"cd")!=0){
            getcwd(name,200);
            writen = write(my_sock,temp,sizeof(temp)-1);
            if(writen == -1){
                printf("명령어 전송 실패\n");
            }
            printf("명령어 전송 성공\n");

            writen = write(my_sock,temp2,sizeof(temp2)-1);
            if(writen == -1){
                printf("명령어2 전송 실패\n");
            }
            printf("명령어2 전송 성공\n");
            
            writen = write(my_sock,name,sizeof(name)-1);
            if(writen == -1){
                printf("경로 전송 실패\n");
            }
            printf("경로 전송 성공\n");

            
            char message[2];
            str_len = read(my_sock,message,sizeof(message)-1); //3번
            if(str_len==-1)
                printf("대기중\n");
            if(strcmp(message,"GO")){
                FILE* fs;
                fs = fopen("made_by_fd1", "r");
                char str[100];
                printf("서버의 응답\n");
                while (feof(fs) == 0) {
                    char str[100];
                    fgets(str, 100, fs);
                    printf("%s \n", str);
                }
            }       
            // printf("서버에서 : %s \n", message);
        }
    }
    
    
    //close(my_sock); //4번
    //return 0;
}