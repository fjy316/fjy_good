#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"
#include "string.h"
#include "math.h"
#include "DS1302_to_Timestamp.h"


uint32_t Timestamp_DS1302=0;//DS1302实时时间转变为时间戳的变量
struct TX_TIMEData TX_TimeData;


/**
  *函    数: 读取时间戳
	*参    数: 无
	*返 回 值: 无
*/
void Read_Timestamp(void)
{
	DS1302_read_realTime();
	Timestamp_DS1302 = calculateTimestamp(TimeData.year,TimeData.month,TimeData.day,TimeData.hour,TimeData.minute,TimeData.second);		
}
/**
  *函    数: 读取时间
	*参    数: timestamp 时间戳
	*返 回 值: 无
*/
void Read_Tx_Time(uint32_t timestamp)
{
	TX_TimeData.year = calculateYear(timestamp);//年份
	TX_TimeData.month = calculateMonth(timestamp,TX_TimeData.year);//月份
	TX_TimeData.day = calculateDay(timestamp,TX_TimeData.year,TX_TimeData.month);//月份
	TX_TimeData.hour = calculateHour(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day);//月份
	TX_TimeData.minute = calculateMinute(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day,TX_TimeData.hour);//月份
	TX_TimeData.second = calculateSecond(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day,TX_TimeData.hour,TX_TimeData.minute);//月份
	
}




/*******************将实时时间转为时间戳*********************/
/**
  *函    数: 判断闰年函数 
	*参    数: year 年份值
	*返 回 值: 如果是闰年返回1，不是闰年返回0
*/
uint8_t isLeapYear(uint16_t year)
{
    if ((year % 4 == 0 && year % 100!= 0) || (year % 400 == 0))
    {
        return 1;
    }
    return 0;
}
/**
  *函    数: 计算每年的秒数
	*参    数: year 年份值
	*返 回 值: 每年的秒数
*/

uint32_t calculateYearSeconds(uint16_t year)
{
    uint32_t yearSeconds = 0;
    for (uint16_t y = 1970; y < year; y++)
    {
        if (isLeapYear(y)) 
        {
            yearSeconds += 31622400;//闰年 366天，共31622400秒
        }
        else
        {
            yearSeconds += 31536000;//平年 365天，共31536000秒
        }
    }
    return yearSeconds;
}
/**
  *函    数: 计算月份累计天数对应的秒数
	*参    数: year  年份值
	*参    数: month 月份值
	*返 回 值: 某个月的秒数
*/

uint32_t calculateMonthSeconds(uint8_t month, uint16_t year)
{
     uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year) && month == 2)
    {
        monthDays[2] = 29;
    }
    uint32_t monthSeconds = 0;
    for (uint8_t m = 1; m < month; m++)
    {
        monthSeconds += monthDays[m] * 86400;
    }
    return monthSeconds;
}

/**
  *函    数: 计算日对应的秒数
	*参    数: day  日数
	*返 回 值: 某日的秒数
*/

uint32_t calculateDaySeconds(uint8_t day)
{
    return (day - 1) * 86400;
}
/**
  *函    数: 计算时对应的秒数
	*参    数: hour  时数
	*返 回 值: 某个小时的秒数
*/

uint32_t calculateHourSeconds(uint8_t hour)
{
    return hour * 3600;
}
/**
  *函    数: 计算分对应的秒数
	*参    数: minute  分数
	*返 回 值: 某个分钟的秒数
*/

uint32_t calculateMinuteSeconds(uint8_t minute)
{
    return minute * 60;
}
/**
  *函    数: 计算秒对应的秒数
	*参    数: second  日数
	*返 回 值: 某个秒的秒数
*/
uint32_t calculateSecondSeconds(uint8_t second)
{
    return second;
}

/**
  *函    数: 将所有秒数加起来得到时间戳
	*参    数: year 年份值
	*参    数: month 月份值
	*参    数: day  日数
	*参    数: hour  时数
	*参    数: minute  分数
	*参    数: second  日数
	*返 回 值: 时间戳
*/

uint32_t calculateTimestamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    uint32_t timestamp = calculateYearSeconds(year);
    timestamp += calculateMonthSeconds(month, year);
    timestamp += calculateDaySeconds(day);
    timestamp += calculateHourSeconds(hour);
    timestamp += calculateMinuteSeconds(minute);
    timestamp += calculateSecondSeconds(second);
    return timestamp;
}
/*******************将时间戳转为实时时间********************/
/**
  *函    数: 通过时间戳计算年份
	*参    数: timestamp  时间戳
	*返 回 值: 年份
*/

