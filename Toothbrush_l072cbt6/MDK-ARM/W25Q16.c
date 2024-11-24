#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"

/*****************SPI函数配置********************/


/*引脚配置层*/

/**
  *函    数: SPI写SS引脚电平
	*参    数: val 协议层传入的当前需要写入SS的电平，范围0~1
	*返 回 值: 无
	*注意事项：此函数需要用户实现内容，当val为0时，需要置SS为低电平，当val为1时，需要置SS为高电平
*/
void SPI_W_SS(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_CS_GPIO_Port, W25Q16_CS_Pin, val);
}

/**
  *函    数: SPI写SCK引脚电平
	*参    数: val 协议层传入的当前需要写入SCK的电平，范围0~1
	*返 回 值: 无
	*注意事项：此函数需要用户实现内容，当val为0时，需要置SCK为低电平，当val为1时，需要置SCK为高电平
*/
void SPI_W_SCK(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_CLK_GPIO_Port, W25Q16_CLK_Pin, val);
}
/**
  *函    数: SPI写MOSI引脚电平
	*参    数: val 协议层传入的当前需要写入MOSI的电平，范围0~1
	*返 回 值: 无
	*注意事项：此函数需要用户实现内容，当val为0时，需要置MOSI为低电平，当val为1时，需要置MOSI为高电平
*/
void SPI_W_MOSI(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_DI_GPIO_Port, W25Q16_DI_Pin, val);
}
/**
  *函    数: SPI写MISO引脚电平
	*参    数: val 协议层传入的当前需要写入MISO的电平，范围0~1
	*返 回 值: 无
	*注意事项：此函数需要用户实现内容，当val为0时，需要置MISO为低电平，当val为1时，需要置MISO为高电平
*/
uint8_t SPI_R_MISO(void) {
    return HAL_GPIO_ReadPin(W25Q16_DO_GPIO_Port,W25Q16_DO_Pin);
}

/**
  *函    数: SPI初始化
	*参    数: 无
	*返 回 值: 无
	*注意事项：此函数需要用户实现内容，实现SS、SCK电平初始化
*/

void SPI_Init(void)
{
//模式0
    SPI_W_SS(1);//SS默认情况下为高电平
    SPI_W_SCK(0);//SCK默认情况下为低电平
}

/* 协议层 */


/*
起始条件：SS由高电平 -> 低电平
终止条件：SS由低电平 -> 高电平

*/
/**
  *函    数: SPI起始
	*参    数: 无
	*返 回 值: 无
*/
void SPI_Start() {
    //SS置为低电平，开始时序
    SPI_W_SS(0);
}
/**
  *函    数: SPI终止
	*参    数: 无
	*返 回 值: 无
*/
void SPI_Stop() {
    // SS置回高电平，终止时序
    SPI_W_SS(1);
}

/**
  *函    数: SPI传输交换一个字节，使用SPI模式0
	*参    数: 无
	*返 回 值: 无
  *注意事项：该操作为掩码操作，不改变原来数据
*/

uint8_t SPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;					//定义接收的数据，并赋初值为0x00，此处必须赋初值0x00
	
	for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
	{
		SPI_W_MOSI(ByteSend & (0x80 >> i));		//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
		SPI_W_SCK(1);								//拉高SCK,上升沿移除数据
		if (SPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	//读取MISO数据,并存储到ByteReceive变量
																//当MISO为1时,置变量指定位为1，当MISO为0时，不做处理，指定为默认的初始值0
		SPI_W_SCK(0);								//拉低SCK,下降沿移入数据
	}
	
	return ByteReceive;								//返回接收到的一个字节数据
}

/*****************W25Q16函数配置********************/

/**
  *函    数: W25Q16初始化
	*参    数: 无
	*返 回 值: 无
*/
void W25Q16_Init(void)
{
	SPI_Init();					//先初始化底层SPI
}

/**
  *函    数: 读取W25Q16 设备ID号
	*参    数: MID 工厂ID，使用输出参数的形式返回
	*参    数: DID 设备ID，使用输出参数的形式返回
	*返 回 值: 无
*/
void W25Q16_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q16_JEDEC_ID);			//交换发送读取ID的指令
	*MID = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//交换接收MID,通过输出参数返回
	*DID = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//交换接收DID  ，一次交换只能交换8位，先交换高8位，后交换低8位
	*DID <<= 8;															//高8位移到高位
	*DID |= SPI_SwapByte(W25Q16_DUMMY_BYTE);	//或上 交换接收DID的低8位,通过输出参数返回
	SPI_Stop();								//SPI停止
}

/**
  *函    数: W25Q16 写使能
	*参    数: 无
	*返 回 值: 无
*/
void W25Q16_WriteEnable(void)
{
	SPI_Start();								      //SPI起始
	SPI_SwapByte(W25Q16_WRITE_ENABLE);//交换发送写使能的指令
	SPI_Stop();												//SPI终止
}
/**
  *函    数: W25Q16 写失能
	*参    数: 无
	*返 回 值: 无
*/
void W25Q16_WriteDisable(void)
{
	SPI_Start();								      //SPI起始
	SPI_SwapByte(W25Q16_WRITE_DISABLE);//交换发送写失能的指令
	SPI_Stop();												//SPI终止
}


/**
  *函    数: W25Q16 等待忙
	*参    数: 无
	*返 回 值: 无
*/

