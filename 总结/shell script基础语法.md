
```c
#!/bin/bash
if false;then

echo hello world  #输出当行文本

#输出多行文本
echo "
    hello
    world
    "

#用-e参数使得echo解析转义字符
echo -e " hello \n world"
```
---

- 单引号'':单引号内所有字符被解释为普通字符，原样输出
- 双引号"":双引号内的字符会被解释为字符串，其中的特殊字符(* $ \等)保留其特殊意义
- 反引号\``:shell先执行被``括起来的内容(通常是linux命令)将结果替换到原命令中，现在多用$()取代


### 参数变量
> ./sh_04.sh 参数1 参数2...
### 获取参数：  
- $# 参数的数目
- $0 被运行的脚本名称
- $1 第一个参数
- $2 第二个参数
- $N 第N个参数
----
### shift命令挪移变量值
```c
echo "第一个参数是\$1"
shift
echo "第一个参数是\$1"
```
**同样是 \$1 ，通过 shift 使得它的值会变成原本是 $2 的值。**
**因此 shift 命令常被用在循环中，使得参数一个接一个地被处理**

### 定义数组
```c
array=('v1' 'v2' 'v3')

#访问数组 下标从0开始
echo ${array[2]}
echo ${array[*]} 访问所有数组元素
```
### 使用let命令实现运算
```c
let "a=5"
let "b=3"
let "c=a+b"

echo "c=$c"
```
---


### read命令从标准输入读取文本存储到一个变量
```c
read oneName towName #一次给多个变量赋值，空格分开
echo "hello $oneName $towName!"

# -p参数表示“提示”
read -p "请输入姓名：" name
echo "hello $name!"

# -n参数可限制用户输入字符串最大长度
read -p "请输入姓名：" -n 5 name
echo "hello $name!"


# -t参数限定用户输入时间，超过时间不读取输入
read -p "请输入姓名：" -n 5 -t 4 name


# -s参数可以隐藏输入内容
read -p "请输入密码：" -s password
echo "密码是$password"
```


### 条件语句
```c
# if [ 条件测试 ] #条件测试作用必须要有空格
# then
#     ...
# fi
name="lion"
# 进行字符串比较时，在变量和比较操作符之间加上空格
if [ $name = "lion" ];then
    echo "hello $name!"
fi


# if else

if [ $1 = $2 ];then
    echo "p1=p2"
else
    echo "p1!=p2"
fi

# if elif 
if [ -z $1 ];then
    echo "p1为空"
elif [ $1 = "lion" ];then
    echo "hello lion"
elif [ $1 = "frank" ];then
    echo "hello frank"
else 
    echo "不认识"
fi
```
---

### 条件测试
 ##### 测试字符串：
  - \$string1 =\$string2 表示两个字符串是否相等
  - \$string1 != \$string2 表示两个字符串是否不相等
  - -z $string 表示字符串string是否为空
  - -n $string 表示字符串string是否不为空
 ##### 测试数字
  - \$num1 -eq \$num2  表示两个数字是否相等
  - \$num1 -ne \$num2  表示两个数字是否不相等
  - \$num1 -lt \$num2  表示num1是否小于num2
  - \$num1 -le \$num2  表示num1是否小于或等于num2
  - \$num1 -gt \$num2  表示num1是否大于num2
  - \$num1 -ge \$num2  表示num1是否大于或等于num2
 #### 测试文件：
  - -e $file  表示文件是否存在
  - -d $file  表示文件是否是一个目录
  - -f $file  表示文件是否是一个文件
  - -L $file  表示文件是否是一个符号链接
  - -r $file 表示文件是否可读(判断当前运行脚本的用户的读权限)
  - -w $file 表示文件是否可写
  - -x $file 表示文件是否可执行
  - \$file1 -nt \$file2 表示文件file1是否比file2更新(修改时间)
  - \$file1 -ot \$file2 表示文件file1是否比file2更旧

```c
num1=20;
num2=20;

if [ $num1 -le $num2 ];then
    echo "num1<=num2"
else 
    echo "num1>=num2"
fi


file=$1
if [ ! -e $file ] || [ ! -f $file ];then
    echo "$file不存在"
else
    echo "$file存在"
fi
```
----

### case语句
```c
case $1 in
    "lion")
        echo "hello lion"
    ;;
    "frank"|"frank1") # 这里逻辑或为单|
        echo "hello frank"
    ;;
    *)
        echo "不认识"
    ;;
esac
```


### 循环语句
#### while循环
```c
while [ -z $response ]|| [ $response != 'yes' ]
do
    read -p "Say yes:" response
done
```



