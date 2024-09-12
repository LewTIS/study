



## 文件操作
### 文件格式 
**file命令，查看文件类型**
 - `file [选项] 文件名`
 > `file code.tar.bz2` 查看code.tar.bz2的文件类型
### 文件权限和所有权
 **chmod命令，修改文件权限**
 - `chmod <权限> <文件>`   rwx-->4 2 1
 > `chmod 755 test.c`或 `chmod u=rwx,og=rx test.c`   user、group、other权限为rwxr_xr_x   
 > `chmod u+x test.c`    

**chown命令，修改文件所有者**
 - `chown  <账号名称> <文件或目录>`
 - `chown -R <账号名称> <文件或目录>` 将该目录及其子目录及文件递归更改所有者
 > `chown root test.c` 将test.c文件的所有者更改为root  
 
**chgrp命令，修改文件所属组**
  - `chgrp <目标群组> <文件名称>`
  > `chgrp root test.c` 将该文件所属群组改为root   
### 创建文件
- touch命令，文件不存在时创建文件，存在时将已存在的文件或目录的时间标签更新为当前系统时间
- `touch <文件名>`
> `touch file.txt`   
### 查看文件
#### 连接并显示文件内容
**cat命令，连接并显示文件内容**
- cat file.txt 显示文本文件内容
> `cat -n a.c`   显示文件内容，并显示行号  
#### 分页查看文件内容
- more <文件名>
- less <文件名>
#### 显示文件开头部分
- head file_name  不见选项参数，默认显示第一行
> `head -n 10 file` 显示file文件的前10行  
#### 显示文件的结尾部分
- `tail <文件名>   不见选项参数，默认显最后一行`
> `tail -n 10 file` 显示file文件的后10行   
### 文本编辑器
- `nano file_name`
- `vi file_name`
- `vim file_name`
### 删除文件
**rm命令，永久性地删除指定文件或目录**
- `rm [选项] 文件或目录`
   - -f：强制删除，系统不在询问，直接删除
   - -i：与-f相反，删除之前系统会给出提示信息
   - -r：递归删除，可删除目录及包含的子目录和文件
### 移动文件
**mv命令，既可以在不同目录之间移动文件或目录，也可对文件和目录进行重命名**
- `mv  [选项]  源文件  目标文件/目标路径`
>`mv main.c main1.c` 修改文件名称，将名称修改为main1.c  
> `mv main.c ./file` 移动main.c文件到当前目录的file目录下
### 复制文件
**cp命令，用来复制文件和目录**
- `cp  [选项]  源文件(被复制文件)  目标路径`
    -	-i：若目标文件已经存在会询问，如果目标文件已经存在，则     会询问是否覆盖  不会复制文件权限和属性
	- -a:  会复制权限和属性
	- -r：递归复制，用于复制目录
	- -u：若目标文件和源文件有差异，则可以更新目标文件，可用于对文件的升级和备用
### 查找文件
**find命令，用于查找linux系统某个指定路径下的文件位置，并把文件的绝对路径输出到终端**  
- `find 路径 [选项] 文件名`  
> `find /usr -name "*.h"` 在/usr中查找所有以.h结尾的文件  
> `find /tmp/ -type d`  在/tmp中查找类型为目录的文件

**grep命令，用于查找文件中符合条件的字符串，并将符合的行显示出来**  
- `grep [选项] [搜寻内容] [搜寻路径/文件]`
> `grep  -n "for"  test.c` 查找test.c文件中查找"for"，并显示行号   
> `grep -r "example" /path/to/directory`,递归查找目录中的文件
### 查找命令路径
**which、whereis命令，查找系统中可执行文件的路径，用于确定某个命令是否存在以及该命令的完整路径**   
- `which [选项] 命令名`
- `whereis [选项] 命令名`
>`whereis ls`  查找ls命令的路径  
>`which ls` 
### I/O重定向
|  指令  |  说明  |
|  ----- | ------ |
|[指令1] **`\|`**[指令2]|将[指令1]的standard output作为[指令2]的standard input|
|**`>`** [file]|将standard output导入[file]，如果[file]已经存在，standard output内容会覆盖[file]|
|**`<`** [file]|将[file]作为standard input|
|**`>>`** [file]|将standard output导入[file]，若[file]已经存在，则standard output内容会附加到[file]的最后|