void W25Q16_WaitBusy(void)
{
	uint32_t Timeout;
	SPI_Start();																//SPI起始
	SPI_SwapByte(W25Q16_READ_STATUS_REGISTER_1);//交换发送读状态寄存器1的指令
	Timeout = 100000;														//给定超时计数时间
	while ((SPI_SwapByte(W25Q16_DUMMY_BYTE) & 0x01) == 0x01)//循环等待忙标志位
	{
		Timeout --;								  //等待时，计数值自减
		if (Timeout == 0)						//自减到0，等待超时
		{
			/*超时的错误处理代码，添加到此处*/
			break;								//跳出等待,跳出While循环
		}
	}
	SPI_Stop();								//SPI终止
}

/**
  *函    数: W25Q16 页编程
	*参    数: Address 页编程的起始地址，范围：0x000000~0x7FFFFF
	*参    数: DataArray 用于写入数据的数组
	*参    数: Count 要写入数据的数量
	*返 回 值: 无
*/

void W25Q16_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q16_WriteEnable();						//写使能
	
	SPI_Start();										//SPI起始
	SPI_SwapByte(W25Q16_PAGE_PROGRAM);		//交换发送页编程的指令
	SPI_SwapByte(Address >> 16);					//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);						//交换发送地址15~8位
	SPI_SwapByte(Address);								//交换发送地址17~0位
	for (i = 0; i < Count; i ++)					//循环Count次
	{
		SPI_SwapByte(DataArray[i]);					//依次在起始地址后写入数据
	}
	SPI_Stop();														//SPI终止
	
	W25Q16_WaitBusy();										//等待忙  ，事后等待，稳定，但效率没有事前等待高
}
/**
  *函    数: W25Q16 扇区擦除(4KB)，总共有512个扇区
	*参    数: Address 指定擦除的地址，范围：0x000000~0x7FFFFF
	*返 回 值: 无
*/

void W25Q16_SectorErase(uint32_t Address)
{
	W25Q16_WriteEnable();						//写使能
	
	SPI_Start();								  //SPI起始
	SPI_SwapByte(W25Q16_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	SPI_SwapByte(Address >> 16);				    //交换发送地址23~16位
	SPI_SwapByte(Address >> 8);							//交换发送地址15~8位
	SPI_SwapByte(Address);									//交换发送地址7~0位
	SPI_Stop();								 							//SPI终止
	
	W25Q16_WaitBusy();											//SPI终止
}
/**
  *函    数: W25Q16 全片擦除(4KB)，总共有512个扇区
	*参    数: 无
	*返 回 值: 无
*/

void W25Q16_ChipErase(void)
{
	W25Q16_WriteEnable();						//写使能
	
	SPI_Start();								  	//SPI起始
	SPI_SwapByte(W25Q16_CHIP_ERASE);//交换发送全片擦除的指令
	SPI_Stop();								 			//SPI终止
	
	W25Q16_WaitBusy();							//SPI终止
}

/**
  *函    数: W25Q16 读取数据
	*参    数: Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
	*参    数: DataArray 用于接收读取数据的数组，使用输出参数的形式返回
	*参    数: Count 要读取数据的数量 ，范围：0~0x800000
	*返 回 值: 无
*/

void W25Q16_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	SPI_Start();												//SPI起始
	SPI_SwapByte(W25Q16_READ_DATA);			//交换发送读取数据的指令
	SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);					//交换发送地址15~8位
	SPI_SwapByte(Address);							//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		DataArray[i] = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//依次在起始地址后读取数据
	}
	SPI_Stop();													//SPI终止
}



// 将32位拆为4个字节数据
void split_uint32_to_bytes(uint32_t data, uint8_t *bytes_array)
{
    bytes_array[0] = (data >> 24) & 0xff;//31~24位
    bytes_array[1] = (data >> 16) & 0xff;//23~16位
    bytes_array[2] = (data >> 8) & 0xff; //15~8位
    bytes_array[3] = data & 0xff;        //7~0位
}
// 将16位拆为2个字节数组
void split_int16_to_bytes(int16_t data, uint8_t *bytes_array)
{
    bytes_array[0] = (data >> 8) & 0xff; //15~8位
    bytes_array[1] = data & 0xff;        //7~0位
}
/**
  *函    数: 将时间戳和欧拉角数据存入W25Q16 
	*参    数: Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
	*参    数: timestamp 时间戳
	*参    数: pitch 俯仰角
	*参    数: yaw   偏航角
	*参    数: roll  横滚角
	*返 回 值: 无
*/

void store_data_to_W25Q16(uint32_t Address,uint32_t timestamp, int16_t pitch, int16_t yaw, int16_t roll)
{
	
	// 将时间戳拆为字节数组
	uint8_t timestamp_bytes[4];
	split_uint32_to_bytes(timestamp, timestamp_bytes);

	// 将欧拉角拆为字节数组
	uint8_t pitch_bytes[2];
	split_int16_to_bytes(pitch, pitch_bytes);
	uint8_t yaw_bytes[2];
	split_int16_to_bytes(yaw, yaw_bytes);
	uint8_t roll_bytes[2];
	split_int16_to_bytes(roll, roll_bytes);

	// 结合数据到一个字节数组
	uint8_t combined_data[10];
	for (int i = 0; i < 4; i++)
	{
			combined_data[i] = timestamp_bytes[i];
	}
	for (int i = 0; i < 2; i++)
	{
			combined_data[i + 4] = pitch_bytes[i];
			combined_data[i + 6] = yaw_bytes[i];
			combined_data[i + 8] = roll_bytes[i];
	}
	
	W25Q16_PageProgram(Address,combined_data, 10);//写入数据
	
	
}


