
#include "font.h"

/***************************************** ͼƬȡģ *********************************************/
const uint8_t refresh_32x32_logo[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x00,
    0x00,0xFF,0xF8,0x00,0x03,0xFF,0xFC,0x00,0x07,0xE0,0x7E,0x00,0x0F,0x80,0x1F,0x00,
    0x1F,0x00,0x07,0x80,0x1E,0x00,0x03,0xC0,0x3C,0x00,0x01,0xC0,0x38,0x00,0x01,0xC0,
    0x38,0x00,0x00,0xE0,0x78,0x00,0x00,0xE0,0x70,0x00,0x00,0xE0,0x70,0x00,0x00,0xE0,
    0x70,0x00,0x0E,0xEC,0x70,0x00,0x0F,0xFE,0x70,0x00,0x07,0xFE,0x38,0x00,0x07,0xFC,
    0x38,0x00,0x03,0xF8,0x3C,0x00,0x01,0xE0,0x1E,0x00,0x00,0xC0,0x1F,0x00,0x00,0x00,
    0x0F,0x80,0x1C,0x00,0x07,0xE0,0x7C,0x00,0x03,0xFF,0xFC,0x00,0x00,0xFF,0xF8,0x00,
    0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};  /*"ic_Refresh.png" */

const uint8_t temp_32x32_logo[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xC0,0x00,0x00,0x1F,0xE3,0xE0,
	0x00,0x3F,0xE7,0xE0,0x00,0x38,0x73,0xC0,0x00,0x70,0x70,0x00,0x00,0x70,0x77,0xE0,
	0x00,0x70,0x77,0xE0,0x00,0x70,0x70,0x00,0x00,0x70,0x70,0x00,0x00,0x70,0x77,0xE0,
	0x00,0x20,0x27,0xE0,0x00,0x70,0x78,0x00,0x00,0xF0,0x7C,0x00,0x01,0xE0,0x1E,0x00,
	0x03,0xC0,0x0E,0x00,0x03,0xB8,0x6F,0x00,0x07,0x38,0xE7,0x00,0x07,0x38,0x67,0x00,
	0x07,0x38,0x67,0x00,0x07,0x0C,0x87,0x00,0x07,0x0F,0xC7,0x00,0x03,0x8F,0xC7,0x00,
	0x03,0x87,0x8F,0x00,0x03,0xC0,0x1E,0x00,0x01,0xE0,0x3C,0x00,0x00,0xF8,0xFC,0x00,
	0x00,0x7F,0xF8,0x00,0x00,0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};	/*"Ѫѹ.png" */

const uint8_t zdyz_32x32_logo[] =
{
    0x00, 0x1F, 0xF8, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x07, 0xE0, 0x3F, 0xE0, 0x0F, 0x00, 0x03, 0xF0, 0x1C, 0xFF, 0x80, 0xF0,
    0x17, 0xFF, 0xF0, 0x78, 0x3F, 0xFF, 0xFC, 0x3C, 0x3E, 0x00, 0x7E, 0x1C, 0x70, 0x00, 0x0F, 0x3E, 0x4F, 0xFE, 0x07, 0xFE, 0xBF, 0xFF, 0x81, 0xFF,
    0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xFC, 0x07, 0xFF, 0xFF, 0xF8, 0x0F, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF,
    0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0x83, 0xFF, 0xFD, 0x7F, 0xC0, 0xFF, 0xF2, 0x7D, 0xF0, 0x06, 0x0E, 0x78, 0x7E, 0x00, 0x7C,
    0x3C, 0x3F, 0xFF, 0xFC, 0x1E, 0x0F, 0xFF, 0xE8, 0x1F, 0x01, 0xFF, 0x30, 0x0F, 0xC0, 0x00, 0xF0, 0x07, 0xFC, 0x07, 0xC0, 0x01, 0xFF, 0xFF, 0x80,
    0x00, 0xFF, 0xFF, 0x00, 0x00, 0x1F, 0xF8, 0x00, /*"C:\Users\lzy\Desktop\LOGO.bmp",0*/
};	/*"����ԭ��LOGO.png" */


/***************************************** ����ȡģ��16 24 32�� *********************************************/

 /* �ַ����� */
const char font1616_table[] =
{
    "����ԭ��"
};

