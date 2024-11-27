cmd_/home/linux/share/CODE/kernel_modules/modules.order := {   echo /home/linux/share/CODE/kernel_modules/hello.ko; :; } | awk '!x[$$0]++' - > /home/linux/share/CODE/kernel_modules/modules.order
