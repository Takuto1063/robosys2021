//SPDX-License-Identifier:GPL-3.0:
//  Copyright (C) 2021 Suzuki Takuto and Ryuichi Ueda.

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h> 
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Suzuki takuto and Ryuichi Ueda");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static int led_gpio[5] = {25,22,23,26,21};

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
                char c;   //読み込んだ字を入れる変数
		int m;
		if(copy_from_user(&c,buf,sizeof(char)))
			  return -EFAULT;

		if(c == '0'){

			gpio_base[10] = 1 << 25;
		        gpio_base[10] = 1 << 22;
			gpio_base[10] = 1 << 23;
			gpio_base[10] = 1 << 26;
			gpio_base[10] = 1 << 21;

		}else if(c == '1'){

			gpio_base[7] = 1 << 25;

		}else if(c == '2'){

			gpio_base[7] = 1 << 22;

		}else if(c == '3'){

			gpio_base[7] = 1 << 23;

		}else if(c == '4'){

			gpio_base[7] = 1 << 26;

		}else if(c == '5'){

			gpio_base[7] = 1 << 21;

		}else if(c == '6'){

			for(m = 0; m < 3; m++){

				gpio_base[7] = 1 << 25;
				ssleep(1);
				gpio_base[10] = 1 << 25;
				ssleep(1);
				gpio_base[7] = 1 << 22;
				ssleep(1);
				gpio_base[10] = 1 << 22;
				ssleep(1);
				gpio_base[7] = 1 << 23;
				ssleep(1);
				gpio_base[10] = 1 << 23;
				ssleep(1);
				gpio_base[7] = 1 << 26;
				ssleep(1);
				gpio_base[10] = 1 << 26;
				ssleep(1);
				gpio_base[7] = 1 << 21;
				ssleep(1);
				gpio_base[10] = 1 << 21;
				ssleep(1);

			}
		}
			
		return 1; //読み込んだ文字数を返す（この場合はダミーの1）
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	    int size = 0;
	    char sushi[] = {0xF0,0x9F,0x8D,0xA3,0x0A}; //寿司の絵文字のバイナリ
            if(copy_to_user(buf+size,(const char *)sushi, sizeof(sushi))){
	          printk( KERN_INFO "sushi : copy_to_user failed\n" );
		  return -EFAULT;
	    }
		           size += sizeof(sushi);
			       return size;
}

static struct file_operations led_fops = {
	        .owner = THIS_MODULE,
		.write = led_write,
		.read = sushi_read
};

static int __init init_mod(void) //カーネルモジュールの初期化
{
	int retval;

	retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
	        printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));       
       	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
        if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE,"myled");   //ここから追加
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
	        return PTR_ERR(cls);
	}

	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	gpio_base = ioremap_nocache(0xfe200000, 0xA0); //Pi4の場合は0xfe200000

	int n;

	for(n = 0; n < 5; n++){

	const u32 led = led_gpio[n];
	const u32 led_index = led/10;
	const u32 led_shift = (led%10)*3;
	const u32 led_mask = ~(0x7 << led_shift);

        gpio_base[led_index] = (gpio_base[led_index] & led_mask) | (0x1 << led_shift);

	}

	return 0;
}

static void __exit cleanup_mod(void) 
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);     // マクロで関数を登録
module_exit(cleanup_mod);  // 同上