/* 16x16 �������壨���壩 ���� ȡģ��ʽ:����ʽ ˳�򣨸�λ��ǰ�� */
const uint8_t font_1616[]=
{
	0x00,0x02,0x40,0x02,0x40,0x02,0x43,0xFE,0x40,0x02,0x40,0x02,0x40,0x02,0x7F,0xFE,
	0x41,0x02,0x41,0x02,0x41,0x02,0x41,0x02,0x41,0x02,0x40,0x02,0x00,0x02,0x00,0x00,/*"��",0*/
	0x00,0x00,0x00,0x01,0x00,0x02,0x03,0xEC,0x02,0x20,0x02,0x20,0x02,0x28,0xFE,0x26,
	0x12,0x20,0x12,0x28,0x12,0x26,0x12,0x20,0x13,0xE0,0x10,0x08,0x10,0x07,0x00,0x00,/*"��",1*/
	0x00,0x02,0x00,0x0C,0xFF,0xF0,0x80,0x04,0x80,0x08,0x9F,0xD0,0x92,0x44,0xB2,0x42,
	0xD2,0x7C,0x92,0x40,0x92,0x40,0x9F,0xD0,0x80,0x08,0x80,0x04,0x80,0x00,0x00,0x00,/*"ԭ",2*/
	0x02,0x00,0x82,0x00,0x82,0x00,0x82,0x00,0x82,0x00,0x82,0x04,0x82,0x02,0x8F,0xFC,
	0x8A,0x00,0x92,0x00,0xA2,0x00,0xC2,0x00,0x82,0x00,0x02,0x00,0x02,0x00,0x00,0x00,/*"��",3*/
	0x04,0x00,0x24,0x10,0x24,0x12,0x24,0x21,0x7F,0xFE,0xC4,0x40,0x44,0x82,0x04,0x02,
};

/* �ṹ���Ա��ֵ�����㺺����ʾ����ͳһ���� */
const font_t font_16x16_mis =
{
    .ch =
    {
        .w = 16,
        .h = 16,
        .index = font1616_table,
        .bitmap = font_1616,
    }
};


/* �ַ����� */
const char font2424_table[] =
{
     "����ԭ��"
};

/* 24x24 �������壨���壩 ���� ȡģ��ʽ:����ʽ ˳�򣨸�λ��ǰ�� */
const uint8_t font_2424[]=
{
	0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x10,0x00,0x08,0x10,0x00,0x08,0x10,
	0x00,0x08,0x10,0xFF,0xF8,0x10,0x40,0x08,0x10,0x00,0x08,0x10,0x00,0x08,0x10,0x00,
	0x08,0x10,0x00,0x08,0x1F,0xFF,0xF8,0x10,0x10,0x08,0x10,0x10,0x08,0x10,0x10,0x08,
	0x10,0x10,0x08,0x10,0x10,0x08,0x10,0x10,0x08,0x10,0x30,0x08,0x30,0x10,0x18,0x10,
	0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",0*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x06,0x00,0x00,0x1C,0x00,
	0x00,0x00,0x00,0x7F,0x80,0x00,0x41,0x00,0x00,0x41,0x20,0x00,0x41,0x1C,0x00,0x41,
	0x0C,0x7F,0xC1,0x00,0x24,0x41,0x00,0x04,0x41,0x20,0x04,0x41,0x18,0x04,0x41,0x0C,
	0x04,0x41,0x00,0x04,0x7F,0x80,0x04,0x7F,0xA0,0x0C,0x00,0x10,0x04,0x00,0x1C,0x00,
	0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",1*/
	0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x04,0x00,0x00,0x18,0x3F,0xFF,0xE0,0x3F,
	0xFE,0x04,0x20,0x00,0x08,0x20,0x00,0x10,0x23,0xFE,0x30,0x23,0xFE,0x60,0x22,0x44,
	0xC0,0x22,0x44,0x04,0x26,0x44,0x04,0x3A,0x44,0x0E,0x22,0x47,0xFC,0x22,0x44,0x00,
	0x22,0x44,0x80,0x22,0x44,0x40,0x22,0x44,0x60,0x63,0xFE,0x20,0x20,0x00,0x38,0x00,
	0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,/*"ԭ",2*/
	0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x20,0x20,0x00,0x20,
	0x20,0x00,0x20,0x20,0x00,0x20,0x20,0x10,0x20,0x20,0x10,0x20,0x20,0x08,0x20,0x20,
	0x0C,0x21,0xFF,0xFC,0x21,0xFF,0xF8,0x22,0x20,0x00,0x22,0x20,0x00,0x24,0x20,0x00,
	0x28,0x20,0x00,0x30,0x20,0x00,0x30,0x20,0x00,0x20,0x20,0x00,0x00,0x60,0x00,0x00,
	0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",3*/
};

