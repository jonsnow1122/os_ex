//chat.c  聊天程序

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <termios.h>

int kbhit(void);

int main()
{
    int fd,i;
    char msg[101];  //装字符设备读取的文字
    
    fd=open("/dev/chardev0", O_RDWR|O_APPEND);  //打开字符设备
    if(fd==-1){
        fprintf(stderr, "can't openfile chardev0\n");
        exit(0);
    }

    

    while(1){
       
	if( kbhit() )   //有键盘输入
	{
	   char k[100]; //用来存储键盘输入的数据
	   char head[150]; //存放消息头信息和数据,表明是哪个进程写入的消息
	   int pid = getpid();

	   fgets(k,100,stdin);//从键盘输入
           //printf("有按键按下,开始读取键盘输入信息,并写入字符设备\n");
           //printf("我是进程%d,我写入的数据为:%s\n",pid,k);
           sprintf(head,"我是进程%d:%s",pid,k);   //将引号内容写入到head当中
           write(fd,head,strlen(head));//将head内容写入字符设备
        }

         for(i=0;i<101;i++)
             msg[i]='\0';
          
          read(fd,msg,100);
          if(strlen(msg) != 0 ) { //要有新数据才会显示在屏幕上
            //printf("读出的数据为:\n%s\n",msg);
            printf("%s\n",msg);         
          }
         
          sleep(2);
    }
    close(fd);
    return 0;
}

//非阻塞检测按键函数
int kbhit(void)
{ 
	struct termios oldt,newt;
	int ch,oldf; 
	tcgetattr(STDIN_FILENO, &oldt);
	newt=oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf=fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	  ch=getchar();
	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	  fcntl(STDIN_FILENO, F_SETFL, oldf);
	  if(ch != EOF) { 
	     ungetc(ch, stdin);
	     return 1;
	  } 
    return 0;
}



