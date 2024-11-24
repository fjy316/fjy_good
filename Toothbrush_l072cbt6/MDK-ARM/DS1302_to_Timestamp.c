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


uint32_t Timestamp_DS1302=0;//DS1302ʵʱʱ��ת��Ϊʱ����ı���
struct TX_TIMEData TX_TimeData;


/**
  *��    ��: ��ȡʱ���
	*��    ��: ��
	*�� �� ֵ: ��
*/
void Read_Timestamp(void)
{
	DS1302_read_realTime();
	Timestamp_DS1302 = calculateTimestamp(TimeData.year,TimeData.month,TimeData.day,TimeData.hour,TimeData.minute,TimeData.second);		
}
/**
  *��    ��: ��ȡʱ��
	*��    ��: timestamp ʱ���
	*�� �� ֵ: ��
*/
void Read_Tx_Time(uint32_t timestamp)
{
	TX_TimeData.year = calculateYear(timestamp);//���
	TX_TimeData.month = calculateMonth(timestamp,TX_TimeData.year);//�·�
	TX_TimeData.day = calculateDay(timestamp,TX_TimeData.year,TX_TimeData.month);//�·�
	TX_TimeData.hour = calculateHour(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day);//�·�
	TX_TimeData.minute = calculateMinute(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day,TX_TimeData.hour);//�·�
	TX_TimeData.second = calculateSecond(timestamp,TX_TimeData.year,TX_TimeData.month,TX_TimeData.day,TX_TimeData.hour,TX_TimeData.minute);//�·�
	
}




/*******************��ʵʱʱ��תΪʱ���*********************/
/**
  *��    ��: �ж����꺯�� 
	*��    ��: year ���ֵ
	*�� �� ֵ: ��������귵��1���������귵��0
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
  *��    ��: ����ÿ�������
	*��    ��: year ���ֵ
	*�� �� ֵ: ÿ�������
*/

uint32_t calculateYearSeconds(uint16_t year)
{
    uint32_t yearSeconds = 0;
    for (uint16_t y = 1970; y < year; y++)
    {
        if (isLeapYear(y)) 
        {
            yearSeconds += 31622400;//���� 366�죬��31622400��
        }
        else
        {
            yearSeconds += 31536000;//ƽ�� 365�죬��31536000��
        }
    }
    return yearSeconds;
}
/**
  *��    ��: �����·��ۼ�������Ӧ������
	*��    ��: year  ���ֵ
	*��    ��: month �·�ֵ
	*�� �� ֵ: ĳ���µ�����
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
  *��    ��: �����ն�Ӧ������
	*��    ��: day  ����
	*�� �� ֵ: ĳ�յ�����
*/

uint32_t calculateDaySeconds(uint8_t day)
{
    return (day - 1) * 86400;
}
/**
  *��    ��: ����ʱ��Ӧ������
	*��    ��: hour  ʱ��
	*�� �� ֵ: ĳ��Сʱ������
*/

uint32_t calculateHourSeconds(uint8_t hour)
{
    return hour * 3600;
}
/**
  *��    ��: ����ֶ�Ӧ������
	*��    ��: minute  ����
	*�� �� ֵ: ĳ�����ӵ�����
*/

uint32_t calculateMinuteSeconds(uint8_t minute)
{
    return minute * 60;
}
/**
  *��    ��: �������Ӧ������
	*��    ��: second  ����
	*�� �� ֵ: ĳ���������
*/
uint32_t calculateSecondSeconds(uint8_t second)
{
    return second;
}

/**
  *��    ��: �����������������õ�ʱ���
	*��    ��: year ���ֵ
	*��    ��: month �·�ֵ
	*��    ��: day  ����
	*��    ��: hour  ʱ��
	*��    ��: minute  ����
	*��    ��: second  ����
	*�� �� ֵ: ʱ���
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
/*******************��ʱ���תΪʵʱʱ��********************/
/**
  *��    ��: ͨ��ʱ����������
	*��    ��: timestamp  ʱ���
	*�� �� ֵ: ���
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
  *��    ��: ͨ��ʱ��������·�
	*��    ��: timestamp  ʱ���
	*��    ��: year ���ֵ
	*�� �� ֵ: �·�
*/
uint8_t calculateMonth(uint32_t timestamp, uint16_t year)
{
     uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
    {
        monthDays[2] = 29;
    }
    uint32_t remainingSeconds = timestamp;
    // ��ȥ��ݵ�����
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
  *��    ��: ͨ��ʱ��������շ�
	*��    ��: timestamp  ʱ���
	*��    ��: year ���ֵ
	*��    ��: month �·�ֵ
	*�� �� ֵ: �շ�
*/

uint8_t calculateDay(uint32_t timestamp, uint16_t year, uint8_t month)
{
    uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year) && month == 2)
    {
        monthDays[2] = 29;
    }
    uint32_t remainingSeconds = timestamp;
    // ��ȥ�·ݺ���ݵ�����
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
  *��    ��: ͨ��ʱ�������Сʱ
	*��    ��: timestamp  ʱ���
	*��    ��: year ���ֵ
	*��    ��: month �·�ֵ
	*��    ��: day  ����
	*�� �� ֵ: Сʱ
*/
uint8_t calculateHour(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day)
{
    uint32_t remainingSeconds = timestamp;
    // ��ȥ�պ��·ݺ���ݵ�����
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
  *��    ��: ͨ��ʱ����������
	*��    ��: timestamp  ʱ���
	*��    ��: year ���ֵ
	*��    ��: month �·�ֵ
	*��    ��: day  ����
	*��    ��: hour  ʱ��
	*�� �� ֵ: ����
*/
uint8_t calculateMinute(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour)
{
    uint32_t remainingSeconds = timestamp;
    // ��ȥСʱ���պ��·ݺ���ݵ�����
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
  *��    ��: ͨ��ʱ���������
	*��    ��: timestamp  ʱ���
	*��    ��: year ���ֵ
	*��    ��: month �·�ֵ
	*��    ��: day  ����
	*��    ��: hour  ʱ��
	*��    ��: minute  ����
	*�� �� ֵ: ��
*/

uint8_t calculateSecond(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
    uint32_t remainingSeconds = timestamp;
    // ��ȥ���ӡ�Сʱ���պ��·ݺ���ݵ�����
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