#### for循环
##### 遍历一组值
```c
for animal in dog cat pig
do 
    echo "$animal"
done

array=('dog' 'cat' 'pig')
for index in "${array[@]}"
do 
    echo "$index "
done


# 遍历ls命令的执行结果
listfile=`ls`
for file in $listfile
do
    echo "$file"
done



# 借助seq的for循环
for i in `seq 1 10`
do 
    echo $i
done
```
---

### 函数
```c
# 函数名(){    #函数名后面括号不加任何参数
#    函数体
# }

print(){
    echo  "This is a function!"
}

# 传递参数
print2(){
    echo "hello $1"  #函数内部$1是函数调用传递的参数，不是调用程序传递的参数
}
print2 lion
print2 frank

# 函数返回值
print3(){
    echo "hello $1"
    return 1
}
print3 lion

echo "函数返回值为 $?" # $? 获取的是上一个函数的返回值

```
---

### 统计文件行数
```c
line_in_file(){
    cat $1 | wc -l      # wc -l(word count缩写) 统计文件行数，这里cat查看文件内容将输出传递给wc -l计算行数
    
}
#调用函数，并将程序执行程序传递的第一个参数作为函数参数,
#将函数输出(行数)赋值给变量line_num,需在调用函数时$(函数名 参数) 通过命令替换赋值

line_num=$(line_in_file $1)
echo "这个文件$1有$line_num行"

```


### 函数的局部变量
```c
local_global(){
    local var1='local 1'
    echo "val1 is $var1"
}
local_global
```
----

### 字符串替换：      
 - `${变量名#匹配规则}` 从变量开头进行规则匹配，删除变量值从开头到第一次匹配的最短部分
 - `${变量名##匹配规则}`  从变量开头进行规则匹配，删除变量值从开头到最后一次匹配的最长部分
 - `${变量名%匹配规则}` 从变量尾部进行规则匹配，删除从尾部到第一次匹配的最短部分
 - `${变量名%%匹配规则}` 从变量尾部进行规则匹配，删除从尾部到最后一次匹配的最长部分
 - `${变量名/旧字符串/新字符串}` 变量内容符合旧字符串，则第一个符合的旧字符串被替换为新字符串
 - `${变量名//旧字符串/新字符串}` 变量内容符合旧字符串，则全部符合的旧字符串被替换为新字符串
 ```c
var_1="I love you ,Do you love me"  
var=${var_1#*ov}  # e you,Do you love me   *ov:从左匹配任意长度序列直到遇到ov,若去掉*,只有开头匹配才会删除
var=${var_1##*ov}  # e me
var=${var_1%ov*}  # I love you ,Do you l
var=${var_1%%ov*}  # I l

var_2="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

var=${var_2/bin/BIN}  # /usr/local/BIN:/usr/bin:/bin:/usr/sbin:/sbin
var=${var_2//bin/BIN}  # /usr/local/BIN:/usr/BIN:/BIN:/usr/sBIN:/sBIN
```
---
### 计算字符串长度：
 - `${#string}`  `${#array[@]}` #计算数组元素数 @表示数组中所有元素
 - `expr length "$string"` 如果string内有空格则必须加双引号

 ```c
var_1="Hello world"

len=${#var_1} 
len=$(expr length "$var_1")
echo $len
```


### 获取子串在字符串中的索引位置(第一次出现的位置)
 - `expr index $string $substring`  从1开始计算索引位置
```c
var_1="quickstart start is an app"

ind=$(expr index "$var_1" start) # 6
ind=$(expr index "$var_1" uniq) #1 ,匹配到的是q，位置是1
ind=$(expr index "$var_1" fd)  # 没有匹配到任何字符，结果为0
echo $ind 
```


### 计算子串长度
- `expr match $string substr` 从头开始匹配子串，没有匹配到返回0，匹配到则返回匹配的子串长度
```c
var_1="quickstart is an app"

sub_len=$(expr match "$var_1" app) # 返回0，var_1开头不是app，不匹配
sub_len=$(expr match "$var_1" quic) # 4
sub_len=$(expr match "$var_1" quic.*) # 20
echo $sub_len
```

### 提取子串(bash中字符串索引从0开始)
```c
var_1="quickstartisanapp"

substr=${var_1:10} #isanapp 从下标为10的字符i开始直到末尾
substr=${var_1:10:2} # is,从下标为10的字符i开始提取长度为2
substr=${var_1: -5} #anapp,从字符串末尾匹配，提取长度为5，-5前需加空格，不加则需要使用括号
substr1=$(expr substr "$var_1" 10 5) #tisan 这种方式索引从1开始
echo $substr $substr1 
```
---
### 读取文本内容
```c
file="input.txt"
while read -r line
do
    echo $line
done < $file


read -r line < "input.txt"
echo $line
```
---
### 运算
 - $((...))
 - expr
 - let
 - bc
 - declare -i
