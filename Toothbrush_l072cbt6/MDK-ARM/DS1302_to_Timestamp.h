#ifndef _DS1302_TO_TIMESTAMP_H
#define _DS1302_TO_TIMESTAMP_H


struct TX_TIMEData
{
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  week;
};//创建TX_TIMEData结构体方便存储时间日期数据



uint8_t isLeapYear(uint16_t year);
uint32_t calculateYearSeconds(uint16_t year);
uint32_t calculateMonthSeconds(uint8_t month, uint16_t year);
uint32_t calculateDaySeconds(uint8_t day);
uint32_t calculateHourSeconds(uint8_t hour);
uint32_t calculateMinuteSeconds(uint8_t minute);
uint32_t calculateSecondSeconds(uint8_t second);
uint32_t calculateTimestamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void Read_Timestamp(void);

uint16_t calculateYear(uint32_t timestamp);
uint8_t calculateMonth(uint32_t timestamp, uint16_t year);
uint8_t calculateDay(uint32_t timestamp, uint16_t year, uint8_t month);
uint8_t calculateHour(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day);
uint8_t calculateMinute(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour);
uint8_t calculateSecond(uint32_t timestamp, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);

void Read_Tx_Time(uint32_t timestamp);

#endif


