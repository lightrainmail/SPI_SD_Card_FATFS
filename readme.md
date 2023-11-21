# SPI模式驱动的SD卡,配上FATFS文件系统

## 本项目所用到的硬件

![IMG_20231121_233512](assets/IMG_20231121_233512.jpg)

一个8GB的SD卡以及SD卡槽模块	一个用于调试的小屏幕(这不是重点)	一个开发板NUCLEO-L476

### 用到的单片机外设

SPI1 用于小屏幕的显示

SPI3 用于与SD卡通信

若干GPIO引脚,操作屏幕的引脚,SD卡的片选引脚等

## CubeMX生成代码

### SPI1

**SPI1**用于屏幕的显示,因此选择**Trandmit Only Master**即可

主要参数如下图:

![image-20231121235805969](assets/image-20231121235805969.png)

**NOTE:**

重点在于**Data Size**要设置为8Bits

​		**First Bit**要设为MSB First

​		**CLock Polarity**设为Low

​		**Clock Phase**设为1 Edge

使用软件片选

其他选项默认即可

以下是SPI1所用到的两个引脚

SPI1_CLK->PA5

SPI1_MOSI->PA7

### SPI3

SPI3用于和SD卡进行通信,要选择**Full-Duplex Master **模式

主要参数如下图

![image-20231122000443586](assets/image-20231122000443586.png)

**NOTE:**

重点在于**Data Size**要设置为8Bits

​		**First Bit**要设为MSB First

​		**CLock Polarity**设为High

​		**Clock Phase**设为2 Edge

使用软件片选

其他选项默认即可

以下是SPI3占用的引脚

SPI3_CLK->PC10

SPI3_MISO->PC11

SPI3_MOSI->PC12

### FATFS文件系统

只用勾选一个User-defined

![image-20231122000757522](assets/image-20231122000757522.png)