#### $((...))
```c
echo $((2+2))
echo $((5/2))

i=0
echo $((i++)) # 0
echo $((++i)) # 2


echo $(((2+3)*4)) #使用圆括号改变运算顺序

echo $((0xff)) #16进制转换为10进制
echo $((16>>2)) #位运算
echo $((3>2)) #逻辑运算，表达式微针，返回1，否则返回0

a=0
echo $((a<1?1:0)) 三元运算符
```

#### expr \$num1 operator $num2
##### 操作符：
- `num1 | num2` -- num1不为空且非0，返回num1，否则返回num2
- `num1 & num2` -- num1不为空且非0，返回num1，否则返回0
- `num1 < num2` -- num1小于num2 返回1，否则返回0
- `num1 = num2` -- num1等于num2 返回1，否则返回0
- `num1 > num2` -- num1大于num2 返回1，否则返回0
- `num1 + num2` -- 求和
- `num1 - num2` -- 求差
- `num1 * num2` -- 求积
- `num1 / num2` -- 求商
- `num1 % num2` -- 求余

```c

num1=30;num2=50
echo $(expr $num1 \| $num2) #返回num1
echo $(expr $num1 \& $num2) #返回num1
echo $(expr $num1 \< $num2) #返回1
echo $(expr $num1 + $num2) #返回1
```

#### bc 支持浮点数运算
```c
echo "scale=2;23/5" | bc   #scale=2表示结果保留2位小树

#declare -i 命令 -i参数声明整数类型后，可以直接进行数学运算
declare -i val1=12 val2=5
declare -i result
result=val1*val2
echo $result
```
#### declare -可以声明一些特殊类型的变量，为变量设置一些限制，比如声明只读类型的变量和整数类型的变量。
 - `-r` :将变量设为只读
 - `-i` :将变量设为整数,被 -i声明后可以直接进行数学运算
 - `-a` :将变量定义为数组
 - `-f` :显示此脚本前定义过的所有函数及内容
 - `-F` :仅显示此脚本前定义过的函数名
 - `-x` 将变量声明为环境变量
 - 不带任何参数时，declare命令输出当前环境的所有变量，包括函数在内，等同于不带有任何参数的set命令。
```c
declare -i val1=12 val2=5
declare -i result
result=val1*val2
echo $result

declare -r bar=1 #声明为只读，不可更改
bar=2

declare -x foo=3 #等同于 export foo=3
```
---
### 命令替换-获取某一段命令的执行结果
 - \`command`
 - $(command)

### 获取系统的所有用户并输出
```c
#awk -F : '{print $1}' /etc/passwd
#cut -d ":" -f 1  cut处理cat输出 -d设置字段分隔符为: -f 1指定只打印每行第一段
for user in $(cat /etc/passwd | cut -d ":" -f 1)
do
    echo $user
done
```



### 获取今年已经过了多少天和周
```c
echo $(date +%Y-%m-%d) # 当前系统日期：2024-10-10
echo "This year have passed $(date +%j) days"  #+%j选项用于显示当前年份的第几天
echo "This year have passed $(($(date +%j)/7)) weeks"
```


### 定义数组
> `array=('v1' 'v2' 'v3')`

#### 输出数组内容
- `${array[@]}` 输出全部内容
- `${array[*]}` 也是输出全部内容
- `${array[1]}` 输出下标索引为1的内容

#### 获取数组长度
- `${#array[@]}` #获取数组内元素个数
- `${#array[2]}` #数组下标为2的元素长度
#### 数组赋值
- `array[0]="frank"`
- `array[4]="lion" `
#### 删除元素
- `unset array[2]` #删除下标为2的元素
- `unset array`  #删除整个数组
#### 数组遍历
```c
for v in ${array[@]}
do
     ...
done
```
---
### 文本三剑客- grep sed awk
#### grep
 - `-i` 忽略大小写，grep -i path /etc/profile
 - `-n` 显示行号，grep -n path /etc/profile
 - `-E` 搭配正则表达式，dmesg|grep -E "ERROR|FAILED"
 - `-o` 仅输出与正则表达式匹配的部分 dmesg|grep -Eo "ERROR|FAILED"
 - `-v` 仅显示搜索文本不在的行
 - `-r` 递归查找
