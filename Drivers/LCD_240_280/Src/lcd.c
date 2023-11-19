#include "lcd.h"
#include "lcdfont.h"

/* @brief 调用接口向屏幕写入8位数据*/
void LCD_Write8BitData(uint8_t data) {
    lcd_interface_write_8bit_data(data);
}

/* @brief 调用接口向屏幕写入16bit数据*/
void LCD_Write16BitData(uint16_t data) {
    lcd_interface_write_16bit_data(data);
}

void LCD_WriteCommand(uint8_t command) {
    LCD_DC_Clr();   //DC的GPIO口初始化为上拉输出,不把它拉低他就默认为高电平
    lcd_interface_write_8bit_data(command);
    LCD_DC_Set();
}

void LCD_WriteAnyLengthData(uint8_t *pdata,uint16_t len) {
    lcd_interface_write_any_data(pdata,len);
}

void LCD_Delayms(uint32_t delay) {
    lcd_interface_delayms(delay);
}



/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WriteCommand(0x2a);//列地址设置
		LCD_Write16BitData(x1);
		LCD_Write16BitData(x2);
		LCD_WriteCommand(0x2b);//行地址设置
		LCD_Write16BitData(y1+20);
		LCD_Write16BitData(y2+20);
		LCD_WriteCommand(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WriteCommand(0x2a);//列地址设置
		LCD_Write16BitData(x1);
		LCD_Write16BitData(x2);
		LCD_WriteCommand(0x2b);//行地址设置
		LCD_Write16BitData(y1+20);
		LCD_Write16BitData(y2+20);
		LCD_WriteCommand(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WriteCommand(0x2a);//列地址设置
		LCD_Write16BitData(x1+20);
		LCD_Write16BitData(x2+20);
		LCD_WriteCommand(0x2b);//行地址设置
		LCD_Write16BitData(y1);
		LCD_Write16BitData(y2);
		LCD_WriteCommand(0x2c);//储存器写
	}
	else
	{
		LCD_WriteCommand(0x2a);//列地址设置
		LCD_Write16BitData(x1+20);
		LCD_Write16BitData(x2+20);
		LCD_WriteCommand(0x2b);//行地址设置
		LCD_Write16BitData(y1);
		LCD_Write16BitData(y2);
		LCD_WriteCommand(0x2c);//储存器写
	}
}


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_Write16BitData(color);
		}
	} 					  	    
}