**范例**
> `echo "hello world" > test_file` 将字符串写入test_file   
> `A=$(< test_file)` 将test_file的内容储存到变量A
### 创建链接
>ln 命令，是英文link的缩写，中文具有链接的含义，该命令用于给文件创建链接，在Linux系统中，为文件创建链接的方式分为以下两种：
- 软链接：类似于 Windows 系统中给文件创建快捷方式，即产生一个特殊的文件，该文件用来指向另一个文件，此链接方式同样适用于目录。

- 硬链接：文件的基本信息都存储在inode中，而硬链接指的就是给一个文件的 inode 分配多个文件名，通过任何一个文件名，都可以找到此文件的inode，从而读取该文件的数据信息。

- 注意：软链接文件的源文件必须写成绝对路径，而不能写成相对路径（硬链接没有这样的要求），否则软链接文件会报错。
> `ln [选项] [源文件] [目标文件]`
> `-s`：建立symbolic link(软链接)  
> `-f`：删除现有捷径（如果存在），再建立新捷径  
> `ln  test1 test2`  

## 目录操作
### 打印路径
**pwd命令，显示用户当前所处的工作目录，把路径信息输出到终端**  
- pwd 该命令输出路径为绝对路径
- pwd -P 显示完整路径
### 获取路径
- basename /etc/sysconfig/network  获取最后的文件名 network
- dirname /etc/sysconfig/network 获取目录名称 /etc/sysconfig
### 查看目录
- `ls [选项] [目录名称]`  选项可以省略，目录名称也可以省略
    - ls -a #列出当前目录下所有内容, 含 . 隐藏文件
    - ls -i #显示目录下每个文件的节点信息, 文件的节点相当于文件在磁盘的索引
    - ls -l #列出目录下的详细信息 可以简化为 ll
### 切换目录
- `cd [选项] [目录名称]`  
>`cd`    //用户家目录  
>`cd -`  //刚刚去过的路径   
>`cd ~`  //用户家目录 对应的绝对路径   /home/xxx    xxx就是用户名   
>`cd ~用户名`  // 该用户的家目录   
>`cd .` //用户当前目录 此层目录   
>`cd ..` //相对当前所在目录的上一级目录   
### 创建目录
- `mkdir [-m or -p] [目录名]`  创建空目录
>  -m ：用于手动配置所创建目录的权限，而不再使用默认权限   
>  -p ：递归创建所有目录，如：`/home/gec/test/abc`，使用-p选项，会递归创建每个目录  

`mkdir -m 711 test1` 创建有权限的目录  
`mkdir -p a/b/c`  递归创建目录
### 压缩与解压缩
#### 压缩/解压缩
**gzip**
 - `gzip -v [filename]` 压缩 
 - `gzip -d [filename]` / `gunzip [filename]`解压缩
 > `gzip -v test.c`  将test.c文件压缩，压缩后为test.c.gz  
 >`gzip -d test.c.gz ` 解压缩
 >`gunzip test.c.gz` 解压缩  
- 若为文本文件，则可以使用zcat/zmore/zless查看压缩文件内容  

**bzip2**
- `bzip2 -v [filename]` 压缩
- `bzip2 -d [filename]` 解压缩
> `bzip2 -v test.c`  将test.c文件压缩，压缩后为test.c.bz2
>`bzip2 -d test.c.bz2 `  
- 同上，可使用bzcat/bzmore/bzless查看压缩文件内容

**xz**
- `xz -v [filename]` 压缩
- `xz -d [filename]` 解压缩
> `xz -v test.c`  将test.c文件压缩，压缩后为test.c.xz
>`xz -d test.c.xz `     

