#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "f10x-pcd8544.h"
#include "font6x8.h"	  // �����


// ���������� ������ LCD_CS
#define LCD_CS1   GPIO_SetBits  (LCD_PORT, SCE)
#define LCD_CS0   GPIO_ResetBits(LCD_PORT, SCE)
// ���������� ������ LCD_RST
#define LCD_RST1  GPIO_SetBits  (LCD_PORT, RST)
#define LCD_RST0  GPIO_ResetBits(LCD_PORT, RST)
// ���������� ������ LCD_DC
#define LCD_DC1   GPIO_SetBits  (LCD_PORT, DC)
#define LCD_DC0   GPIO_ResetBits(LCD_PORT, DC)
// ���������� ������ LCD_SCK
#define LCD_SCK1   GPIO_SetBits  (LCD_PORT, SCLK)
#define LCD_SCK0   GPIO_ResetBits(LCD_PORT, SCLK)
// ���������� ������ LCD_MOSI
#define LCD_MOSI1   GPIO_SetBits  (LCD_PORT, MOSI)
#define LCD_MOSI0   GPIO_ResetBits(LCD_PORT, MOSI)

unsigned char lcd_buff[84*6]; // ����� �������


// �������� ������\������ �� �������
void lcd_senddata(unsigned char data) {
unsigned char i;
	for(i=0;i<8;i++) {
		if (data & 0x80) LCD_MOSI1; else LCD_MOSI0;
		data = data<<1;
		LCD_SCK0;
		LCD_SCK1;
	}
}

// ���������� ������� ���������� ������
void lcd_refresh(void) {
	LCD_CS0;            // �S=0 - ������ ����� ������ � ��������

	LCD_DC0;            // �������� �������
	lcd_senddata(0x40); // ��������� ������� � ������� Y=0; X=0
	lcd_senddata(0x80);

	LCD_DC1;            // �������� ������

	unsigned char y, x;

	for (y=0;y<6;y++) for (x=0;x<84;x++) lcd_senddata(lcd_buff[y*84+x]);
}


// �������������
void lcd_init(void) {

	  RCC_APB2PeriphClockCmd(LCD_PH, ENABLE);

	  GPIO_InitTypeDef GPIO_InitStructure;

	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Pin   = SCLK | MOSI | DC | RST | SCE ;
	  GPIO_Init(LCD_PORT, &GPIO_InitStructure);

	  LCD_SCK0;
	  // ����� �������
	  LCD_CS0;            // CS=0  - ������ ����� ������ � ��������
	  LCD_RST0;           // RST=0 - ����� �������
	  LCD_RST1;           // RST=1

	  // ������������������ ������������� �������
	  LCD_DC0;            // �������� �������

	  lcd_senddata(0x21);      // ������� � ����������� �����
	  lcd_senddata(0xC1);

	  lcd_senddata(0x06);		// ������������� �����������, �� 4 �� 7

	  lcd_senddata(0x13);		// Bias 0b0001 0xxx - �������� ��� �������������

	  lcd_senddata(0x20); 	// ������� � ������� �����
	  lcd_senddata(0b1100);	    // 0b1100 - normal mode
			                        // 0b1101 - invert mode
			                        // 0b1001 - ��������� ����������� �����
			                        // 0b1000 - ������ �����

	  LCD_DC1;            // �������� �������

	  lcd_refresh();
}

// ����� �������
void lcd_putpix(unsigned char x, unsigned char y, unsigned char mode) {
	if ((x>84) || (y>47)) return;

	unsigned int adr=(y>>3)*84+x;
	unsigned char data=(1<<(y&0x07));

	if (mode) lcd_buff[adr]|=data;
   else lcd_buff[adr]&=~data;
}



// ��������� ��������� �����
void lcd_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
signed char   dx, dy, sx, sy;
unsigned char  x,  y, mdx, mdy, l;


  dx=x2-x1; dy=y2-y1;

  if (dx>=0) { mdx=dx; sx=1; } else { mdx=x1-x2; sx=-1; }
  if (dy>=0) { mdy=dy; sy=1; } else { mdy=y1-y2; sy=-1; }

  x=x1; y=y1;

  if (mdx>=mdy) {
     l=mdx;
     while (l>0) {
         if (dy>0) { y=y1+mdy*(x-x1)/mdx; }
            else { y=y1-mdy*(x-x1)/mdx; }
         lcd_putpix(x,y,mode);
         x=x+sx;
         l--;
     }
  } else {
     l=mdy;
     while (l>0) {
        if (dy>0) { x=x1+((mdx*(y-y1))/mdy); }
          else { x=x1+((mdx*(y1-y))/mdy); }
        lcd_putpix(x,y,mode);
        y=y+sy;
        l--;
     }
  }
  lcd_putpix(x2, y2, mode);

}



// ��������� �������������� (�� ������������)
void lcd_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
	lcd_line(x1,y1, x2,y1, mode);
	lcd_line(x1,y2, x2,y2, mode);
	lcd_line(x1,y1, x1,y2, mode);
	lcd_line(x2,y1, x2,y2, mode);
}

// ����� ������� �� ����� �� �����������
void lcd_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode) {
	const unsigned char *fontpointer;


	if (ch<127) {	// �������������� ��������
		fontpointer=NewFontLAT; ch=ch-32; }
		else	{	// �������������� ������� ����
		fontpointer=NewFontRUS; ch=ch-192;}

    unsigned char lcd_YP=7- (py & 0x07);    // ������� ������� ������� � �����
    unsigned char lcd_YC=(py & 0xF8)>>3; 	// �������� ������� ������� �� ������
    unsigned char x;
	for (x=0; x<6; x++) {

		unsigned char temp=*(fontpointer+ch*6+x);

		if (mode!=0) {
			temp=~temp;
			if (py>0) lcd_putpix(px, py-1, 1);	// ���� ������ � ������ �������� - ������ �������� �����
		}

		temp&=0x7F;

		lcd_buff[lcd_YC*84+px]=lcd_buff[lcd_YC*84+px] | (temp<<(7-lcd_YP)); 	// ������ ������� ����� �������

	    if (lcd_YP<7) lcd_buff[(lcd_YC+1)*84+px]=lcd_buff[(lcd_YC+1)*84+px] | (temp>>(lcd_YP+1)); 	// ������ ������ ����� �������

		px++;
		if (px>83) return;
	}
}


// ������ ����������� �����
void lcd_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode) {
	unsigned int divid=10000;
	unsigned char i;

	for (i=5; i!=0; i--) {

		unsigned char res=numb/divid;

		if (i<=dcount) {
			lcd_putchar(x, y, res+'0', mode);
			x=x+6;
		}

		numb%=divid;
		divid/=10;
	}
}


// ����� ����� ������� ��� ������ � �������
void lcd_leftchline(unsigned char x, unsigned char y) {
	if (x>0) lcd_line(x-1, y-1, x-1, y+6, 1);
}


// ����� ������
void lcd_putstr(unsigned char x, unsigned char y, unsigned char str[], unsigned char mode) {
	if (mode) lcd_leftchline(x, y);
	while (*str!=0) {
		lcd_putchar(x, y, *str, mode);
		x=x+6;
		str++;
	}
}
