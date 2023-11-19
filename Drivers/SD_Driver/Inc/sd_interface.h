//
// Created by Lightrain on 2023/11/19.
//

#ifndef DEMO_SPI_SD_CARD_SD_INTERFACE_H
#define DEMO_SPI_SD_CARD_SD_INTERFACE_H

#include "main.h"
#include "spi.h"


//SD卡片选引脚选择
#define SD_CS_GPIO_Port     GPIOD
#define SD_CS_Pin           GPIO_PIN_2
#define SD_CS_GPIO_Colck    __HAL_RCC_GPIOD_CLK_ENABLE()


//SD卡SPI
#define HandSPI     hspi3


/* SPI发送与接收接口函数.这里SPI使用应答式,MISO和MOSI不同时传输有效数据
 * @param txdata 要发送的八位数据
 * @retval RxData 接收到的数据*/
uint8_t sd_interface_transmit_receive_data(uint8_t txdata);


/* @brief 对CS引脚GPIO进行初始化*/
void sd_interface_gpio_init(void);

/* SD_CS_Pin接口函数*/
void sd_interface_cs_set(void);
void sd_interface_cs_clr(void);

/* @brief 提供ms级延迟*/
void sd_interface_delayms(uint32_t ms);

#endif //DEMO_SPI_SD_CARD_SD_INTERFACE_H