```c
 grep -E path /etc/profile --> 完全匹配path
 grep -E ^path /etc/profile --> 匹配path开头的字符串
 grep -E [Pp]ath /etc/profile --> 匹配path或Path
```
---
#### sed  -stream Editor 的缩写，流编辑器，对标准输出或文件逐行进行处理
 - 模式空间：是 sed 用来存储和处理文本行的内部缓冲区。
 ##### 1.查找 -p --打印
 - 使用 -n 选项来抑制默认输出，只显示 p 命令指定的内容 ，
 - 不加-n，执行完后。sed在执行完一个编辑命令（如 p）后，会自动将模式空间的内容输出,因此一行会输出2次
 ```c
  #sed -n '/Adela/p' input.txt 打印input.txt中匹配Adela的行
  #sed -n 'p' input.txt  打印所有行
 ```
 ##### 2.修改
 ```c
  # sed 's/old/new/' input.txt  将input.txt中匹配old的所有行的第一个实例替换为new
  # sed 's/old/new/g' input .txt  将input.txt中匹配old的所有行的所有实例替换为new
  # sed 's/old/new/2g' input .txt  ...替换第二个匹配的实例到剩下所有
  # sed 's/old/new/ig' input .txt  将行内所有old忽略大小写，全部替换为new
  # sed '2,4s/old/new/' input.txt  在文件的第2到第4行中将匹配行的第一个 old 替换 为new
  ```
 ##### 3.删除
 ```c
  # sed 'd' input.txt  删除input.txt每一行
  # sed '2d' input.txt 删除第二行
  # sed '/pattern/d' input.txt  删除匹配pattern的所有行
  # sed '2,/pattern/d' input.txt 从第二行开始删除，直到匹配pattern(删除包含这行)
```
 ##### 4.增加
  - `'a'` 行后追加
  - `'i'` 行前追加
  - `'r'` 外部文件读入，行后追加
  - `'w'` 匹配行写入外部文件
 - `-i`选项表示直接修改文件内容
 ```c
    # sed -i '/Ada/a hello' input.txt 在input.txt的匹配Ada的行的下一行追加hello
    # sed -i '/Ada/i hello world' input.txt  行前追加
    # sed -i '/Alva/r dmesg_errors.txt' input.txt 在input.txt匹配Alva的下一行追加dmesg_errors.txt内容
    # sed -n '/Alva/w dmesg_errors.txt' input.txt 将匹配到Alva的所有行内容写入dmesg_errors.txt中(替换原内容)
```
 ##### 5.反向引用
```c
  # sed -i 's/A..a/&s/g' input.txt 匹配A开头a结尾的字符串，在其后面添加's'
```
---
#### awk -是一个文本处理工具，通常用于处理数据并生成结果报告
 ##### 语法格式
  - `awk 'BEGIN{}pattern{commands}END{}' file_name`
  -  `BEGIN{}` 正式处理数据之前执行
  - `pattern` 匹配模式
  - `{commands}` 处理命令
  - `END{}` 处理完所有匹配数据后执行
 ##### 内置变量
  - `$0` 整行内容
  - `$1-$n`  当前行的第1-n个字段
  - `NF` (Number Field)表示当前行的字段个数，也就是有多少列
  - `NR`  (Number Row)表示当前行的行号，从1开始计数
  - `FNR` (File Number Row)表示多文件处理时，每个文件行号单独计数，都是从1开始
  - `FS`  (Field Separator)表示输入行字段分隔符,不指定默认以空格或tab键划分
  - `RS`  (Row Separator)表示输入行分隔符，默认回车换行\n
  - `OFS` (Output Field Separator)表示输出行分隔符，默认回车换行
  - `ORS` (Output Row Separator)表示输出行分隔符，默认回车换行
  - `FILENAME`  当前输入的文件名
  - `ARGC`  命令行参数个数
  - `ARGV`  命令行参数数组
---
 ##### 样例：
 ```c
  # awk '{print $0}' input.txt  输出每一行的内容
  # awk 'BEGIN{FS=":"}{print $1}' input.txt  处理文本前先定义字段分隔符为冒号，然后输出每行第一个字段
  # awk 'BEGIN{FS=":"}{print $NF}' input.txt ...输出最后一列，即最后一个字段
  # awk 'BEGIN{RS=":"}{print $0}' input.txt  将行分隔符设置为':',将输入视为由冒号分隔的多个记录
  # awk 'BEGIN{ORS="-"}{print $0}' input.txt  设置行输出分隔符为'-',以'-'连接每一行记录
  # awk 'BEGIN{ORS="-";RS=":"}{print $0}' input.txt 
 ```
 ##### printf  -awk格式化输出
  - `%s` -- 打印字符串；
  - `%d` -- 打印10进制数；
  - `%f` -- 打印浮点数；
  - `%x` -- 打印16进制数；
  - `%o` -- 打印8进制数；
  - `%e` -- 打印数字的科学计数法格式；
  - `%c` -- 打印单个字符的 ASCII 码
  - 修饰符：`'-'`左对齐 `'+'`右对齐 `'#'` 显示八进制在前面加0，显示16进制在前面加0x 
  ```c
   #awk 'BEGIN{FS=":"}{printf "%s %s\n",$1,$2}' input.txt
   #awk 'BEGIN{FS=":"}{printf "%-8s %-3#o\n",$1,$2}' input.txt
 ```
