
//
// Bitmap font C header file generated by bmfont2c.py 
// 

#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
    uint8_t GlyphCount;
    uint8_t FirstAsciiCode;
    uint8_t GlyphBytesWidth;
    uint8_t GlyphHeight;
    uint8_t FixedWidth;
    uint8_t const *GlyphWidth;
    uint8_t const *GlyphBitmaps;
} font_t;

extern font_t Consolas14;
extern font_t Consolas20;

#endif // FONT_H
