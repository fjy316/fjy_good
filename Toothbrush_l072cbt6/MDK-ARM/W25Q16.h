#ifndef _W25Q16_H
#define _W25Q16_H


//��CS��д��
void SPI_W_SS(uint8_t val)   ;
//��SCK��д��
void SPI_W_SCK(uint8_t val)  ;
//��MOSI��д��
void SPI_W_MOSI(uint8_t val) ;
//��MISO��д��
uint8_t SPI_R_MISO(void)     ;
//SPI��ʼ��
void SPI_Init(void) ;
//SPI��ʼ
void SPI_Start()    ;
//SPI��ֹ
void SPI_Stop()     ;
//SPI�ֽڽ���
uint8_t SPI_SwapByte(uint8_t ByteSend);
//W25Q16��ʼ��
void W25Q16_Init(void);
//��ȡW25Q16 �豸ID��
void W25Q16_ReadID(uint8_t *MID, uint16_t *DID);
//W25Q16 дʹ��
void W25Q16_WriteEnable(void);
//W25Q16 дʧ��
void W25Q16_WriteDisable(void);
//W25Q16 �ȴ�æ
void W25Q16_WaitBusy(void);
//W25Q16 ҳ���
void W25Q16_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
//W25Q16 ��������(4KB)
void W25Q16_SectorErase(uint32_t Address);
//W25Q16 ȫƬ����
void W25Q16_ChipErase(void);
//W25Q16 ��ȡ����
void W25Q16_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);


void split_uint32_to_bytes(uint32_t data, uint8_t *bytes_array);
void split_int16_to_bytes(int16_t data, uint8_t *bytes_array);
void store_data_to_W25Q16(uint32_t Address,uint32_t timestamp, int16_t pitch, int16_t yaw, int16_t roll);


#endif


