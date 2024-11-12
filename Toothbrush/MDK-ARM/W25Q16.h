#ifndef _W25Q16_H
#define _W25Q16_H


//对CS的写入
void SPI_W_SS(uint8_t val)   ;
//对SCK的写入
void SPI_W_SCK(uint8_t val)  ;
//对MOSI的写入
void SPI_W_MOSI(uint8_t val) ;
//对MISO的写入
uint8_t SPI_R_MISO(void)     ;
//SPI初始化
void SPI_Init(void) ;
//SPI起始
void SPI_Start()    ;
//SPI终止
void SPI_Stop()     ;
//SPI字节交换
uint8_t SPI_SwapByte(uint8_t ByteSend);
//W25Q16初始化
void W25Q16_Init(void);
//读取W25Q16 设备ID号
void W25Q16_ReadID(uint8_t *MID, uint16_t *DID);
//W25Q16 写使能
void W25Q16_WriteEnable(void);
//W25Q16 等待忙
void W25Q16_WaitBusy(void);
//W25Q16 页编程
void W25Q16_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
//W25Q16 扇区擦除(4KB)
void W25Q16_SectorErase(uint32_t Address);
//W25Q16 读取数据
void W25Q16_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
#endif


