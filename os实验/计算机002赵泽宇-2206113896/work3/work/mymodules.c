#include <linux/init.h>                                  /*必须要包含的头文件*/
#include <linux/kernel.h>
#include <linux/module.h>                              /*必须要包含的头文件*/
 // 其他header信息                                            
static int mymodule_init(void)                           //模块初始化函数
{
	printk("hello, my module was loaded!\n");
	return 0;
}
static void mymodule_exit(void)                      //模块清理函数
{
	printk("goodbye, my module is unloaded.\n");
}  
module_init(mymodule_init);                             //注册初始化函数
module_exit(mymodule_exit);                           //注册清除函数
MODULE_LICENSE("GPL");                           //模块许可声明

