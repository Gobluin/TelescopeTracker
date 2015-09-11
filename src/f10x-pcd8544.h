// ���� ����� � ������� ��������� �������
#define SCLK GPIO_Pin_0
#define MOSI GPIO_Pin_1
#define DC   GPIO_Pin_2
#define RST  GPIO_Pin_3
#define SCE  GPIO_Pin_4

#define LCD_PORT GPIOC	// ���� � �������� ��������� �������

#define LCD_PH RCC_APB2Periph_GPIOC // ���� � ������� ���������� �������


void lcd_init(void);  // �������������� �������

void lcd_refresh(void); // ���������� ������ �� ������

void lcd_putpix(unsigned char x, unsigned char y, unsigned char mode); // ����� �������

void lcd_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // ����� �����

void lcd_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode); // �������������

void lcd_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode); //  ����� �������

void lcd_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode); // ����� ����������� �����

void lcd_putstr(unsigned char x, unsigned char y, unsigned char str[], unsigned char mode); // ����� ������


