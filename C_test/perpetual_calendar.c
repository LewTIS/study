#include <stdio.h>

int calendar(int year,int month,int day)
{
    int days_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return (day 
            + (month > 2 ? (((year%4==0&&year%100!=0)||year%400==0) ? 1 : 0) : 0) 
            + (month > 1 ? days_month[0] : 0) 
            + (month > 2 ? days_month[1] : 0) 
            + (month > 3 ? days_month[2] : 0) 
            + (month > 4 ? days_month[3] : 0) 
            + (month > 5 ? days_month[4] : 0) 
            + (month > 6 ? days_month[5] : 0) 
            + (month > 7 ? days_month[6] : 0) 
            + (month > 8 ? days_month[7] : 0) 
            + (month > 9 ? days_month[8] : 0) 
            + (month > 10 ? days_month[9] : 0) 
            + (month > 11 ? days_month[10] : 0) 
            + 365 * (year - 1900)     //以1900年1月1日是星期一為基準
            + (year - 1900) / 4 
            - (year - 1900) / 100 
            + (year - 1900) / 400 
			+ (((year%4==0&&year%100!=0)||year%400==0) ? 0 : 1)//若是平年，+1
            ) % 7; //星期天為0，星期一為1...星期六為6
}
int acc_month[] =  /* accumulated days before last month */
{ 0,
  31,
  31+28,
  31+28+31,
  31+28+31+30,
  31+28+31+30+31,
  31+28+31+30+31+30,
  31+28+31+30+31+30+31,
  31+28+31+30+31+30+31+31,
  31+28+31+30+31+30+31+31+30,
  31+28+31+30+31+30+31+31+30+31,
  31+28+31+30+31+30+31+31+30+31+30};
  
int get_weekday(int year, int month, int day)
{
    return ((365*(year-1)+     /* 到去年為止, 每年 365 天累積的日子 */
                  ((month<=2) ? ((year-1)/4-(year-1)/100+(year-1)/400) : (year/4 - year/100 + year/400))+  /* 加上累積的潤日 */
                             acc_month[month-1]+ /* 加上累積到上個月的日子 */
                             day) % 7);          /* 加上這個月的日子後,  %7 */
}        

int main()
{
    int year,month,day=0;
    while(scanf("%d%d%d",&year,&month,&day)!=EOF)
    {
       int result = calendar(year,month,day);
       int result2 = get_weekday(year,month,day);
       printf("%d %d\n",result,result2);
    }
}