/* �ṹ���Ա��ֵ�����㺺����ʾ����ͳһ���� */
const font_t font_24x24_mis =
{
    .ch =
    {
        .w = 24,
        .h = 24,
        .index = font2424_table,
        .bitmap = font_2424,
    }
};


/* �ַ����� */
const char font3232_table[] =
{
    "����ԭ��"
};

/* 32x32 �������壨��Բ�� ���� ȡģ��ʽ:����ʽ ˳�򣨸�λ��ǰ�� */
const uint8_t font_3232[]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x20,0x00,0x00,0x18,
	0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x7F,0xFF,0xF8,
	0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,
	0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,0x30,0x00,0x00,0x18,
	0x3F,0xFF,0xFF,0xF8,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,
	0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,
	0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,
	0x30,0x02,0x00,0x18,0x30,0x00,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,/*"��",0*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x18,
	0x00,0x1F,0xF0,0x70,0x00,0x3F,0xF9,0xC0,0x00,0x30,0x0D,0x00,0x00,0x30,0x0C,0x00,
	0x00,0x30,0x0C,0x00,0x00,0x30,0x0C,0x00,0x00,0x30,0x0C,0x00,0x00,0x30,0x0F,0xC0,
	0x00,0x30,0x0D,0xFE,0x7F,0xF0,0x0C,0x00,0x7F,0xF0,0x0C,0x00,0x0C,0x30,0x0C,0x00,
	0x0C,0x30,0x0C,0x00,0x0C,0x30,0x0C,0x00,0x0C,0x30,0x0E,0x00,0x0C,0x30,0x0D,0xC0,
	0x0C,0x30,0x0C,0xFC,0x0C,0x30,0x0C,0x0C,0x0C,0x30,0x0C,0x00,0x0C,0x30,0x0C,0x00,
	0x0C,0x30,0x0C,0x00,0x0C,0x3F,0xF8,0x00,0x0C,0x1F,0xFB,0x80,0x0C,0x00,0x00,0xE0,
	0x0C,0x00,0x00,0x78,0x04,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",1*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x01,0xFC,0x3F,0xFF,0xFF,0xE0,
	0x30,0x00,0x00,0x00,0x60,0x00,0x00,0x08,0x60,0x00,0x00,0x18,0x60,0x00,0x00,0x30,
	0x61,0xFF,0xE0,0x60,0x61,0x88,0x61,0xC0,0x63,0x08,0x23,0x80,0x63,0x08,0x6E,0x0C,
	0x63,0x08,0x60,0x0C,0x63,0x08,0x60,0x04,0x63,0x08,0x60,0x06,0x63,0x08,0x60,0x06,
	0x6F,0x08,0x60,0x04,0x7F,0x08,0x7F,0xEC,0x63,0x08,0x7F,0xF8,0x63,0x08,0x60,0x00,
	0x63,0x08,0x60,0x00,0x63,0x08,0x60,0x00,0x63,0x08,0x60,0x00,0x63,0x08,0x6C,0x00,
	0x63,0x08,0x67,0x00,0x63,0x08,0x23,0x80,0x61,0x08,0x60,0xC0,0x61,0xFF,0xE0,0x70,
	0x60,0x00,0x00,0x38,0x60,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"ԭ",2*/
	0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,
	0x00,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,0x30,
	0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x18,0x30,0x03,0x00,0x08,0x30,0x03,0x00,0x0C,
	0x30,0x03,0x00,0x0C,0x30,0x23,0x00,0x0C,0x30,0x73,0x00,0x0C,0x30,0x7B,0x00,0x0C,
	0x30,0xCF,0x00,0x0C,0x30,0x8F,0x00,0x18,0x31,0x87,0x80,0x18,0x33,0x03,0xF8,0xF0,
	0x33,0x03,0x7F,0xE0,0x36,0x03,0x00,0x00,0x3C,0x03,0x00,0x00,0x38,0x03,0x00,0x00,
	0x38,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,
	0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",3*/
};

/* �ṹ���Ա��ֵ�����㺺����ʾ����ͳһ���� */
const font_t font_32x32_mis =
{
    .ch =
    {
        .w = 32,
        .h = 32,
        .index = font3232_table,
        .bitmap = font_3232,
    }
};


