/*

    gdew042t2.h - header file for the gdew042t2 4.2 inch e-paper display

    http://www.e-paper-display.com/products_detail/productId=321.html

    author: Pete Fan
    date: Oct 16, 2020

*/

#ifndef GDEW042T2_H
#define GDEW042T2_H

#include <stdint.h>
#include <stdbool.h>

void epd_init(void);

void epd_reset(void);

bool epd_is_idle(void);

void epd_set_partial_window(const uint8_t * frame_buffer, int x, int y, int w, int l);

void epd_set_lut_slow(void);

void epd_set_lut_fast(void);

void epd_display_frame_buffer(const uint8_t * frame_buffer);

void epd_display_frame(void);

void epd_clear_frame(void);

void epd_sleep(void);

#endif // GDEW042T2_H
