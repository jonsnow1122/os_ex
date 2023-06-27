#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define AP 10
#define PP 3
#define TOTAL_INSTRUCTION 20
//队列数据结构
int Queue[PP+1]={};
int head = 0;
int tail = 0;
//特殊的栈
int Stack[PP] = {};
int top = PP-1;
int bottom = PP-1;
//用于存储中间结果
int temp[PP][TOTAL_INSTRUCTION];
//检查pagecontrol是否还有空位
int isConEmpty(int* first_empty,int pagecontrol[],int control_num)
{
    int flag = 0;
    for(int iter = 0;iter<control_num;iter++)
    {
        if(pagecontrol[iter]==-1)
        {
            flag = 1;
            *first_empty = iter;
            break;
        }

    }
    return flag;
}
/*
* FIFO算法
* page[cur]是当前需要调入的页面
*/
void FIFO(int curpage,int page[],int pagecontrol[],int control_num)
{
    int first_empty = -1;
    //如果pagecontrol中有空位，则直接放入空位
    if(isConEmpty(&first_empty,pagecontrol,control_num))
    {
        pagecontrol[first_empty]=curpage;
        //该页面已在内存中
        page[curpage] = 1;
        //队列记录的是最先放入数据的位置而不是页面号
        Queue[tail]=first_empty;
        if((tail+1)%(PP+1) != head)
        {
            tail = (tail + 1)%(PP+1);
        }
        else
        {
            //根据队列的设置不会出现队列满的情况
            printf("队列已满！\n");
            exit(1);
        }
    }
    //如果没有空位，则替换最先
    else
    {
        page[pagecontrol[Queue[head]]]=0;
        pagecontrol[Queue[head]]=curpage;
        page[curpage] = 1;
        Queue[tail]=Queue[head];
        if((head+1)%(PP+1)!=tail)
        {
            head = (head+1)%(PP+1);
        }
        else
        {
            //按照队列的设置不会出现队列空
            printf("队列空！\n");
            exit(1);
        }
        if((tail+1)%(PP+1) != head)
        {
            tail = (tail + 1)%(PP+1);
        }
        else
        {
            //根据队列的设置不会出现队列满的情况
            printf("队列已满！\n");
            exit(1);
        }
    }
}
//LRU算法
void LRU(int curpage,int page[],int pagecontrol[],int control_num)
{
    int first_empty = -1;
    //先检查在不在pagecontrol里面
    for(int iter = 0; iter<PP;iter++)
    {
        if(pagecontrol[iter]==curpage)
        {
            int iter2;//renew the stack to ensure the newest list
            for(iter2 = 0;iter2<PP;iter2++)
            {
                if(Stack[iter2]==iter)
                break;
            }
            for(;iter2<PP-1;iter2++)
            {
                Stack[iter2]=Stack[iter2+1];
            }
            Stack[top]=iter;
            return;
        }
    }
    //如果pagecontrol中有空位，则直接放入空位
    if(isConEmpty(&first_empty,pagecontrol,control_num))
    {
        pagecontrol[first_empty]=curpage;
        //该页面已在内存中
        page[curpage] = 1;
        //链表记录的是最先放入数据的位置而不是页面号

        for(int iter = 0;iter<PP-1;iter++)
        {
            Stack[iter]=Stack[iter+1];
        }
        bottom--;
        Stack[top]=first_empty;
    }
    //如果没有空位，替换最近使用最少
    else
    {
        page[pagecontrol[Stack[bottom+1]]]=0;
        pagecontrol[Stack[bottom+1]]=curpage;
        page[curpage] = 1;
        
        int tmp = Stack[bottom+1];
        for(int iter = 0;iter<PP-1;iter++)
        {
            Stack[iter]=Stack[iter+1];
        }
        
        Stack[top]=tmp;
    }
}
int main()
{
    //队列初始化为-1
    for(int iter=0;iter<PP+1;iter++)
    {
        Queue[iter]=-1;
    }
    //栈初始化为-1
    for(int iter=0;iter<PP;iter++)
    {
        Stack[iter]=-1;
    }
    /*定义变量
    * page是页面
    * pagecontrol是内存分配的页面，初始化为-1
    * pageseq是页面使用的顺序
    */
    printf("-----------------------------------------------------\n");
    printf("本程序模拟两种替换算法：FIFO和LRU。\n");
    int page[AP];
    int pagecontrol[PP];
    for(int iter = 0;iter<PP;iter++)
    {
        pagecontrol[iter] = -1;
    }
    for(int iter = 0;iter<AP;iter++)
    {
        page[iter] = 0;
    }
    
    int pageseq[TOTAL_INSTRUCTION] = {};
    //为pageseq的每一个元素赋随机值
    srand((unsigned int)getpid());
    for(int iter = 0;iter<TOTAL_INSTRUCTION;iter++)
    {
        pageseq[iter]=rand()%AP;
    }

    //测试
    //int pageseq[TOTAL_INSTRUCTION] = {7,0,1,2,0,3,1,4,2,3,0,2,5,1,3,0,1,7,0,1};
    //替换算法类型，0为FIFO，1为LRU
    printf("可分配的物理块数量为：%d\n",PP);
    printf("页面访问顺序为：\n%d",pageseq[0]);
    for(int iter = 1;iter<TOTAL_INSTRUCTION;iter++)
    {
        printf(" %d",pageseq[iter]);
    }
    printf("\n");
    int algorithm_type = 0;
    printf("请输入替换算法类型：0为FIFO，1为LRU。\n");
    scanf("%d",&algorithm_type);
    switch(algorithm_type)
    {
        case 0: {printf("FIFO算法。\n");break;}
        case 1: {printf("LRU算法。\n");break;}
        default: {printf("不支持该种替换算法！\n");exit(1);}
    }
    //cur是当前页面序列下标
    int cur = 0;
    //diseffect是未命中次数
    int diseffect = 0;

    while(cur!=TOTAL_INSTRUCTION)
    {
        //如果序列当前页面在已经分配的页面中
        if(page[pageseq[cur]]==1)
            //FIFO什么都不用做
            //LRU要更新记录
        {
            if(algorithm_type==1)
            {
                LRU(pageseq[cur],page,pagecontrol,PP);
            }
        }
        //如果不在当前已经分配的页面中
        else
        {
            diseffect++;
            //使用FIFO算法
            if(algorithm_type == 0)
            {
                FIFO(pageseq[cur],page,pagecontrol,PP);
            }
            //使用LRU算法
            else
            {
                LRU(pageseq[cur],page,pagecontrol,PP);
            }
        }
        
        //记录pagecontrol的值便于输出
        for(int iter = 0;iter<PP;iter++)
        {
            temp[iter][cur]=pagecontrol[iter];
        }     
        //查看下一个待调用的页面
        cur++;
    }
    //计算命中率
    double hit_rate = 100*(1-((double)diseffect/TOTAL_INSTRUCTION));
    //输出相关信息
    printf("-----------------------------------------------------\n");
    printf("替换次序(-1表示空)：\n");
    for(int iter = 0; iter<PP;iter++)
    {
        printf("%d",temp[iter][0]);
        for(int iter2 = 1;iter2<TOTAL_INSTRUCTION;iter2++)
        {
            printf("\t%d",temp[iter][iter2]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------\n");
    printf("共访问页面次数，即TOTAL_INSTRUCTION的大小：%d\n",TOTAL_INSTRUCTION);
    printf("未命中次数，即diseffect为：%d\n",diseffect);
    printf("命中率，即1-diseffect / total_instruction*100%%为：%.2f%%\n",hit_rate);
    printf("-----------------------------------------------------\n");
    return 0;
}