**zip**
- `zip  [filename]` 压缩
- `unzip [filename]` 解压缩
> `zip archive.zip file1 file2` 压缩多个文件
> `unzip archive.zip `
#### 打包
**tar命令，该命令的作用是打包/释放，以及压缩/解压缩，打包的意思是把文件统一打包但不作压缩处理，所以压缩和打包生成的包的大小不同**    

- `tar [选项] [目标文件] [源文件]`
> -c:创建一个新的打包文件  
> -x:从打包文件中提取文件  
> -f:指定要打包的文件名 //该选项必须放在所以所有选项的最后面   
> -z:使用gzip压缩创建tar文件  
> -j:使用bzip2压缩创建tar文件  

> `tar -cvf archive.tar file1 file2` 将file1 file2打包  
> `tar -xvf archive.tar` 解包  
> `tar -czvf archieve.tar.gz file1 file2` 打包并以gzip方式压缩  
> `tar -xzvf archieve.tar.gz` 解压压缩包
## 用户管理
### 用户切换
- `suodu su -` 默认切换成root管理员
- `sudo su [用户]` 切换用户，但是不切换工作目录
- `sudo su - username` 切换成username用户，且切换工作环境
- `sudo [命令]` 拥有部分管理员权限执行命令
- `exit` 超级用户退回为普通用户
### 用户信息查看
- `cat /etc/shadow` 查看账号数量及信息
- `cat /etc/passwd` 查看所有用户信息
- `id <用户名称>` 查看用户信息
- `who` 显示当前登录用户
### 用户创建
- `useradd -m -u 800 user1` 创建用户user1，uid为800，-m选项创建家目录 
- `useradd -m -G group2 user2` 创建用户user2，创建家目录，并将user2添加到群组group2中
> -g :指定gid  
> -u :指定uid
> -G :指定用户所属的附加组  
> -M :不要建立用户家目录
> -m :自动创建用户家目录
> -p :指定新用户的密码
> -r :建立一个系统账号
### 用户删除
- `userdel user2 -r` 删除user2及其家目录
### 修改用户信息
- `passwd <用户>` 修改用户密码
- `usermod -l [新名称] [原名称] -u [新id] -d [工作主目录] -p [新密码]`
- `usermod -l newuser1 user1 -u 1001 -d /home/user1` 将user1用户名修改为newuser1，用户id改为1001，用户主目录改为/home/user1 
- `usermod -g 1000 user01` 修改用户所属附加组
**usermod命令常用选项**

| 选项  |  作用  |
| ---   | ------ |
|-d|修改用户主目录|
|-e|修改账号的有限期限|
|-f|修改在密码过期后多少天关闭该账号|
|-g|修改用户所属组|
|-G|修改用户所属附加组|
|-l|修改账号名称|
|-u|修改用户id|
|-L|锁定用户密码，使密码无效|
|-U|解除密码锁定|
### 用户组操作
- 一个用户可以有多个附加组，但只能有一个基本组
- cat /etc/group 查看用户组的信息，用文本编辑器查看
- `group_name:group_passwd:group_id:group_menbers`  

|字段名|说明|
|-----| --- |
|group_name|用户组名|
|group_passwd|加密后的用户组密码|
|group_id|用户组id|
|group_members|逗号分隔开的组成员|
#### 用户组管理命令
- `groupadd [选项] [组名]`:用指定的组名称来建立新的组账号  

