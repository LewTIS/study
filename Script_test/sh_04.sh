#!/bin/bash
if false;then

echo hello world #输出当行文本

#输出多行文本
echo "
    hello
    world
    "

#用-e参数使得echo解析转义字符
echo -e " hello \n world"

#单引号'':单引号内所有字符被解释为普通字符，原样输出
#双引号"":双引号内的字符会被解释为字符串，其中的特殊字符(* $ \等)保留其特殊意义
#反引号``:shell先执行被``括起来的内容(通常是linux命令)将结果替换到原命令中，现在多用$()取代


#参数变量
# ./sh_04.sh 参数1 参数2...
#获取参数：  
# $# 参数的数目
# $0 被运行的脚本名称
# $1 第一个参数
# $2 第二个参数
# $N 第N个参数
# $@ 全部的参数

# shift命令挪移变量值
echo "第一个参数是$1"
shift
echo "第一个参数是$1"
# 同样是 $1 ，通过 shift 使得它的值会变成原本是 $2 的值。
# 因此 shift 命令常被用在循环中，使得参数一个接一个地被处理

#定义数组
array=('v1' 'v2' 'v3')

#f访问数组 下标从0开始
echo ${array[2]}
echo ${array[*]}

#使用let命令实现运算
let "a=5"
let "b=3"
let "c=a+b"

echo "c=$c"



# read命令从标准输入读取文本存储到一个变量
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



#条件语句
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



# if else elif 
if [ -z $1 ];then
    echo "p1为空"
elif [ $1 = "lion" ];then
    echo "hello lion"
elif [ $1 = "frank" ];then
    echo "hello frank"
else 
    echo "不认识"
fi



# 条件测试
 # 测试字符串：
  # $string1 = $string2 表示两个字符串是否相等
  # $string1 != $string2 表示两个字符串是否不相等
  # -z $string 表示字符串string是否为空
  # -n $string 表示字符串string是否不为空
 # 测试数字
  # $num1 -eq $num2  表示两个数字是否相等
  # $num1 -ne $num2  表示两个数字是否不相等
  # $num1 -lt $num2  表示num1是否小于num2
  # $num1 -le $num2  表示num1是否小于或等于num2
  # $num1 -gt $num2  表示num1是否大于num2
  # $num1 -ge $num2  表示num1是否大于或等于num2
 # 测试文件：
  # -e $file  表示文件是否存在
  # -d $file  表示文件是否是一个目录
  # -f $file  表示文件是否是一个文件
  # -L $file  表示文件是否是一个符号链接
  # -r $file 表示文件是否可读(判断当前运行脚本的用户的读权限)
  # -w $file 表示文件是否可写
  # -x $file 表示文件是否可执行
  # $file1 -nt $file2 表示文件file1是否比file2更新(修改时间)
  # $file1 -ot $file2 表示文件file1是否比file2更旧

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



# case语句

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



# 循环语句
# while循环
while [ -z $response ]|| [ $response != 'yes' ]
do
    read -p "Say yes:" response
done



# for循环
# 遍历一组值

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


# 函数
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



#统计文件行数
line_in_file(){
    cat $1 | wc -l #wc -l(word count缩写) 统计文件行数，这里cat查看文件内容将输出传递给wc -l计算行数
    
}
#调用函数，并将程序执行程序传递的第一个参数作为函数参数,
#将函数输出(行数)赋值给变量line_num,需在调用函数时$(函数名 参数) 通过命令替换赋值

line_num=$(line_in_file $1)
echo "这个文件$1有$line_num行"



#函数的局部变量
local_global(){
    local var1='local 1'
    echo "val1 is $var1"
}
local_global



array=('dog' 'cat' 'pig')
#for index in ${array[@]}
#do 
#    echo "$index "
#done
size=${#array[@]} #计算数组元素数 @表示数组中所有元素
echo $size
for((i=0;i<size;++i))
do
    echo ${array[$i]}
done

fi #(对应if false;then)


