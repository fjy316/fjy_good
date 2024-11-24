#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"

/*****************SPI��������********************/


/*�������ò�*/

/**
  *��    ��: SPIдSS���ŵ�ƽ
	*��    ��: val Э��㴫��ĵ�ǰ��Ҫд��SS�ĵ�ƽ����Χ0~1
	*�� �� ֵ: ��
	*ע������˺�����Ҫ�û�ʵ�����ݣ���valΪ0ʱ����Ҫ��SSΪ�͵�ƽ����valΪ1ʱ����Ҫ��SSΪ�ߵ�ƽ
*/
void SPI_W_SS(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_CS_GPIO_Port, W25Q16_CS_Pin, val);
}

/**
  *��    ��: SPIдSCK���ŵ�ƽ
	*��    ��: val Э��㴫��ĵ�ǰ��Ҫд��SCK�ĵ�ƽ����Χ0~1
	*�� �� ֵ: ��
	*ע������˺�����Ҫ�û�ʵ�����ݣ���valΪ0ʱ����Ҫ��SCKΪ�͵�ƽ����valΪ1ʱ����Ҫ��SCKΪ�ߵ�ƽ
*/
void SPI_W_SCK(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_CLK_GPIO_Port, W25Q16_CLK_Pin, val);
}
/**
  *��    ��: SPIдMOSI���ŵ�ƽ
	*��    ��: val Э��㴫��ĵ�ǰ��Ҫд��MOSI�ĵ�ƽ����Χ0~1
	*�� �� ֵ: ��
	*ע������˺�����Ҫ�û�ʵ�����ݣ���valΪ0ʱ����Ҫ��MOSIΪ�͵�ƽ����valΪ1ʱ����Ҫ��MOSIΪ�ߵ�ƽ
*/
void SPI_W_MOSI(uint8_t val) {
    HAL_GPIO_WritePin(W25Q16_DI_GPIO_Port, W25Q16_DI_Pin, val);
}
/**
  *��    ��: SPIдMISO���ŵ�ƽ
	*��    ��: val Э��㴫��ĵ�ǰ��Ҫд��MISO�ĵ�ƽ����Χ0~1
	*�� �� ֵ: ��
	*ע������˺�����Ҫ�û�ʵ�����ݣ���valΪ0ʱ����Ҫ��MISOΪ�͵�ƽ����valΪ1ʱ����Ҫ��MISOΪ�ߵ�ƽ
*/
uint8_t SPI_R_MISO(void) {
    return HAL_GPIO_ReadPin(W25Q16_DO_GPIO_Port,W25Q16_DO_Pin);
}

/**
  *��    ��: SPI��ʼ��
	*��    ��: ��
	*�� �� ֵ: ��
	*ע������˺�����Ҫ�û�ʵ�����ݣ�ʵ��SS��SCK��ƽ��ʼ��
*/

void SPI_Init(void)
{
//ģʽ0
    SPI_W_SS(1);//SSĬ�������Ϊ�ߵ�ƽ
    SPI_W_SCK(0);//SCKĬ�������Ϊ�͵�ƽ
}

/* Э��� */


/*
��ʼ������SS�ɸߵ�ƽ -> �͵�ƽ
��ֹ������SS�ɵ͵�ƽ -> �ߵ�ƽ

*/
/**
  *��    ��: SPI��ʼ
	*��    ��: ��
	*�� �� ֵ: ��
*/
void SPI_Start() {
    //SS��Ϊ�͵�ƽ����ʼʱ��
    SPI_W_SS(0);
}
/**
  *��    ��: SPI��ֹ
	*��    ��: ��
	*�� �� ֵ: ��
*/
void SPI_Stop() {
    // SS�ûظߵ�ƽ����ֹʱ��
    SPI_W_SS(1);
}

/**
  *��    ��: SPI���佻��һ���ֽڣ�ʹ��SPIģʽ0
	*��    ��: ��
	*�� �� ֵ: ��
  *ע������ò���Ϊ������������ı�ԭ������
*/

uint8_t SPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;					//������յ����ݣ�������ֵΪ0x00���˴����븳��ֵ0x00
	
	for (i = 0; i < 8; i ++)						//ѭ��8�Σ����ν���ÿһλ����
	{
		SPI_W_MOSI(ByteSend & (0x80 >> i));		//ʹ������ķ�ʽȡ��ByteSend��ָ��һλ���ݲ�д�뵽MOSI��
		SPI_W_SCK(1);								//����SCK,�������Ƴ�����
		if (SPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}	//��ȡMISO����,���洢��ByteReceive����
																//��MISOΪ1ʱ,�ñ���ָ��λΪ1����MISOΪ0ʱ����������ָ��ΪĬ�ϵĳ�ʼֵ0
		SPI_W_SCK(0);								//����SCK,�½�����������
	}
	
	return ByteReceive;								//���ؽ��յ���һ���ֽ�����
}

