//
// Created by Lightrain on 2023/11/19.
//

#include "sd.h"
extern void SD_SPI3_Init(void);

uint8_t SD_Type = 0;    //SD卡类型

/* @brief 读取SD应答并判断
 * @param responese 正确的回应
 * @retval 1 返回失败
 *         0 返回成功*/
uint8_t SD_GetResponse(uint16_t response) {
    uint16_t i = 5000; //最大回应次数设为5000
    while ((sd_interface_transmit_receive_data(0xFF) != response) && (i != 0)) {
        i--;
    }

    if(i == 0){
        return 1;   //返回失败
    }
    else{
        return 0;   //返回成功
    }
}


/* @brief 该函数向SD卡写入命令
 * @param command 命令
 *          argumen 参数
 *          crc crc校验值
 *          reset 0 退出时保持SD卡片选
 *                 1 退出时失能SD卡片选
 * @retval temp 返回SD卡响应*/
uint8_t SD_WriteCommand(uint8_t cmd,uint32_t argument,uint8_t crc,uint8_t reset) {
    uint8_t temp = 0xFF;
    uint8_t i;

    //拉高SD,并增加8个时钟确保上次操作完成
    SD_CS_Set();
    sd_interface_transmit_receive_data(0xFF);

    //选中SD卡
    SD_CS_Clr();
    //依次写入命令,参数,校验码
    sd_interface_transmit_receive_data(cmd | 0x40);
    sd_interface_transmit_receive_data(argument >> 24); //Max bit first
    sd_interface_transmit_receive_data(argument >> 16);
    sd_interface_transmit_receive_data(argument >> 8);
    sd_interface_transmit_receive_data(argument);
    sd_interface_transmit_receive_data(crc);

//    i = 0;
//    do {
//        temp = sd_interface_transmit_receive_data(0xFF);    //接收响应
//        i ++;
//        if(i > 200) {   //超时则退出
//            break;
//        }
//    } while (temp == 0xFF); //响应为空则再次接收响应,直到不为空为止

    //do while循环改为for循环,直到接收到响应为止
    for(i = 0;temp == 0xFF;i++) {
        temp = sd_interface_transmit_receive_data(0xFF);    //交换字节,交换响应
        if(i > 200) {   //循环过多次则直接退出
            break;
        }
    }

    //根据要求保持或失能SD卡
    if(reset) {
        SD_CS_Set();
        sd_interface_transmit_receive_data(0xFF);   //取消片选之后，需要再额外发送8个时钟信号，结束本次操作。
    }

    return temp;
}

/* @brief 从指定扇区的起始处读len(len <＝　512)字节
 * @param rxbuff 数据存储区地址
 *        sector 扇区地址(物理扇区)
 *        len 字节数
 * @retval */
uint8_t SD_ReadBlock(uint8_t *rxbuff, uint32_t sector_addr, uint16_t len) {
    uint8_t temp;
    uint16_t i;

    //除SDHC外,其他卡均需要扇区地址装换成字节地址
    if(SD_Type != V2HC) {
        sector_addr = sector_addr << 9;
    }

    //读命令
    temp = SD_WriteCommand(SDCard_CMD17, sector_addr, 0, 1);
    if(temp != 0x00) {
        return temp;
    }

    //启动依次传输
    SD_CS_Clr();

    if(SD_GetResponse(0xFE)) {  //等待SD卡回应0xFE
        SD_CS_Set();
        return 1;   //错误退出
    }

    for(i = 512;i > 0;i--) {
        *rxbuff = sd_interface_transmit_receive_data(0xFF); //用空白信息交换有效信息
        rxbuff++ ; //接收地址自增
    }

    sd_interface_transmit_receive_data(0xFF);   //丢弃两个CRC
    sd_interface_transmit_receive_data(0xFF);

    //结束本次传输
    SD_CS_Set();

    sd_interface_transmit_receive_data(0xFF);   //再送8个时钟以保证操作完成

    return 0;
}


/* @brief 获取逻辑0扇区的物理扇区号*/
uint32_t SD_GetLogic0(void) {
    uint32_t temp = 0;
    uint8_t buff[512];

    SD_ReadBlock(buff,0,512);
    temp += buff[0x1C6];
    temp += buff[0x1C6 + 1] << 8;
    temp += buff[0x1C6 + 2] << 16;
    temp += buff[0x1C6 + 3] << 24;

    return temp;
}

/* @brief 获取根目录的物理扇区号*/
uint32_t SD_GetRoot(uint32_t number) {
    uint32_t temp=0;
    uint8_t buff[512];

  SD_ReadBlock(buff, number, 512);

  if(buff[0x52] == 'F' && buff[0x53] == 'A' && buff[0x54] == 'T' && buff[0x55] == '3' && buff[0x56] == '2') //FAT32
    temp+= buff[14] + (buff[15] << 8) + buff[16] * (buff[36] + (buff[37] << 8));
  else if(buff[0x36] == 'F' && buff[0x37] == 'A' && buff[0x38] == 'T' && buff[0x39] == '1' && buff[0x3a] == '6') //FAT16
    temp+= buff[14] + (buff[15] << 8) + buff[16] * (buff[22] + (buff[23] << 8));

  return temp;
}

