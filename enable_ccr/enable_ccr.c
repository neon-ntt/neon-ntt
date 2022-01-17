#include <linux/module.h>
#include <linux/kernel.h>

void enable_ccr(void *info)
{
   #ifdef __aarch64__
   asm volatile("MSR PMUSERENR_EL0, %0" ::"r"(1));
   asm volatile("MSR PMCR_EL0, %0" ::"r"(1));
   asm volatile("MSR PMCNTENSET_EL0, %0" ::"r"(0x80000000));
   #else
   asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
   asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(1 | 16));
   asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
   #endif
}

int init_module(void)
{
   on_each_cpu(enable_ccr, NULL, 0);
   printk(KERN_INFO "Userspace access to CCR enabled\n");
   return 0;
}

MODULE_LICENSE("CC0");