/*****************W25Q16��������********************/

/**
  *��    ��: W25Q16��ʼ��
	*��    ��: ��
	*�� �� ֵ: ��
*/
void W25Q16_Init(void)
{
	SPI_Init();					//�ȳ�ʼ���ײ�SPI
}

/**
  *��    ��: ��ȡW25Q16 �豸ID��
	*��    ��: MID ����ID��ʹ�������������ʽ����
	*��    ��: DID �豸ID��ʹ�������������ʽ����
	*�� �� ֵ: ��
*/
void W25Q16_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI_Start();								//SPI��ʼ
	SPI_SwapByte(W25Q16_JEDEC_ID);			//�������Ͷ�ȡID��ָ��
	*MID = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//��������MID,ͨ�������������
	*DID = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//��������DID  ��һ�ν���ֻ�ܽ���8λ���Ƚ�����8λ���󽻻���8λ
	*DID <<= 8;															//��8λ�Ƶ���λ
	*DID |= SPI_SwapByte(W25Q16_DUMMY_BYTE);	//���� ��������DID�ĵ�8λ,ͨ�������������
	SPI_Stop();								//SPIֹͣ
}

/**
  *��    ��: W25Q16 дʹ��
	*��    ��: ��
	*�� �� ֵ: ��
*/
void W25Q16_WriteEnable(void)
{
	SPI_Start();								      //SPI��ʼ
	SPI_SwapByte(W25Q16_WRITE_ENABLE);//��������дʹ�ܵ�ָ��
	SPI_Stop();												//SPI��ֹ
}
/**
  *��    ��: W25Q16 дʧ��
	*��    ��: ��
	*�� �� ֵ: ��
*/
void W25Q16_WriteDisable(void)
{
	SPI_Start();								      //SPI��ʼ
	SPI_SwapByte(W25Q16_WRITE_DISABLE);//��������дʧ�ܵ�ָ��
	SPI_Stop();												//SPI��ֹ
}


/**
  *��    ��: W25Q16 �ȴ�æ
	*��    ��: ��
	*�� �� ֵ: ��
*/

void W25Q16_WaitBusy(void)
{
	uint32_t Timeout;
	SPI_Start();																//SPI��ʼ
	SPI_SwapByte(W25Q16_READ_STATUS_REGISTER_1);//�������Ͷ�״̬�Ĵ���1��ָ��
	Timeout = 100000;														//������ʱ����ʱ��
	while ((SPI_SwapByte(W25Q16_DUMMY_BYTE) & 0x01) == 0x01)//ѭ���ȴ�æ��־λ
	{
		Timeout --;								  //�ȴ�ʱ������ֵ�Լ�
		if (Timeout == 0)						//�Լ���0���ȴ���ʱ
		{
			/*��ʱ�Ĵ�������룬��ӵ��˴�*/
			break;								//�����ȴ�,����Whileѭ��
		}
	}
	SPI_Stop();								//SPI��ֹ
}

/**
  *��    ��: W25Q16 ҳ���
	*��    ��: Address ҳ��̵���ʼ��ַ����Χ��0x000000~0x7FFFFF
	*��    ��: DataArray ����д�����ݵ�����
	*��    ��: Count Ҫд�����ݵ�����
	*�� �� ֵ: ��
*/

void W25Q16_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q16_WriteEnable();						//дʹ��
	
	SPI_Start();										//SPI��ʼ
	SPI_SwapByte(W25Q16_PAGE_PROGRAM);		//��������ҳ��̵�ָ��
	SPI_SwapByte(Address >> 16);					//�������͵�ַ23~16λ
	SPI_SwapByte(Address >> 8);						//�������͵�ַ15~8λ
	SPI_SwapByte(Address);								//�������͵�ַ17~0λ
	for (i = 0; i < Count; i ++)					//ѭ��Count��
	{
		SPI_SwapByte(DataArray[i]);					//��������ʼ��ַ��д������
	}
	SPI_Stop();														//SPI��ֹ
	
	W25Q16_WaitBusy();										//�ȴ�æ  ���º�ȴ����ȶ�����Ч��û����ǰ�ȴ���
}
/**
  *��    ��: W25Q16 ��������(4KB)���ܹ���512������
	*��    ��: Address ָ�������ĵ�ַ����Χ��0x000000~0x7FFFFF
	*�� �� ֵ: ��
*/