|选项|作用|
|-----| --- |
|-g|指定组id号，1000以上为普通|
|-o|允许设置相同组id的群组，不必唯一|
|-r|建立系统组账号，即组id低于499|
#### 修改用户组信息
- `groupmod -n [新组名] [旧组名]`
- `group -g [编号] [组名称]`  修改组编号
#### 删除用户组
- `userdel <要删除的用户名>` 删除用户
- `groupdel <组名>` 删除组，但先删除或踢出组员
#### 组成员变动
- `gpasswd <组名>` 修改组密码
- `gpasswd -a <用户名> <组名>` 向组内添加一个用户
- `gpasswd -d <用户名> <组名>` 向组内删除一个用户
- `gpasswd -M <用户1，用户2> <组名>` 把用户加入组，以逗号隔开
- `gpasswd -a user1 sudo` 将user1添加到sudo组里，拥有sudo权限
## 网络操作
### 配置网络
**ifconfig命令，该命令的作用是查看或设置系统ip地址，ifconfig并非系统自带命令，需要利用命令进行安装使用**  
- `sudo apt install ner-tools`
### 网络测试
**ping命令，该命令的作用是向指定主机发送要求回应的ICMP数据包，可以用来测试网络是否连通**
- `ping [IP地址]`
- `ping [主机域名]`
注意：一般而言，如果网络是畅通的，那么ping命令将会看到数据包返回的延迟时间，否则如果看不到延迟时间，那么网络就是不通的。
### 显示网络连接、路由表等
- `netstat -tuln` 显示监听的端口
### 下载文件
- `wget [url]`
## 软件管理
### apt管理器指令
**apt命令来安装软件包**
- `apt update` 更新软件源
- `apt upgrade` 更新所有已安装的包
- `apt install <pkg>` 安装软件包<pkg>，多个软件包用空格隔开
- `apt remove <pkg>` 移除软件包<pkg>
- `apt purge <pkg>` 移除软件包及配置文件
- `apt autoremove` 自动删除不再需要的软件包和依赖关系
- `apt-cache depends [软件包名]` 显示该软件包的依赖信息
- `apt-cache search [软件包名]` 搜索软件包
## 帮助手册
### help
- `[命令名] --help`
### man手册
- 编程开发的过程中，需要使用众多的系统指令、各类函数库、系统调用、各类系统脚本文件等，这些种类繁多、功能各异的信息，可以通过统一的man手册来获取第一手资料，man文档是帮助信息的权威通道，是大多数教程、书籍的知识来源。
- 由于 man 手册所需要罗列的帮助条目众多，因此 man 手册被妥善地分为9册，不同的帮助信息被放入对应的man分册中：

|手册编号|包含信息|
| --- |  ---|
1|Shell命令（默认已安装）
2|系统调用
3|库函数
4|特殊文件（通常出现在/dev目录下）
5|文件的特殊格式或协定（例如/etc/passwd的格式）
6|游戏
7|杂项（例如一些宏定义）
8|系统管理员命令（通常只能由管理员执行）
9|非标准内核例程
- `man strcpy` 查询strcpy函数
*查询某一分册的条目*
- `man 1 ls`
- `man 3 printf`
## 磁盘/文件系统
### 查看分区
**df命令，该命令的作用是查看系统分区信息，侧重在文件系统级别的磁盘占用方面**
- `df [选项]`
- `-h` 转换为倍数显示  

### 文件系统挂载与卸除
- `mount [文件] [挂载点]`
>- `mount /dev/sr0`  /data/cdrom 将sr0挂载到cdrom  
- `umount` 将装置文件卸除
> `umount [选项] [装置文件名或挂载点]`

>-f:强制卸除
-l：立刻卸除文件系统
-n：不更新 /etc/mtab 情况下卸除
>`umount /mnt/udisk` 操作完毕后安全卸载U盘
## 系统/进程/其它
### 查看系统内存使用情况
**显示Linux系统中空闲的、已用的物理内存及swap内存、被内核使用的buffer**
- `free -h`
### 列出指定目录下的所有内容
- `tree [选项] [目录]`

