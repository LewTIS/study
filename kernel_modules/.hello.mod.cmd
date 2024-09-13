savedcmd_/home/linux/kernel_modules/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/linux/kernel_modules/"$$0) }' > /home/linux/kernel_modules/hello.mod
