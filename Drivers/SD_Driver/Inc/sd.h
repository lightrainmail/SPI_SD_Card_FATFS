//
// Created by Lightrain on 2023/11/19.
//

#ifndef DEMO_SPI_SD_CARD_SD_H
#define DEMO_SPI_SD_CARD_SD_H

#include "sd_interface.h"   //包含接口文件


//卡类型
#define MMC     0
#define V1      1
#define V2      2
#define V2HC    4


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
uint8_t SD_ReadBlock(uint8_t *rxbuff,uint32_t sector,uint16_t len);

/* @brief 该函数向SD卡写入命令
 * @param command 命令
 *          argumen 参数
 *          crc crc校验值
 *          reset 0 退出时保持SD卡片选
 *                 1 退出时失能SD卡片选
 * @retval temp 返回SD卡响应*/
uint8_t SD_WriteCommand(uint8_t cmd,uint32_t argument,uint8_t crc,uint8_t reset);

#endif //DEMO_SPI_SD_CARD_SD_H
