#include <linux/module.h> //module_init  module_exit定义

#include <linux/cdev.h> //struct cdev
#include <linux/fs.h> //struct struct file_operations

#include <linux/device.h> //struct class

#include <linux/ioport.h> //struct resource *
#include <linux/io.h> //ioremap
#include <linux/uaccess.h> //copy_from_user

#include <linux/gpio.h> //内核GPIO口标准接口

#define PAD_GPIO_C 2*32

#define PAD_GPIO_B 1*32

#if 0
	#define LED_ON _IO('L',0)
	#define LED_OFF _IO('L',1)
#else
	#define LED_ON _IOW('L',1,unsigned long)
	#define LED_OFF _IOW('L',0,unsigned long)
#endif
unsigned int dev_number;
unsigned int major = 0;  //如果主设备为0，则表示由系统分配一个空间的设备号
unsigned int minor = 0;
static struct class *gpio_dev_class = NULL;
static struct device *led_device = NULL;

//定义一个维护多个GPIO口的结构体
struct led_gpio {
	unsigned	gpio; //gpio口号
	const char	*label; //成功申请gpio口号，给它取的名字
};

static struct led_gpio gpio_dev_gpios[] = {
	{PAD_GPIO_C + 14,"C14"},{PAD_GPIO_C + 28, "C28"},{PAD_GPIO_C + 13, "C13"},
};
//定义文件操作集
int led_open(struct inode *node, struct file *file)
{
  return 0;  
}

//系统调用 ---- ioctl --- > 命令 
long led_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
	switch(cmd) //命令的类型
	{
		case LED_ON:
			switch(args)
			{
				case 14:
					gpio_set_value(gpio_dev_gpios[0].gpio,1);
					break;
				case 28:
					gpio_set_value(gpio_dev_gpios[1].gpio,1);
					break;
				case 13:
					gpio_set_value(gpio_dev_gpios[2].gpio,1);
			}
			break;
		case LED_OFF:
			switch(args)
			{
				case 14:
					gpio_set_value(gpio_dev_gpios[0].gpio,0);
					break;
				case 28:
					gpio_set_value(gpio_dev_gpios[1].gpio,0);
					break;
				case 13:
					gpio_set_value(gpio_dev_gpios[2].gpio,0);
			}
			break;
	}
    return 0;
}
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.unlocked_ioctl = led_ioctl,
};

//定义一个普通字符设备
static struct cdev gpio_cdev;
 
static int __init gpio_dev_init(void)
{
	int retval,i;
	 //申请设备号，并将设备号要注册到内核中：静态注册 和动态注册
	 if(major == 0) //动态申请
	 {
		 //int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
		//	const char *name)
		retval = alloc_chrdev_region(&dev_number,minor,1,"6818_dev_number"); 
	 }else{ //静态注册
		 //得到设备号
		 dev_number = MKDEV(major ,minor);
		 
		//int register_chrdev_region(dev_t from, unsigned count, const char *name)
		retval = register_chrdev_region(dev_number,1,"6818_dev_number");
	 }
	 
	 if(retval < 0)
	 {
		 //在驱动中，打印消息用printk int printk(const char *fmt, ...)
		 
		 printk("register number error\n");
		 //使用goto语句来出错处理
		 goto register_number_error;
	 }
	//初始化字符设备 void cdev_init(struct cdev *, const struct file_operations *);
	
	cdev_init(&gpio_cdev,&led_fops);
	
	//将字符设备增加到内核中 int cdev_add(struct cdev *p, dev_t dev, unsigned count)
	retval = cdev_add(&gpio_cdev,dev_number,1);
	if(retval < 0)
	{
		//对于系统而言，资源是有限的，如果申请出错了，要把之前申请成功的资源还回给系统
		 printk("cdev_add error\n");
		 //使用goto语句来出错处理
		 goto cdev_add_error;
	}
	//创建class  struct class * class_create(struct module *owner, const char *name)
	gpio_dev_class = class_create(THIS_MODULE,"gpio_dev_class");
	if(gpio_dev_class == NULL)
	{
		printk("class_create error\n");
		goto class_create_error;
	}
	//创建device 
	//struct device *device_create(struct class *class, struct device *parent,
	//		     dev_t devt, void *drvdata, const char *fmt, ...)
	led_device = device_create(gpio_dev_class,NULL,dev_number,NULL,"led_drv");
	if(led_device == NULL)
	{
		printk("device_create error\n");
		goto device_create_error;
	}
	//申请GPIO口
	for(i=0;i<3;i++){
		gpio_free(gpio_dev_gpios[i].gpio);//在申请之前，看是否有驱动占用该资源
		retval = gpio_request(gpio_dev_gpios[i].gpio, gpio_dev_gpios[i].label);
		if(retval < 0)
		{
			printk("gpio_request error\n");
		    goto gpio_request_error;
		}
		//设置模式
		gpio_direction_output(gpio_dev_gpios[i].gpio,0);
		
	}
	printk(KERN_ALERT "led_drv init\n");
	return 0; //正常返回
	gpio_request_error:
	while(i--)
	gpio_free(gpio_dev_gpios[i].gpio);
	device_destroy(gpio_dev_class, dev_number);
  
	device_create_error:
	class_destroy(gpio_dev_class);
	gpio_dev_class = NULL;
  
	class_create_error:
	cdev_del(&gpio_cdev);
  
	cdev_add_error:
	unregister_chrdev_region(dev_number, 1);//释放申请成功设备号
  
	register_number_error:
	return retval; //出错返回
}

static void __exit gpio_dev_exit(void)
{
    //正常删除驱动时，将驱动的资源全部释放
    int i = 3;
	while(i--)
	gpio_free(gpio_dev_gpios[i].gpio);
	device_destroy(gpio_dev_class, dev_number);
	class_destroy(gpio_dev_class);
	gpio_dev_class = NULL;
	cdev_del(&gpio_cdev);
	unregister_chrdev_region(dev_number, 1);
	printk(KERN_ALERT "led_drv exit\n");
}

module_init(gpio_dev_init);  //模块的入口  ---执行insmod时，调用                                       
module_exit(gpio_dev_exit);   //模块的出口 ---执行rmmod时，调用


MODULE_AUTHOR("RONGSY");//驱动的设计者
MODULE_DESCRIPTION("Led driver for 6818"); //驱动描述
MODULE_LICENSE("GPL");//驱动程序遵守的GPL协议