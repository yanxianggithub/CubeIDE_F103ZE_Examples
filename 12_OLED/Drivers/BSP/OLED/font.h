
#ifndef BSP_OLED_FONT_H_
#define BSP_OLED_FONT_H_
#include <stdint.h>

struct _font
{
    uint16_t w;
    uint16_t h;
    const char *index;
    const uint8_t *bitmap;
};

typedef struct
{
    struct _font ch;
} font_t;

/* 汉字字模全局声明 */
extern const font_t font_16x16_mis;
extern const font_t font_24x24_mis;
extern const font_t font_32x32_mis;

/* 图模全局声明 */
extern const uint8_t refresh_32x32_logo[];
extern const uint8_t temp_32x32_logo[];
extern const uint8_t zdyz_32x32_logo[];

#endif /* BSP_OLED_FONT_H_ */
