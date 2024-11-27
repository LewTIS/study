#include <stdio.h>
void swap(int *a,int *b)
{
    int tmp=*a;
    *a=*b;
    *b=tmp;
}
void bubble_sort(int array[],int size)
{
    for(int i=0;i<size-1;++i)//冒n-1趟即可有序
    {
        for(int j=0;j<size-1-i;++j)//遍历数组进行冒泡，
        {
            if(array[j]>array[j+1])//升序
            {
                swap(&array[j],&array[j+1]);
            }
        }
    }
}

void select_sort(int array[],int size)
{  
    for(int i=0;i<size-1;++i)
    {
        int max=0;
        for(int j=0;j<size-i;++j)//每趟遍历未有序元素，找出最大的
        {
            if(array[j]>array[max])
            {
                max=j;
            }
        }
        swap(&array[max],&array[size-1-i]);//与数组末尾元素交换
    }
}

//前边元素已有序，将元素插入到一个有序序列中
void insert_sort(int array[],int size)
{
    for(int i=0;i<size;++i)//循环插入数组中的每个元素
    {
        int key=array[i];//当前待插入元素
        int end=i-1;//前面有序序列的最后一个元素位置
    //挨个与前边元素比较，若前边元素大于key，则将前边元素后移动一个位置
    //终止条件：1.若前边元素小于等于key，则找到key可插入的位置，为end+1
    //          2.若end为<=-1,那么说明前面没有元素了，将key作为首元素插入
        while(end>-1&&array[end]>key)
        {
            array[end+1]=array[end];
            end--;
        }
        array[end+1]=key;
    }
}
//三数取中
int get_mid_index(int array[],int left,int right)
{
    int mid=left+((right-left)>>1);
    if(array[left]<array[right-1])
    {
        if(array[left]>array[mid])
            return left;
        else if(array[mid]>array[right-1])
            return right-1;
        else
            return mid;
    }else{
        if(array[mid]>array[left])
            return left;
        else if(array[mid]>array[right-1])
            return mid;
        else
            return right-1;
        }
    
}

int partion(int array[],int left,int right)
{
    //1.确定基准值,这里以右边元素为基准值
    int keyIndex=get_mid_index(array,left,right);
    swap(&array[keyIndex],&array[right-1]);
    int key=array[right-1];
    int begin=left;
    int end=right-1;
    while(begin<end)
    {
        //begin从前往后找比key小的元素
        while(begin<end&&array[begin]<=key)
        {
            begin++;
        }
        //end从后往前找比key大的元素
        while(begin<end&&array[end]>=key)
        {
            end--;
        }
        //找到之后，交换两个元素
        if(begin<end)
            swap(&array[begin],&array[end]);
    }
    //退出循环，begin与end处于同一位置，交换key和end位置的元素
    //此时，key左边都比key小，右边都比key大
    swap(&array[right-1],&array[begin]);
    return begin;
}
void quick_sort(int array[],int left,int right)//左闭右开
{
    if(right-left<=1)//待排序区间只有一个元素，结束排序
        return;

    //将区间元素根据基准值分为两组，左边比key小，右边比key大
    int div=partion(array,left,right);
    quick_sort(array,left,div);//递归排左边
    quick_sort(array,div+1,right);//递归排右边

}


void adjust_down_heap(int array[],int size,int parent)//这里以建立大堆为例
{
    int child=parent*2+1;
    while(child<size)
    {
        if(child+1<size&&array[child]<array[child+1])//选择两个中较大的一个
            child+=1;

        if(array[child]>array[parent])//和父节点比较并交换
        {
            swap(&array[child],&array[parent]);
            parent=child;
            child=parent*2+1;
        }
        else
            return;
    }
}

void heap_sort(int array[],int size)
{
    //升序：建大堆
    //降序：建小堆
    //1.建堆-从最后一个非叶子结点建队(利用adjustdown)
    int lastnotleaf=(size-2)/2;
    for(int i=lastnotleaf;i>=0;--i)
    {
        adjust_down_heap(array,size,i);
    }
    //2.利用堆删除实现有序-堆顶与堆尾元素交换
    int end=size;
    while(end--)
    {
        swap(&array[0],&array[end]);
        //交换之后，从堆顶向下调整，堆顶为最大元素
        adjust_down_heap(array,end,0);
    }
}

void print_array(int array[],int size)
{
    for(int i=0;i<size;++i)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}
int main()
{
    
    int array[]={3,0,6,9,4,1,5,8,7,2};
    int size=sizeof(array)/sizeof(array[0]);
    print_array(array,sizeof(array)/sizeof(array[0]));
    //bubble_sort(array,sizeof(array)/sizeof(array[0]));
    //select_sort(array,sizeof(array)/sizeof(array[0]));
    //insert_sort(array,sizeof(array)/sizeof(array[0]));
    //quick_sort(array,0,size);
    heap_sort(array,sizeof(array)/sizeof(array[0]));
    print_array(array,sizeof(array)/sizeof(array[0]));

    return 0;
}