BEGIN{
    printf "%-10s %-10s\n","User","Total Records"
}

{
  # USER是一个数组，空格分隔后，第6个字段为用户名，第8个字段为records，
  # 每次从数组去除$6的值累加$8,获取重复用户的总Records
    USER[$6]+=$8 
}
END{
    for(u in USER)
    {
        printf "%-10s %-10d\n",u,USER[u]
    }
}