void W25Q16_SectorErase(uint32_t Address)
{
	W25Q16_WriteEnable();						//дʹ��
	
	SPI_Start();								  //SPI��ʼ
	SPI_SwapByte(W25Q16_SECTOR_ERASE_4KB);	//������������������ָ��
	SPI_SwapByte(Address >> 16);				    //�������͵�ַ23~16λ
	SPI_SwapByte(Address >> 8);							//�������͵�ַ15~8λ
	SPI_SwapByte(Address);									//�������͵�ַ7~0λ
	SPI_Stop();								 							//SPI��ֹ
	
	W25Q16_WaitBusy();											//SPI��ֹ
}
/**
  *��    ��: W25Q16 ȫƬ����(4KB)���ܹ���512������
	*��    ��: ��
	*�� �� ֵ: ��
*/

void W25Q16_ChipErase(void)
{
	W25Q16_WriteEnable();						//дʹ��
	
	SPI_Start();								  	//SPI��ʼ
	SPI_SwapByte(W25Q16_CHIP_ERASE);//��������ȫƬ������ָ��
	SPI_Stop();								 			//SPI��ֹ
	
	W25Q16_WaitBusy();							//SPI��ֹ
}

/**
  *��    ��: W25Q16 ��ȡ����
	*��    ��: Address ��ȡ���ݵ���ʼ��ַ����Χ��0x000000~0x7FFFFF
	*��    ��: DataArray ���ڽ��ն�ȡ���ݵ����飬ʹ�������������ʽ����
	*��    ��: Count Ҫ��ȡ���ݵ����� ����Χ��0~0x800000
	*�� �� ֵ: ��
*/

void W25Q16_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	SPI_Start();												//SPI��ʼ
	SPI_SwapByte(W25Q16_READ_DATA);			//�������Ͷ�ȡ���ݵ�ָ��
	SPI_SwapByte(Address >> 16);				//�������͵�ַ23~16λ
	SPI_SwapByte(Address >> 8);					//�������͵�ַ15~8λ
	SPI_SwapByte(Address);							//�������͵�ַ7~0λ
	for (i = 0; i < Count; i ++)				//ѭ��Count��
	{
		DataArray[i] = SPI_SwapByte(W25Q16_DUMMY_BYTE);	//��������ʼ��ַ���ȡ����
	}
	SPI_Stop();													//SPI��ֹ
}



// ��32λ��Ϊ4���ֽ�����
void split_uint32_to_bytes(uint32_t data, uint8_t *bytes_array)
{
    bytes_array[0] = (data >> 24) & 0xff;//31~24λ
    bytes_array[1] = (data >> 16) & 0xff;//23~16λ
    bytes_array[2] = (data >> 8) & 0xff; //15~8λ
    bytes_array[3] = data & 0xff;        //7~0λ
}
// ��16λ��Ϊ2���ֽ�����
void split_int16_to_bytes(int16_t data, uint8_t *bytes_array)
{
    bytes_array[0] = (data >> 8) & 0xff; //15~8λ
    bytes_array[1] = data & 0xff;        //7~0λ
}
/**
  *��    ��: ��ʱ�����ŷ�������ݴ���W25Q16 
	*��    ��: Address ��ȡ���ݵ���ʼ��ַ����Χ��0x000000~0x7FFFFF
	*��    ��: timestamp ʱ���
	*��    ��: pitch ������
	*��    ��: yaw   ƫ����
	*��    ��: roll  �����
	*�� �� ֵ: ��
*/

void store_data_to_W25Q16(uint32_t Address,uint32_t timestamp, int16_t pitch, int16_t yaw, int16_t roll)
{
	
	// ��ʱ�����Ϊ�ֽ�����
	uint8_t timestamp_bytes[4];
	split_uint32_to_bytes(timestamp, timestamp_bytes);

	// ��ŷ���ǲ�Ϊ�ֽ�����
	uint8_t pitch_bytes[2];
	split_int16_to_bytes(pitch, pitch_bytes);
	uint8_t yaw_bytes[2];
	split_int16_to_bytes(yaw, yaw_bytes);
	uint8_t roll_bytes[2];
	split_int16_to_bytes(roll, roll_bytes);

	// ������ݵ�һ���ֽ�����
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
	
	W25Q16_PageProgram(Address,combined_data, 10);//д������
	
	
}


