//
// Created by Lightrain on 2023/11/19.
//
#include "sd_interface.h"

/* SPI发送与接收接口函数.这里SPI使用应答式,MISO和MOSI不同时传输有效数据
 * @param txdata 要发送的八位数据
 * @retval RxData 接收到的数据*/
uint8_t sd_interface_transmit_receive_data(uint8_t txdata) {
    uint8_t RxData;
//    //先调用发送函数
//    HAL_SPI_Transmit(&HandSPI,&txdata,1,10);
//
//    //发送完成后,再调用接收函数
//    HAL_SPI_Receive(&HandSPI,&RxData,1,10);
    HAL_SPI_TransmitReceive(&HandSPI,&txdata,&RxData,1,1000);

    return RxData;
}


/* @brief 对CS引脚GPIO进行初始化*/
void sd_interface_gpio_init(void) {
    //定义初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    //使能GPIO时钟
    SD_CS_GPIO_Colck;

    //初始化GPIO电平
    HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_SET);

    //配置GPIO模式
    GPIO_InitStruct.Pin = SD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    //调用HAL_GPIO_Init()函数进行初始化
    HAL_GPIO_Init(SD_CS_GPIO_Port,&GPIO_InitStruct);
}

/* SD_CS_Pin接口函数*/
void sd_interface_cs_set(void) {
    //拉高SD_CS
    HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_SET);
}

void sd_interface_cs_clr(void) {
    //拉低SD_CS
    HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_RESET);
}


/* @brief 提供ms级延迟*/
void sd_interface_delayms(uint32_t ms) {
    //提供ms级延迟函数
    HAL_Delay(ms);
}