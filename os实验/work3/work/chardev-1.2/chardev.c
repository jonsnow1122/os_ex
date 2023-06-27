//chardev.c   字符驱动程序

#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#define MAX_DEVICES 10
#define MAJOR_NUM  101
#define MINOR_NUM 0
#define BUFFER_SIZE  1048576

struct _device_data{
    struct cdev  chardev;
    unsigned char *buffer;
    int npos;
}*mydata[MAX_DEVICES];

static ssize_t device_read(struct file *filp, char * buff, size_t len, loff_t* offset)
{
    int nlen=len;
    struct _device_data* pdb=filp->private_data;
    if(nlen>pdb->npos-*offset)
        nlen=pdb->npos-*offset;
    if(copy_to_user(buff, (pdb->buffer)+*offset, nlen))
        return -EFAULT;
    *offset += nlen;
    return nlen;
}

static ssize_t device_write(struct file *filp, const char * buff, size_t len, loff_t* offset)
{
    int nlen=len;
    struct _device_data* pdb=filp->private_data;
    if(nlen>BUFFER_SIZE-pdb->npos)
        nlen=BUFFER_SIZE-pdb->npos;
    if(nlen==0)
        return -ENOMEM;
    if(copy_from_user(&pdb->buffer[pdb->npos], buff, nlen))
        return -EFAULT;
    pdb->npos += nlen;
    return nlen;
}

static int device_open(struct inode *inode, struct file * filp)
{
    int nminor=iminor(inode);
    if(!mydata[nminor]->buffer)
        mydata[nminor]->buffer=(unsigned char *)vmalloc(BUFFER_SIZE);
    if(!mydata[nminor]->buffer)
        return -ENOMEM;
    filp->private_data=mydata[nminor];
    if((filp->f_flags&O_ACCMODE)==O_WRONLY)
        mydata[nminor]->npos=0;
    return 0;
}

static int device_release(struct inode* inode, struct file* filp)
{
/*
    int i;
    for(i=0; i<MAX_DEVICES; ++i){
        cdev_del(&mydata[i]->chardev);
        if(mydata[i]->buffer)
            vfree(mydata[i]->buffer);
        kfree(mydata[i]);
    }
*/  
  return 0;
}

struct file_operations fops = {
    .owner=THIS_MODULE,
    .read=device_read,
    .write=device_write,
    .open=device_open,
    .release=device_release,
};

static int device_init(void)
{
    int i, ndev, ret;
    printk(KERN_INFO "Loading " KBUILD_MODNAME "...\n");
    for(i=0; i<MAX_DEVICES; ++i) {
        mydata[i] =(struct _device_data*)kmalloc(sizeof(*mydata[0]), GFP_KERNEL);
        if(!mydata[i]){
            printk(KERN_EMERG "Can't allocate memory to mydata\n");
            return -ENOMEM;
        }
        mydata[i]->buffer=NULL;
        mydata[i]->npos=0;
        cdev_init(&mydata[i]->chardev, &fops);
        mydata[i]->chardev.owner=THIS_MODULE;
        ndev=MKDEV(MAJOR_NUM, MINOR_NUM+i);
        ret=cdev_add(&mydata[i]->chardev, ndev,1);
        if(ret){
            printk(KERN_EMERG "Can't register device[%d]!\n", i);
            return -1;
        }
    }
    return 0;
}

static void device_exit(void)
{
    int i;
    printk(KERN_INFO "Unloading " KBUILD_MODNAME "...\n");
    for(i=0; i<MAX_DEVICES; ++i){
        cdev_del(&mydata[i]->chardev);
        if(mydata[i]->buffer)
            vfree(mydata[i]->buffer);
        kfree(mydata[i]);
    }
}

module_exit(device_exit);
module_init(device_init);
MODULE_LICENSE("GPL");