/* @brief 快速刷屏函数,该函数过于底层*/
void LCD_FastFill(uint16_t color) {
    lcd_interface_fast_fill_color(color);
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_Write16BitData(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_Write16BitData(fc);
						else LCD_Write16BitData(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_Write16BitData(fc);
						else LCD_Write16BitData(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_Write16BitData(fc);
						else LCD_Write16BitData(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_Write16BitData(fc);
						else LCD_Write16BitData(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_Write16BitData(fc);
				else LCD_Write16BitData(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}

/* @brief 以十六进制显示数字*/
void LCD_ShowHexNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode) {
    uint8_t SignleNumber;
    LCD_ShowString(x,y,"0x",fc,bc,sizey,mode);
    for(uint8_t i = 0;i < len;i++){
        SignleNumber =num/ mypow(16,len - i - 1)%16;
        if(SignleNumber < 10){
            LCD_ShowChar(x + (i+2)*sizey/2,y,SignleNumber + '0',fc,bc,sizey,mode);
        } else{
            LCD_ShowChar(x + (i+2)*sizey/2,y,SignleNumber - 10 +'A',fc,bc,sizey,mode);
        }
    }
}

void LCD_ShowBinNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode) {
    for(uint8_t i = 0;i < len;i++){
        LCD_ShowChar(x + i*sizey/2,y,num/mypow(2,len - i -1)%2 + '0',fc,bc,sizey,mode);
    }
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}

/* @brief 带符号显示两位小数
 * @param   x,y显示坐标
            num 要显示小数变量
            len 要显示的位数
            fc 字的颜色
            bc 字的背景色
            sizey 字号*/
void LCD_ShowFloatNumber(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey){
    if(num >= 0){
        LCD_Fill(x,y,x + sizey/2,y + sizey,bc);
        LCD_ShowChar(x,y,'+',fc,bc,sizey,1);
        LCD_ShowFloatNum1(x + sizey/2,y,num,len,fc,bc,sizey);
    } else{
        LCD_Fill(x,y,x + sizey/2,y + sizey,bc);
        LCD_ShowChar(x,y,'-',fc,bc,sizey,1);
        LCD_ShowFloatNum1(x + sizey/2,y,-num,len,fc,bc,sizey);
    }
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_Write8BitData(pic[k*2]);
			LCD_Write8BitData(pic[k*2+1]);
			k++;
		}
	}
}

/* @brief 初始化屏幕并给屏幕填充一个底色
 * @param color 要填充的颜色*/
void LCD_Init(uint16_t color) {
	lcd_interface_gpio_init();//初始化GPIO
	
	LCD_RES_Clr();//复位
	LCD_Delayms(100);
	LCD_RES_Set();
	LCD_Delayms(100);
	
	LCD_BLK_Set();//打开背光
    LCD_Delayms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WriteCommand(0x11); //Sleep out 
	LCD_Delayms(120);              //Delay 120ms
	//************* Start Initial Sequence **********// 
	LCD_WriteCommand(0x36);
	if(USE_HORIZONTAL==0)LCD_Write8BitData(0x00);
	else if(USE_HORIZONTAL==1)LCD_Write8BitData(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Write8BitData(0x70);
	else LCD_Write8BitData(0xA0);

	LCD_WriteCommand(0x3A);			
	LCD_Write8BitData(0x05);

	LCD_WriteCommand(0xB2);			
	LCD_Write8BitData(0x0C);
	LCD_Write8BitData(0x0C); 
	LCD_Write8BitData(0x00); 
	LCD_Write8BitData(0x33); 
	LCD_Write8BitData(0x33); 			

	LCD_WriteCommand(0xB7);			
	LCD_Write8BitData(0x35);

	LCD_WriteCommand(0xBB);			
	LCD_Write8BitData(0x32); //Vcom=1.35V
					
	LCD_WriteCommand(0xC2);
	LCD_Write8BitData(0x01);

	LCD_WriteCommand(0xC3);			
	LCD_Write8BitData(0x15); //GVDD=4.8V  颜色深度
				
	LCD_WriteCommand(0xC4);			
	LCD_Write8BitData(0x20); //VDV, 0x20:0v

	LCD_WriteCommand(0xC6);			
	LCD_Write8BitData(0x0F); //0x0F:60Hz        	

	LCD_WriteCommand(0xD0);			
	LCD_Write8BitData(0xA4);
	LCD_Write8BitData(0xA1); 

	LCD_WriteCommand(0xE0);
	LCD_Write8BitData(0xD0);   
	LCD_Write8BitData(0x08);   
	LCD_Write8BitData(0x0E);   
	LCD_Write8BitData(0x09);   
	LCD_Write8BitData(0x09);   
	LCD_Write8BitData(0x05);   
	LCD_Write8BitData(0x31);   
	LCD_Write8BitData(0x33);   
	LCD_Write8BitData(0x48);   
	LCD_Write8BitData(0x17);   
	LCD_Write8BitData(0x14);   
	LCD_Write8BitData(0x15);   
	LCD_Write8BitData(0x31);   
	LCD_Write8BitData(0x34);   

	LCD_WriteCommand(0xE1);     
	LCD_Write8BitData(0xD0);   
	LCD_Write8BitData(0x08);   
	LCD_Write8BitData(0x0E);   
	LCD_Write8BitData(0x09);   
	LCD_Write8BitData(0x09);   
	LCD_Write8BitData(0x15);   
	LCD_Write8BitData(0x31);   
	LCD_Write8BitData(0x33);   
	LCD_Write8BitData(0x48);   
	LCD_Write8BitData(0x17);   
	LCD_Write8BitData(0x14);   
	LCD_Write8BitData(0x15);   
	LCD_Write8BitData(0x31);   
	LCD_Write8BitData(0x34);
	LCD_WriteCommand(0x21); 

	LCD_WriteCommand(0x29);

    //初始化完成,给屏幕填充一个底色
    LCD_Fill(0,0,LCD_W,LCD_H,color);
//    LCD_FastFill(color);
} 


