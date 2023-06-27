#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>                                                                                                         
int pid1,pid2;     // 定义两个进程变量 
int main( ) {
    int fd[2]; 
    char InPipe[3000];               // 定义读缓冲区
    char c1='1',c2='2'; 
    pipe(fd);                                           // 创建管道 
    while((pid1 = fork( )) == -1);          // 如果进程1创建不成功,则空循环 
    if(pid1 == 0) {               // 如果子进程1创建成功,pid1为进程号 
        //在子进程1中
        //补充；                        // 锁定管道 
        //lockf(fd[1],1,0);
        //补充;                        //  分2000次每次向管道写入字符’1’ 
        for(int i = 0;i<2000;i++)
        {
            //sprintf(InPipe,1);
            write(fd[1],&c1,1);
	    sleep(0.1);
        }
        sleep(5);                                       // 等待读进程读出数据 
        //lockf(fd[1],0,0);
        //补充;                               // 解除管道的锁定 
        exit(0);                                 // 结束进程1 
    } 
    else { 
        while((pid2 = fork()) == -1);                          // 若进程2创建不成功,则空循环 
        if(pid2 == 0) { 
            //在子进程2中
            //lockf(fd[1],1,0);
            for(int i = 0;i<2000;i++)
            {
                write(fd[1],&c2,1);
		sleep(0.1);
            }
            //补充;                                        //  分2000次每次向管道写入字符’2’ 
            sleep(5); 
            //lockf(fd[1],0,0); 
            exit(0); 
        } 
        else { 
            //在父进程中
            wait(0);
            //补充;                                                  // 等待子进程1 结束 
            wait(0);                                              // 等待子进程2 结束 
            //补充;                                 // 从管道中读出4000个字符
            lockf(fd[0],1,0);
            read(fd[0], InPipe, 4000);
            InPipe[4000] = '\0';
            lockf(fd[0],1,0);
            //补充;                                                              //  加字符串结束符 
            printf("%s\n",InPipe);                                 // 显示读出的数据
            printf("---------------------------------------------\n");
            exit(0);                                                        // 父进程结束 
            } 
        }
} 

