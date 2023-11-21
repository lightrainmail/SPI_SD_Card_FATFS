//
// Created by Lightrain on 2023/11/19.
//

#ifndef DEMO_SPI_SD_CARD_SD_H
#define DEMO_SPI_SD_CARD_SD_H

#include "sd_interface.h"   //包含接口文件

#define u8      uint8_t
#define u16     uint16_t
#define u32     uint32_t


//卡类型
#define MMC     0
#define V1      1
#define V2      2
#define V2HC    4



#define SDCard_CMD0 0 //卡复位

#define SDCard_CMD8 8 //命令8 ，SEND_IF_COND

#define SDCard_CMD9 9 //命令9 ，读CSD数据

#define SDCard_CMD12 12 //命令12，停止数据传输

#define SDCard_CMD16 16 //命令16，设置扇区大小 应返回0x00

#define SDCard_CMD17 17 //命令17，读扇区

#define SDCard_CMD18 18 //命令18，读多个扇区

#define SDCard_CMD23 23 //命令23，设置多扇区写入前预先擦除block

#define SDCard_CMD24 24 //命令24，写扇区

#define SDCard_CMD25 25 //命令25，写多个扇区

#define SDCard_CMD41 41 //命令41，应返回0x00

#define SDCard_CMD55 55 //命令55，应返回0x01

#define SDCard_CMD58 58 //命令58，读OCR信息


//函数声明,来自华为云,开发者https://bbs.huaweicloud.com/blogs/383913
u8 SDCardReadWriteOneByte(u8 data);                     //底层接口，SPI读写字节函数
void SDCardReadData(u8*buf,u32 sector,u32 cnt);		    //读块(扇区)
void SDCardWriteData(u8*buf,u32 sector,u32 cnt);		//写块(扇区)
u8 SDCardSendData(u8*buf,u8 cmd);  						//发送数据包
u8 SDCardRecvData(u8*buf,u16 len);						//接收数据包
u32 GetSDCardSectorCount(void);   		 			    //读扇区数


//控制SD_CS引脚电平
#define SD_CS_Set()     sd_interface_cs_set()
#define SD_CS_Clr()     sd_interface_cs_clr()

/* @brief 读取SD应答并判断
 * @param responese 正确的回应
 * @retval 1 返回失败
 *         0 返回成功*/
uint8_t SD_GetResponse(uint16_t response);

/* @brief 初始化SD卡
 * @retval 初始化成功返回0,失败返回非0*/
uint8_t SD_Init(void);

/* @brief 获取根目录的物理扇区号*/
uint32_t SD_GetRoot(uint32_t number);

/* @brief 获取逻辑0扇区的物理扇区号*/
uint32_t SD_GetLogic0(void);

/* @brief 从指定扇区的起始处读len(len <＝　512)字节
 * @param rxbuff 数据存储区地址
 *        sector 扇区地址(物理扇区)
 *        len 字节数
 * @retval */
uint8_t SD_ReadBlock(uint8_t *rxbuff, uint32_t sector_addr, uint16_t len);

/* @brief 该函数向SD卡写入命令
 * @param command 命令
 *          argumen 参数
 *          crc crc校验值
 *          reset 0 退出时保持SD卡片选
 *                 1 退出时失能SD卡片选
 * @retval temp 返回SD卡响应*/
uint8_t SD_WriteCommand(uint8_t cmd,uint32_t argument,uint8_t crc,uint8_t reset);

#endif //DEMO_SPI_SD_CARD_SD_H
