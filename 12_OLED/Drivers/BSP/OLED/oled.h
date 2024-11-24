
#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#include "stdlib.h"
#include "../../SYSTEM/sys/sys.h"
#include "font.h"
/******************************************************************************************/

/* OLED SPIģʽ��ض˿ڿ��ƺ��� ����
 * ע��:OLED_RST/OLED_CS/OLED_RS,�������Ǻ�80����ģʽ���õ�,��80ģʽҲ����ʵ����3������!
 */
#define OLED_RST(x)     do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����RST���� */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����CS���� */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RS_GPIO_Port, OLED_RS_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RS_GPIO_Port, OLED_RS_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����RS���� */

#define OLED_WR(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_WR_GPIO_Port, OLED_WR_Pin, GPIO_PIN_SET) :  \
                                  HAL_GPIO_WritePin(OLED_WR_GPIO_Port, OLED_WR_Pin, GPIO_PIN_RESET); \
                        } while (0)     /* ����WR���� */

#define OLED_RD(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RD_GPIO_Port, OLED_RD_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RD_GPIO_Port, OLED_RD_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����RD���� */

/* ����/���� ���� */
#define OLED_CMD        0       /* д���� */
#define OLED_DATA       1       /* д���� */
#define OLED_H          64  	/* ˮƽ�ֱ��� */
#define OLED_W          128 	/* ��ֱ�ֱ��� */

/******************************************************************************************/
void oled_init(void);           /* OLED��ʼ�� */
void oled_clear(void);          /* OLED���� */
void oled_display_on(void);     /* ����OLED��ʾ */
void oled_display_off(void);    /* �ر�OLED��ʾ */
void oled_refresh_gram(void);   /* �����Դ浽OLED */
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);    /* OLED���� */
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);        /* OLED������� */
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode); /* OLED��ʾ�ַ� */
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);  /* OLED��ʾ���� */
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size);           /* OLED��ʾ�ַ��� */
void oled_draw_bitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t p_w, uint16_t p_h, uint8_t mode); /* ��ʾͼƬ */
void show_str(uint16_t x, uint16_t y, const char *str, uint8_t f_h, const font_t *font, uint8_t mode);	/* ��ָ��λ�ÿ�ʼ��ʾ�ַ���(֧����Ӣ����ʾ) */
#endif /* OLED_OLED_H_ */
