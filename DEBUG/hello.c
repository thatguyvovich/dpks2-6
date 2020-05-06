#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/slab.h>

// MODULE DATA FOR `modinfo`
MODULE_AUTHOR("Volodymyr Rusinov <delta@zone05.net>");
MODULE_DESCRIPTION("Module that plays with memory.");
MODULE_LICENSE("Dual BSD/GPL");
// HELLO PARAMETER
static uint hello_param = 1;
module_param(hello_param, uint, S_IRUGO); // visibility: world, immutable
MODULE_PARM_DESC(hello_param, "An unsigned int, describes how many times \"Hello world\" will be said.");
// STRUCT
static LIST_HEAD(list_node_head);
struct hello_alloc{
	struct list_head node_head;
	int event_num;
	ktime_t event_time;
	ktime_t kill_time;
};



static int __init hello_init(void)
{
	if(hello_param == 0 || (hello_param  >= 5 && hello_param <= 10))
		printk(KERN_EMERG "WARNING! Invalid argument. Continuing job...");
	
	else if(hello_param > 10) {
		printk(KERN_EMERG "ERROR! Invalid arguemnt. Exiting job...");
		return -EINVAL;
	}

	else {
		// declaring variables here, since code isn't as pretty as it used to be
		int i = 0; 
		ktime_t start, end;
		struct hello_alloc* alloc_list; // if doesn't work, move kmalloc here

		for(i = 0; i < hello_param; i++){
			BUG_ON(hello_param > 10);

			start = ktime_get();

			printk(KERN_EMERG "Hello, world!\n");
			
			end = ktime_get();
			
			if(i != 3)
			{
				alloc_list = (struct hello_alloc*) kmalloc(sizeof(struct hello_alloc), GFP_KERNEL);
			}
			else
			{
				alloc_list = (struct hello_alloc*) 0;
			}
			
			*alloc_list = (struct hello_alloc){
				.event_num = i,
				.event_time = start,
				.kill_time = end
			};

			list_add_tail(&alloc_list->node_head, &list_node_head);
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_alloc *md, *tmp;

	printk(KERN_EMERG "Freeing memory. Displaying event times.");

	list_for_each_entry_safe(md, tmp, &list_node_head, node_head) {
		printk(KERN_EMERG "Freeing memory from event number %d", md->event_num);
		pr_info("%lld ns print time.\n", (long long int) (md->kill_time - md->event_time));
		list_del(&md->node_head);
		kfree(md);
	}

	BUG_ON(!list_empty(&list_node_head));
		
	printk(KERN_EMERG "Done. Unloading the module...");
}

module_init(hello_init);
module_exit(hello_exit);
