#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2ca90791, "module_layout" },
	{ 0xae2ae519, "param_ops_int" },
	{ 0x38ccd1b1, "param_ops_charp" },
	{ 0xdb6ebebf, "sysfs_remove_file_ns" },
	{ 0x4a165127, "kobject_put" },
	{ 0xd7558de5, "sysfs_create_file_ns" },
	{ 0x92997ed8, "_printk" },
	{ 0x6fabae87, "kobject_create_and_add" },
	{ 0xaafb0b88, "kernel_kobj" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x37a0cba, "kfree" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x85df9b6c, "strsep" },
	{ 0x9166fada, "strncpy" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "28DAA79523B16261CF53C1F");
