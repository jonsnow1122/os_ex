#include <stdio.h> 
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int wait_flag;
void stop(int signum);
int main(){ 
    //定义两个进程号变量 
    int pid1, pid2;    
    //alarm(1);                                          
    //捕捉中断信号sig后执行function规定的操作。 
    signal(SIGINT,stop)//
    //signal(SIGQUIT,stop);//3,ctrl \
    //signal(SIGALRM,stop);//alarm函数
    //若创建子进程1不成功,则空循环
    while((pid1 = fork( )) == -1);                
    //子进程创建成功,pid1为进程号 
    //在父进程里
    if(pid1 > 0) {                               
        //创建子进程2 (与子进程1并列)
        while((pid2 = fork( )) == -1);             
        //在父进程里
        if(pid2 > 0) { 
            wait_flag = 1; 
            // 父进程等待5秒
           // sleep(5);                                            
            //收到中断信号后stop函数中将wait_flag置为0，再杀死子进程
            //signal(SIGALRM,stop);
            while(wait_flag == 1);
            
            // 杀死进程1发中断号16
            //kill的作用不是使进程结束，而是发送信号让子进程自己结束。
	    kill(pid1,16);	    
            //--------------------------
            // 杀死进程2发中断号17
            kill(pid2,17); 
                                  
            // 等待第1个子进程1结束的信号
	    printf("waiting");            
            wait(0);                                   
            // 等待第2个子进程2结束的信号
	    printf("waiting"); 
            wait(0);                                    
            printf("\n Parent process is killed !!\n");
            printf("-----------------------------------------\n");
            // 父进程结束  
            
            exit(0);                                                      
            
        } 
        //在pid2里
        else { 
            wait_flag = 1; 
            //补充-----------------------
            // 等待进程2被杀死的中断号17 
            signal(17,stop);
            //--------------------------                                                                                 
            while(wait_flag==1);
            printf("\n Child process 2 is killed by parent !!\n"); 
            
            exit(0);                                                                                                    
        }
    }     
    //在pid1里                                                                                                               
    else {                                                                                                               
        wait_flag = 1;                  
        // 等待进程1被杀死的中断号16                                                                                 
        signal(16,stop);                                                                                                                         
        while(wait_flag==1);
        printf("\n Child process 1 is killed by parent !!\n");                                                        
        exit(0); 
    }                                                                                                                    
}   
void stop(int signum) { 
    //补充-----------------------
    
    wait_flag = 0;
    
    printf("\n stop by %d \n",signum);        
    //--------------------------                                                                    
}  