参数说明：
-  -a：显示所有文件
-  -d：只显示目录
-  -f：显示每个文件的绝对路径
-  -i：不显示树枝，常与-f配合使用
### 进程状态
**ps命令，用于获取Linux系统中各进程的当前状态参数**
- `ps  [选项]`
> `ps -l`:仅观察自己的bash相关程序  
> `ps -ef`：将系统上所有进程列出来  
> `ps -aux`：显示所有用户的进程
> `top`：动态观察程序的变化
> `pstree`：将运行的程序以树结构显示，可以看出程序之间的相关性
### 给某个进程发送信号
- `kill -l` 列出信号的名称
- `kill [信号] pid` 给某个进程发送信号

**常用信号**

|信号id|信号|含义|
| --- |--- |--- |
|1|SIGHUP|挂起信号，用于重新加载配置文件|
|2|SIGINT|中断进程，类似ctrl+c|
|9|SIGKILL|强制终止进程，无法被捕获和忽略|
|15|SIGTERM|以正常流程终止一个进程|
|18|SIGCONT|继续执行已停止的进程|
|19|SIGSTOP|暂停一个程序的运行，类似ctrl+z|
### 系统时间
**data命令，可以查看或者修改Linux系统日期和时间**
- `data`
- `data -s [日期时间]`  -s选项可以设置日期和时间
### 命令别名
**alias命令，用于创建和管理命令别名**
- `alias [别名]=[命令]`
> `alias l="ls-l"` 注意单次有效，若要重复有效，要写入启动文件
> `source .bashrc` 写入配置文件后，让配置文件命令立即生效  

**注意：alias命令设置的别名只在当前终端中有效，如果需要永久生效，需要将其写入到用户家目录下的配置文件.bashrc中，然后使用source命令使配置文件立即生效。一定在尾部进行追加。**
### 清除屏幕
 - `clear`
### 系统开关机
- `reboot` 重启电脑
- `poweroff` 关机
- `suspend` 进入休眠模式
### 查看硬件信息
**查看CPU架构信息**
- `lscpu` 查看CPU核心的编号和其它相关信息
- `cat /proc/cpuinfo | grep "processor"` 打印CPU的详细信息
- `nproc` 显示当前系统中可用的处理器数量
- `top或htop` 显示每个CPU核心的使用情况
## 交叉编译
### 编译程序
- `gcc example.c -o example`
- `arm-linux-gcc demo.c -o demo_arm`
### 运行
#### 设置cpu亲和性
**启动一个新进程并绑定到特定CPU核心**
- `taskset -c 0 ./my_program` 启动一个新进程并绑定到0号CPU核心
- `taskset -p <pid>` 显示当前进程的CPU亲和性

**绑定已经运行的进程到特定CPU核心**
- `taskset -cp 0 1234` 将pid为1234的进程绑定到CPU核心0上运行
### GCC（GNU Compiler Collection,GNU编译器套件）
#### 基本命令
**1.编译链接单个源文件**
- `gcc [source_file] -o [output_file]`
- output_file：编译链接成的可执行程序文件
- source_file.c：源文件

**2.编译并链接多个源文件**
- `gcc file1.c file2.c -o [output_file]`
- file1.c和file2.c：多个源文件
#### 常用选项
**1.指定目标架构**
在嵌入式开发中，通常需要为特定的目标架构编译代码
- `arm-linux-gnueabi-gcc`：指定目标架构的交叉编译器
> `arm-linux-gnueabi-gcc -o source_file.c output_file`  

**2.优化选项**
- -O0：不优化（默认）
- -O1：基本优化
- -O2：较高水平的优化
- -O3：最高级别的优化
- -Os：优化代码大小
>`gcc -O2 -o output_file source_file`   

**3.调试信息**  
- `-g`：生成调试信息
> `gcc -g -o output_file source_file.c`  

**4.预处理**  
- `-E`：仅运行预处理器
> `gcc -E source_file.c -o output_file.i`  

**5.指定库路径和库**  
- `-L`：指定库文件搜索路径（库所在路径）
- `-l`：指定链接的库（库名）
> `gcc -L/path/to/lib -lmylib -o output_file source_file.c`
**6.生成静态链接的可执行文件**
> `gcc -static -o my_program my_program.c`










 

