uint16_t calculateYear(uint32_t timestamp)
{
    uint16_t year = 1970;
    uint32_t yearSeconds = 0;
    while (1)
    {
        if (isLeapYear(year))
        {
            yearSeconds += 31622400;
        }
        else
        {
            yearSeconds += 31536000;
        }
        if (yearSeconds > timestamp)
        {
            year--;
            break;
        }
        year++;
    }
    return year;
}

/**
  *函    数: 通过时间戳计算月份
	*参    数: timestamp  时间戳
	*参    数: year 年份值
	*返 回 值: 月份
*/
uint8_t calculateMonth(uint32_t timestamp, uint16_t year)
{
     uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
    {
        monthDays[2] = 29;
    }
    uint32_t remainingSeconds = timestamp;
    // 减去年份的秒数
    if (isLeapYear(year))
    {
        remainingSeconds -= calculateYearSeconds(year);
    }
    else
    {
        remainingSeconds -= calculateYearSeconds(year);
    }
    uint8_t month = 1;
    uint32_t monthSeconds = 0;
    while (1)
    {
        monthSeconds += monthDays[month] * 86400;
        if (monthSeconds > remainingSeconds)
        {
            break;
        }
        month++;
    }
    return month;
}
/**
  *函    数: 通过时间戳计算日份
	*参    数: timestamp  时间戳
	*参    数: year 年份值
	*参    数: month 月份值
	*返 回 值: 日份
*/

uint8_t calculateDay(uint32_t timestamp, uint16_t year, uint8_t month)
{
    uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year) && month == 2)
    {
        monthDays[2] = 29;
    }
    uint32_t remainingSeconds = timestamp;
    // 减去月份和年份的秒数
    if (isLeapYear(year))
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
    }
    else
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
    }
    uint8_t day = 1;
    uint32_t daySeconds = 0;
    while (1)
    {
        daySeconds += 86400;
        if (daySeconds > remainingSeconds)
        {
            break;
        }
        day++;
    }
    return day;
}
/**
  *函    数: 通过时间戳计算小时
	*参    数: timestamp  时间戳
	*参    数: year 年份值
	*参    数: month 月份值
	*参    数: day  日数
	*返 回 值: 小时
*/
uint8_t calculateHour(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day)
{
    uint32_t remainingSeconds = timestamp;
    // 减去日和月份和年份的秒数
    if (isLeapYear(year))
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
    }
    else
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
    }
    uint8_t hour = 0;
    uint32_t hourSeconds = 0;
    while (1)
    {
        hourSeconds += 3600;
        if (hourSeconds > remainingSeconds)
        {
            break;
        }
        hour++;
    }
    return hour;
}
/**
  *函    数: 通过时间戳计算分钟
	*参    数: timestamp  时间戳
	*参    数: year 年份值
	*参    数: month 月份值
	*参    数: day  日数
	*参    数: hour  时数
	*返 回 值: 分钟
*/
uint8_t calculateMinute(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour)
{
    uint32_t remainingSeconds = timestamp;
    // 减去小时、日和月份和年份的秒数
    if (isLeapYear(year))
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
        remainingSeconds -= calculateHourSeconds(hour);
    }
    else
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
        remainingSeconds -= calculateHourSeconds(hour);
    }
    uint8_t minute = 0;
    uint32_t minuteSeconds = 0;
    while (1)
    {
        minuteSeconds += 60;
        if (minuteSeconds > remainingSeconds)
        {
            break;
        }
        minute++;
    }
    return minute;
}
/**
  *函    数: 通过时间戳计算秒
	*参    数: timestamp  时间戳
	*参    数: year 年份值
	*参    数: month 月份值
	*参    数: day  日数
	*参    数: hour  时数
	*参    数: minute  分数
	*返 回 值: 秒
*/

uint8_t calculateSecond(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
    uint32_t remainingSeconds = timestamp;
    // 减去分钟、小时、日和月份和年份的秒数
    if (isLeapYear(year))
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
        remainingSeconds -= calculateHourSeconds(hour);
        remainingSeconds -= calculateMinuteSeconds(minute);
    }
    else
    {
        remainingSeconds -= calculateYearSeconds(year);
        remainingSeconds -= calculateMonthSeconds(month, year);
        remainingSeconds -= calculateDaySeconds(day);
        remainingSeconds -= calculateHourSeconds(hour);
        remainingSeconds -= calculateMinuteSeconds(minute);
    }
    return remainingSeconds;
}



