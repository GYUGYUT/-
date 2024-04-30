//
//  0530.c
//  0516
//
//  Created by 오규태 on 2022/05/30.
//

//
//  main.c
//  myls
//
//  Created by 오규태 on 2022/04/18.
//
//myls -ila
//getopt() 함수 이용
//Readdir
//stat
//file1, 접근권한, 마지막 수정시간, 바이트 단위 파일크기, inode번호 순서
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <dirent.h>
#define BUF_SIZE 256
#define HOUR(SEC) (SEC/3600)
#define MINUTE(SEC) ((SEC%3600)/60)
#define SECOND(SEC) (SEC%3600%60)

int main(int argc, char *argv[]){
    int opt;
    int ops_num = 0;
    char buffer[BUF_SIZE];
    while((opt = getopt(argc, argv, "ila")) != -1)
        //l or i or a or " " 조건에 대해 판별
        {
            switch(opt)
            {
                case 'i': // 1 증가
                    ops_num += 1;
                    break;
                case 'l': // 2 증가
                    ops_num += 2;
                    break;
                case 'a': // 3 증가
                    ops_num += 3;
                    break;
                default:
                    ops_num += 0;
            }
        }
    if(getcwd(buffer, BUF_SIZE) == NULL)
        exit(1);
    DIR *dir_ptr = NULL;
    struct dirent *file = NULL;
    //printf("%s\n",buffer);
    if((dir_ptr = opendir(buffer)) == NULL){//dir_ptr은 현재 디렉토리의 첫번째 항목을 가리킨다.
        exit(1);
    }
    while((file = readdir(dir_ptr))!=NULL){//struct dirent 포인트형의 디렉터리 항 반환
        if (ops_num == 0){//ls
            long long int len = strlen(file->d_name);
            if(len == 1 && file->d_name[0] == '.'){
                continue;
            }
            if(len>=2){
                if(file->d_name[0] == '.' || file->d_name[1] == '.')
                    continue;
            }
            printf("%s\n",file->d_name);
            //printf("%s",strcat("./",file->d_name));

        }
        else if(ops_num == 1){//ls -i
            long long int len = strlen(file->d_name);
            if(len == 1 && file->d_name[0] == '.'){
                continue;
            }
            if(len>=2){
                if(file->d_name[0] == '.' || file->d_name[1] == '.')
                    continue;
            }
            printf("%llu ",file->d_ino);
            printf("%s \n",file->d_name);
        }
        else if (ops_num == 2){//ls -l
            long long int len = strlen(file->d_name);
            if(len == 1 && file->d_name[0] == '.'){
                continue;
            }
            if(len>=2){
                if(file->d_name[0] == '.' || file->d_name[1] == '.')
                    continue;
            }
            struct stat finfo;
            if(stat(file->d_name,&finfo)==-1){
                fprintf(stderr,"Coudn't stat %s \n",file->d_name);
            }
            printf("%s %o %lld %u\n",file->d_name,finfo.st_mode,finfo.st_size,finfo.st_mtimespec);
        }
        else if (ops_num == 3){//ls -a
            printf("%s\n",file->d_name);
        }
        else if (ops_num == 4){//ls -ia
            printf("%llu ",file->d_ino);
            printf("%s\n",file->d_name);
        }
        else if (ops_num == 5){//ls -la
            struct stat finfo;
            if(stat(file->d_name,&finfo)==-1){
                fprintf(stderr,"Coudn't stat %s \n",file->d_name);
            }
            printf("%s %o %lld %u\n",file->d_name,finfo.st_mode,finfo.st_size,finfo.st_mtimespec);
        }
        else if (ops_num == 6){//ls -ila
            struct stat finfo;
            if(stat(file->d_name,&finfo)==-1){
                fprintf(stderr,"Coudn't stat %s \n",file->d_name);
            }
            printf("%s %o %llu %lld %u\n",file->d_name,finfo.st_mode,finfo.st_ino,finfo.st_size,finfo.st_mtimespec);
        }
        else{
            printf("실패");
        }
    }
    printf("—————성공——————\n");
    closedir(dir_ptr);
    return 0;
}