---
 ##### 模式匹配
  ###### 1.按正则表达式匹配
  ```c
   #awk '/Ada/{print $0}' input.txt 匹配input.txt中包含Ada的所有行
   #awk '/^Ad/{print $0}' input.txt 匹配以Ad开头的行
 ```
  ###### 2.运算符匹配
   - `< > <= >= == != || && ! ~匹配正则表达式 !~不匹配正则表达式`
   ```c
   #awk 'BEGIN{FS=":"}$2>=3{print $0}' input.txt 以冒号分割字段，匹配第二个字段>=3的行
 #awk中的表达式 (BEGIN和END中都可正常执行表达式)
   #awk 'BEGIN{var=20;var1="aaa";print var,var1}'  输出20 aaa
   #awk 'BEGIN{n1=20;n2+=n1;print n1,n2}'  输出20 20
   #awk 'BEGIN{sum=0}/^$/{sum++}END{print sum}' /etc/services  计算空行数目，^为行开始，$为行结尾，^$匹配空行
   ```
 ##### awk中的条件语句
 ```c
   # if(条件表达式)
   #     {动作1}
   # else if(条件表达式)
   #     {动作2}
   # else
   #     {动作3}

   #awk 'BEGIN{FS=":"}{if($2>5&&$2<8) {print $0}}' input.txt 以冒号为分隔符，只打印第二个字段在5和8之间的行
 ```
 ---
 ##### awk中的循环语句
  - 创建文件：while.awk  执行awk -f while.awk 以脚本方式执行awk动作
  ```c
  #while.awk
    # BEGIN{sum=0
    #   while(i<=100){
    #      sum+=i
    #      i++
    #  } 
    #  print sum
    # }
  #for.awk   awk -f for.awk
    # BEGIN{sum=0
    #  for(i=0;i<=10;++i)
    #  {
    #      sum+=i
    #  }
    #  print sum
    # }
 ```
 ---
 ##### awk字符串函数
  - length(str)  计算长度  
    - `awk 'BEGIN{FS=":"}{print length($1)}' input.txt`
  - index(str1,str2)  返回在str1中查询到的str2的位置 
    - `awk   'BEGIN{FS=":"}{print index($1,"de")}' input.txt` 位置从1开始
  - tolower(str)  小写转换 
    - `awk 'BEGIN{print tolower("Ade")}'`
  - toupper(str)  大写转换 
    - `awk 'BEGIN{print toupper("Ade")}'`
  - split(str1,arr,fs)  按分割符fs分割str1，并保存到数组中 
    - `awk 'BEGIN{str="I have a dream";split(str,arr," ");for(a in arr) print arr[a]}' `
  - match(str,RE)  返回正则表达式匹配到的子串位置 
    - `awk 'BEGIN{str="I have 1 dream";print match(str,/[0-9]/)}'`
  - substr(str,m,n)  截取子串，从m个字符开始，截取n为。若n不指定，则默认截取到末尾
    - `awk 'BEGIN{str="I have 1 dream";print substr(str,4)}'`
  - sub(RE,ReqStr,str)  替换查找到的第一个子串
    - `awk 'BEGIN{FS=":"}{sub("Alva","a",$1);print $1}' input.txt`
  - gsub(RE,ReqStr,str)  替换查找到的所有子串
    - `awk 'BEGIN{FS=":"}{gsub("Alma","a",$1);print $1}' input.txt`
 ##### 选项option
   ###### `-v` 参数传递
- awk 的 BEGIN 中不能直接使用 shell 中定义的变量，需要通过 -v 参数进行传递
    `
    var=20 num=30  awk -v var1="$var" -v var2="$num" 'BEGIN{print var1,var2}'
`
###### `-f` 指定脚本文件 
  - `awk -f test.awk`
  ###### `-F` 指定分隔符 
  - `awk -F : '{print $2}' input.txt`等同于 `awk 'BEGIN{FS=":"}{print $2}' input.txt`

  




