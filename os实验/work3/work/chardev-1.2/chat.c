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
    char msg[101];
    
    fd=open("/dev/chardev0", O_RDWR|O_APPEND);
    if(fd==-1){
        fprintf(stderr, "can't openfile chardev0\n");
        exit(0);
    }

    

    while(1){
       
	if( kbhit() ) 
	{
	   char k[100]; //用来存储键盘输入的数据
	   char head[150]; //存放消息头信息和数据,表明是哪个进程写入的消息
	   int pid = getpid();

	   fgets(k,100,stdin);
           //printf("有按键按下,开始读取键盘输入信息,并写入字符设备\n");
           //printf("我是进程%d,我写入的数据为:%s\n",pid,k);
           int delta=0;
           if(k[0]=='@'){
           	delta++;
           	sprintf(head,"%c",k[delta]);
		while(k[delta+1]!=' '){
			sprintf(head,"%c",k[delta]);
			delta++;       
		}    
		sprintf(head," "); delta+=2;
	   }
           sprintf(head,"我是进程%d:%s",pid,k+delta);
           write(fd,head,strlen(head));//写入字符设备
        }

         for(i=0;i<101;i++)
             msg[i]='\0';
          
          read(fd,msg,100);
	  /*
	  if(msg[0]=='@'){
	  	int namel=0;
		int pid[10];
		for(int j=0;j++;j<10)
		{
		  pid[j]=0;
		}
		while(msg[namel]!=':'){
		  namel++;
		  pid[namel]=msg[namel];
		}
		namel--;
		
	  }*/
	  int delta=0;
          if(strlen(msg) != 0 ) { //要有新数据才会显示在屏幕上
            //printf("读出的数据为:\n%s\n",msg);
            if(msg[0]=='@'){
            	while(msg[delta]!=' ') delta++;
            	msg[delta]='\0';
            	int tmp=atoi(msg+1);
            	if(getpid()!=tmp) goto end;
            	delta++;
            }
            printf("%s\n",msg+delta);        end:;
          }
         
          sleep(2);
    }
    close(fd);
    return 0;
}

//非阻塞检测按键函数
int kbhit(void)
{ 
	struct termios oldt, newt;
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



