//
// Created by Lightrain on 2023/11/19.
//
#include "sd_interface.h"

/* SPI发送与接收接口函数.这里SPI使用应答式,MISO和MOSI不同时传输有效数据
 * @param
 * @retval*/
uint8_t sd_interface_transmit_receive_data(uint8_t txdata) {
    uint8_t RxData;
    //先调用发送函数
    HAL_SPI_Transmit(&HandSPI,&txdata,1,1000);

    //发送完成后,再调用接收函数
    HAL_SPI_Receive(&HandSPI,&RxData,1,1000);
    return RxData;
}