/*

    epdgl.h - header file for the e-paper display graphics library

    author: Pete Fan
    date: Oct 19, 2020

*/

#ifndef EPDGL_H
#define EPDGL_H

#include "gdew042t2.h"
#include "font.h"
#include <stdint.h>
#include <stdbool.h>

// Display resolution
#define EPD_WIDTH   400
#define EPD_HEIGHT  300

typedef enum epd_orientation {
    LANDSCAPE,          // rotate 0
    PORTRAIT,           // rotate 90
    LANDSCAPE_REVERSE,  // rotate 180
    PORTRAIT_REVERSE    // rotate 270
} epd_orientation_t;

typedef enum epd_color {
    EPD_WHITE,
    EPD_BLACK,
    EPD_GREY
} epd_color_t;

typedef enum epd_update {
    EPD_FAST,
    EPD_PART,
    EPD_SLOW
} epd_update_t;

typedef struct plot_config {
    int32_t min_x;
    int32_t max_x;
    int32_t min_y;
    int32_t max_y;
    epd_color_t color;
} plot_config_t;

typedef struct text_config {
    font_t * font;
    epd_color_t color;
} text_config_t;

void epdgl_init(void);

void epdgl_set_orientation(epd_orientation_t o);

bool epdgl_update_screen(epd_update_t u);

void epdgl_draw_pixel(int32_t x, int32_t y, epd_color_t c);

void epdgl_clear(void);

void epdgl_invert(void);

void epdgl_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, epd_color_t c);

void epdgl_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, epd_color_t c);

void epdgl_fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, epd_color_t c);

void epdgl_draw_circle(int32_t x, int32_t y, int32_t r, epd_color_t c);

void epdgl_fill_circle(int32_t x, int32_t y, int32_t r, epd_color_t c);

void epdgl_set_cursor(int32_t x, int32_t y);

// Moves cursor
void epdgl_draw_char(char s, text_config_t * cfg);

// Moves cursor
void epdgl_draw_int(int32_t i, text_config_t * cfg);

// Moves cursor
void epdgl_draw_string(const char * s, text_config_t * cfg);

void epdgl_plot(int32_t * x, int32_t * y, int32_t l, plot_config_t cfg);

#endif // EPDGL_H
