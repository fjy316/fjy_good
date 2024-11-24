#ifndef _W25Q16_INS_H
#define _W25Q16_INS_H


//���ּĴ���
#define W25Q16_WRITE_ENABLE							          0x06
#define W25Q16_WRITE_DISABLE						    			0x04
#define W25Q16_READ_STATUS_REGISTER_1							0x05
#define W25Q16_READ_STATUS_REGISTER_2							0x35
#define W25Q16_WRITE_STATUS_REGISTER							0x01
#define W25Q16_PAGE_PROGRAM							    			0x02
#define W25Q16_QUAD_PAGE_PROGRAM					  			0x32
#define W25Q16_BLOCK_ERASE_64KB						  			0xD8
#define W25Q16_BLOCK_ERASE_32KB						  			0x52
#define W25Q16_SECTOR_ERASE_4KB						  			0x20
#define W25Q16_CHIP_ERASE							      			0xC7
#define W25Q16_ERASE_SUSPEND					 	    			0x75
#define W25Q16_ERASE_RESUME							    			0x7A
#define W25Q16_POWER_DOWN							      			0xB9
//#define W25Q16_HIGH_PERFORMANCE_MODE							0xA3
//#define W25Q16_CONTINUOUS_READ_MODE_RESET			    0xFF
//#define W25Q16_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Q16_MANUFACTURER_DEVICE_ID							0x90
#define W25Q16_READ_UNIQUE_ID											0x4B
#define W25Q16_JEDEC_ID														0x9F
#define W25Q16_READ_DATA													0x03
#define W25Q16_FAST_READ													0x0B
#define W25Q16_FAST_READ_DUAL_OUTPUT							0x3B
#define W25Q16_FAST_READ_DUAL_IO									0xBB
#define W25Q16_FAST_READ_QUAD_OUTPUT							0x6B
#define W25Q16_FAST_READ_QUAD_IO									0xEB
//#define W25Q16_OCTAL_WORD_READ_QUAD_IO						0xE3
 
#define W25Q16_DUMMY_BYTE													0xFF
 
 
 
//�������е����ݴ���W25Q16��
//#define ARRAY_SIZE 33  //�����С
//uint8_t dataArray[ARRAY_SIZE];  // Ҫ�洢����������
//uint32_t currentAddress = 0;  // ��ǰ��W25Q16�Ĵ洢��ַ����ʼ��Ϊ0
//#define W25Q16_FLASH_SIZE 0x200000  //W25Q16оƬ����
////��W25Q16�ж�ȡ���ݴ�������
//#define Read_ARRAY_SIZE 33  //�����С
//uint8_t Read_dataArray[Read_ARRAY_SIZE];  // Ҫ�洢����������
//uint32_t Read_currentAddress = 0;  // ��ǰ��W25Q16�Ĵ洢��ַ����ʼ��Ϊ0
////��W25Q16�ж�ȡ���ݴ�������
//#define Printf_ARRAY_SIZE 15  //�����С
//uint32_t Printf_dataArray[Read_ARRAY_SIZE];  // Ҫ�洢����������
//uint32_t Read_FinalAddress = 0x0000000+33*20;  // ���ݴ洢�����һ����ַ


#define W25Q16_FLASH_SIZE 0x200000  //W25Q16оƬ����

#define W25Q64_FLASH_SIZE 0x800000  //W25Q64оƬ����



//���õ�����ȡ��ַ�������ֵ��Χ���ڣ������Զ�ȡ
#define Max_Set_Read_ARRAY 33*50+0x000000
 
 

void Show_W25Q16_Data(void);
void Write_Data_Once_second(void);
void Read_data_to_printf(void);
uint8_t  CheckAngleStability(void);//��鵱ǰ�Ƕ��Ƿ��ȶ�



#endif