/* @brief 初始化SD卡
 * @retval 初始化成功返回0,失败返回非0*/
uint8_t SD_Init(void) {
    uint8_t temp;
    uint16_t i;
    uint8_t buff[6];

    //初始化SPI
    MX_SPI3_Init();

    //初始化SD_CS引脚
    sd_interface_gpio_init();
    SD_CS_Set();

    //为适应MMC卡要求时钟 < 400KHz,暂用256分频
    SD_SPI3_Init();

    //延迟100ms等卡上电
    sd_interface_delayms(100);

    //输出卡要求的至少74个初始化脉冲,并要求CS为高电平
    for(i = 0;i < 10;i++) {
        sd_interface_transmit_receive_data(0xFF);
    }

    //发送CMD0,进入SPI模式
    i = 0;
    do {
        temp = SD_WriteCommand(SDCard_CMD0,0,0x95,1); //CMD0 = 0 复位卡,进入IDLE(闲置)状态
        i++;
    } while ((temp != 0x01) && (i < 200));  //等待回应0x01,返回0x01就代表复位成功

    if(temp == 200) {
        return 1;   //失败退出
    }


    /* 下面进入SD卡,获取卡的版本信息*/
    SD_CS_Clr();
    temp = SD_WriteCommand(SDCard_CMD8,0x1AA,0x87,1);  //CMD8 = 8 argument = 0x1AA,1表示电压范围2.7-3.6V AA是固定的check pattern

    if(temp == 0x05) {
        //v1.0版和MMC
        SD_Type = V1;   //预设SDV1.0

        SD_CS_Set();

        sd_interface_transmit_receive_data(0xFF);   //增加8个始终确保本次操作完成

        i = 0;
        do {
            temp = SD_WriteCommand(SDCard_CMD55,0,0,1);   //发送CMD55,应返回0x01
            if(temp == 0xFF) {
                return temp;    //返回0xFF表明无卡,退出
            }
            temp = SD_WriteCommand(SDCard_CMD41,0,0,1);   //再发送ACMD41 = 41,应返回0x00
            i++;
            //回应正确,则卡类型预设成立
        } while ((temp != 0x00) && (i < 400));

        if(i == 400) {
            //无回应,是MMC卡
            i = 0;

            do {
                //MMC卡初始化
                temp = SD_WriteCommand(1,0,0,1);
                i++;
            } while ((temp != 0x00) && (i < 400));

            if(i == 400) {
                return 1;   //MMC卡初始化失败
            }

            SD_Type = MMC;
        }

        MX_SPI3_Init(); //SPI正常初始化,8分频

        sd_interface_transmit_receive_data(0xFF);   //输出8个时钟确保前次操作结束

        //禁用CRC校验
        temp = SD_WriteCommand(59,0,0x95,1);
        if(temp != 0x00) {
            return temp;    //错误返回
        }

        //设置扇区宽度
        temp = SD_WriteCommand(SDCard_CMD16,512,0x95,1);   //设定数据块大小为512字节
        if(temp != 0x00) {
            return temp;    //错误返回
        }
    } else if(temp == 0x01) {
        //V2.0和V2.0HC版
        //忽略V2.0卡的后续4字节
        sd_interface_transmit_receive_data(0xFF);
        sd_interface_transmit_receive_data(0xFF);
        sd_interface_transmit_receive_data(0xFF);
        sd_interface_transmit_receive_data(0xFF);

        SD_CS_Set();

        sd_interface_transmit_receive_data(0xFF);   //输出8个时钟确保前次操作结束

        //在2.0版本卡的基础下,循环发送CMD55和CMD41,等待初始化成功,如果CMD41返回0就表示卡复位成功
        i=0;
        do {
            temp = SD_WriteCommand(SDCard_CMD55,0,0,1);
            if(temp != 0x01) {
                return temp;	//错误返回
            }

            temp = SD_WriteCommand(SDCard_CMD41,0x40000000,0,1);
            if(i > 200)
              return temp;  //超时返回
        } while (temp != 0);

        //初始化成功后发送CMD58,判断是高速卡还是低速卡
        temp=SD_WriteCommand(SDCard_CMD58,0,0,0);  //CMD58 = 58,获取卡的容量,命令返回值为0则代表执行成功
        if(temp != 0x00) {
            SD_CS_Set();  //失能SD
            return temp;  //错误返回
        }

        //接收4个字节OCR信息
        buff[0]=sd_interface_transmit_receive_data(0xff);   //31-24
        buff[1]=sd_interface_transmit_receive_data(0xff);   //23-16
        buff[2]=sd_interface_transmit_receive_data(0xff);   //15-8
        buff[3]=sd_interface_transmit_receive_data(0xff);   //7-0

        SD_CS_Set();

        sd_interface_transmit_receive_data(0xff); //输出8个时钟确保前次操作结束

        //操作条件寄存器中的第30位指示了是高速卡还是低速卡,为1就是SDHC,为0就是SDSC
        if(buff[0] & 0x40) {
            SD_Type = V2HC;
        }
        else {
            SD_Type = V2;
        }

        //SD卡恢复正常速度
        MX_SPI3_Init();
    }

    return temp;
}




