//
// Created by Lightrain on 2023/11/19.
//

#ifndef DEMO_SPI_SD_CARD_SD_INTERFACE_H
#define DEMO_SPI_SD_CARD_SD_INTERFACE_H

#include "main.h"
#include "spi.h"

//SD卡片选引脚选择
#define SD_CS_Port  GPIOD
#define SD_Pin      GPIO_PIN_2

//SD卡SPI
#define HandSPI     hspi3

#endif //DEMO_SPI_SD_CARD_SD_INTERFACE_H
