/*

    epdgl.c - source file for the e-paper display graphics library

    author: Pete Fan
    date: Oct 19, 2020

*/

#include "epdgl.h"

// text printing cursor
static int32_t CURSOR_X;
static int32_t CURSOR_Y;

// refresh counts to maintain safe operation and contrast level
#define PART_REFRESH_LIMIT 4
#define FAST_REFRESH_LIMIT 32
static uint8_t fast_refresh_count = 0;
static uint8_t part_refresh_count = 0;

// update request boolean
static bool UPDATE_REQUEST = false;
static bool FORCE_SLOW_UPDATE = false;

// minimal update area
static int32_t UPD_MIN_X;
static int32_t UPD_MAX_X;
static int32_t UPD_MIN_Y;
static int32_t UPD_MAX_Y;

static epd_orientation_t EPDGL_ROT = LANDSCAPE;

#define BUF_SIZE (((EPD_WIDTH + 7) / 8) * EPD_HEIGHT)
static uint8_t EPDGL_BUF[BUF_SIZE] = {0};

static void
epdgl_abs_update(int32_t x, int32_t y)
{
    // x = (x > EPD_WIDTH ? EPD_WIDTH : x) < 0 ? 0 : x;
    // y = (y > EPD_HEIGHT ? EPD_HEIGHT : y) < 0 ? 0 : y;
    UPD_MIN_X = (x < UPD_MIN_X ? x : UPD_MIN_X);
    UPD_MAX_X = (x > UPD_MAX_X ? x : UPD_MAX_X);
    UPD_MIN_Y = (y < UPD_MIN_Y ? y : UPD_MIN_Y);
    UPD_MAX_Y = (y > UPD_MAX_Y ? y : UPD_MAX_Y);
}

static void
epdgl_abs_reset()
{
    UPD_MIN_X = INT32_MAX;
    UPD_MAX_X = INT32_MIN;
    UPD_MIN_Y = INT32_MAX;
    UPD_MAX_Y = INT32_MIN;
}

static int32_t
epdgl_update_area()
{
    return (UPD_MAX_X - UPD_MIN_X + 1) * (UPD_MAX_Y -  UPD_MIN_Y + 1);
}

static void
epdgl_absolute(int32_t * x, int32_t * y)
{
    int32_t tmp;
    switch (EPDGL_ROT) {
    case LANDSCAPE:
        break;
    case PORTRAIT:
        tmp = *x;
        *x = *y;
        *y = EPD_HEIGHT - tmp;
        break;
    case LANDSCAPE_REVERSE:
        *x = EPD_WIDTH - *x;
        *y = EPD_HEIGHT - *y;
        break;
    case PORTRAIT_REVERSE:
        tmp = *x;
        *x = EPD_WIDTH - *y;
        *y = tmp;
        break;
    }
    epdgl_abs_update(*x, *y); 
}

void
epdgl_init()
{
    epd_set_lut_slow();
    epdgl_clear();
    epdgl_abs_reset();
}

bool
epdgl_update_screen(epd_update_t u)
{
    if (!epd_is_idle()) return false;
    if (!UPDATE_REQUEST) return true;

    int32_t upd_area = epdgl_update_area();
    epd_update_frame(EPDGL_BUF);

    if (FORCE_SLOW_UPDATE) u = EPD_SLOW;

    switch (u) {
    case EPD_PART:
        if ((part_refresh_count < PART_REFRESH_LIMIT) && 
            (upd_area < (EPD_WIDTH * EPD_HEIGHT / 8))
        ) {
            ++part_refresh_count;
            UPDATE_REQUEST = true;  // partial refresh must be followed by full
            epd_refresh_part(UPD_MIN_X,UPD_MIN_Y,UPD_MAX_X,UPD_MAX_Y);
            break;
        }
    case EPD_FAST:
        part_refresh_count = 0;
        if (fast_refresh_count < FAST_REFRESH_LIMIT) {
            ++fast_refresh_count;
            epd_refresh_fast();
            break;
        }
    case EPD_SLOW:
        fast_refresh_count = 0;
        FORCE_SLOW_UPDATE = false;
        epd_refresh_slow();
        break;
    }

    UPDATE_REQUEST = false;
    epdgl_abs_reset();
    return true;
}

void
epdgl_set_orientation(epd_orientation_t o)
{
    EPDGL_ROT = o;
}

static void
epdgl_draw_abs_pixel(int32_t x, int32_t y, epd_color_t c)
{
    if (x >= 0 && x < EPD_WIDTH && y >= 0 && y < EPD_HEIGHT) {
        switch (c) {
        case EPD_GREY:
            if ((x + y) & 0x01) goto white_pixel;
            goto black_pixel;
        case EPD_WHITE:
white_pixel:
            EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] |= (0x80 >> (x & 0x07));
            break;
        case EPD_BLACK:
black_pixel:
            EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] &= ~(0x80 >> (x & 0x07));
            break;
        }
    }
}

static void
epdgl_draw_abs_horizontal(int32_t x, int32_t y, int32_t l, epd_color_t c)
{
    uint8_t fill, mask = 0xff;
    switch (c)
    {
    case EPD_WHITE: fill = 0xff; break;
    case EPD_BLACK: fill = 0x00; break;
    case EPD_GREY: fill = (y & 0x01) ? 0xaa : 0x55; break;
    }

    int32_t pre = x & 0x07;
    int32_t post = (x + l) & 0x07;

    mask = 0xff >> pre;
    if (l < pre) {
        mask = mask & (0xff << (8 - post));
    }

    if (pre > 0) {
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] |= mask;
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] &= (fill | ~mask);
        x += (8 - pre);
        l -= (8 - pre);
    }

    while (l >= 8) {
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] = fill;
        x += 8;
        l -= 8;
    }

    if (l > 0) {
        mask = 0xff << (8 - post);
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] |= mask;
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] &= (fill | ~mask);
    }
}

static void
epdgl_draw_abs_vertical(int32_t x, int32_t y, int32_t l, epd_color_t c)
{
    uint8_t fill, mask = 0x80 >> (x & 0x07);
    switch (c)
    {
    case EPD_WHITE: fill = 0xff; break;
    case EPD_BLACK: fill = 0x00; break;
    case EPD_GREY: fill = (y & 0x01) ? 0xaa : 0x55; break;
    }

    while (l > 0) {
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] |= mask;
        EPDGL_BUF[(x + (y * EPD_WIDTH)) >> 3] &= (fill | ~mask);
        if (c == EPD_GREY) fill = ~fill;
        --l;
        ++y;
    }
}

void
epdgl_draw_pixel(int32_t x, int32_t y, epd_color_t c)
{
    epdgl_absolute(&x, &y);

    epdgl_draw_abs_pixel(x, y, c);

    UPDATE_REQUEST = true;
}

void
epdgl_clear(void)
{
    for (int i = 0; i < BUF_SIZE; ++i) {
        EPDGL_BUF[i] = 0xff;
    }

    UPDATE_REQUEST = true;
    FORCE_SLOW_UPDATE = true;
}

void
epdgl_invert(void)
{
    for (int i = 0; i < BUF_SIZE; ++i) {
        EPDGL_BUF[i] = ~EPDGL_BUF[i];
    }

    UPDATE_REQUEST = true;
    FORCE_SLOW_UPDATE = true;
}

void
epdgl_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, epd_color_t c)
{
    epdgl_absolute(&x0, &y0);
    epdgl_absolute(&x1, &y1);

    /* Bresenham algorithm */
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while((x0 != x1) || (y0 != y1)) {
        epdgl_draw_abs_pixel(x0, y0, c);
        if (2 * err >= dy) {     
            err += dy;
            x0 += sx;
            if (sx > 0 && x0 > x1) break;
            if (sx < 0 && x0 < x1) break;
        }
        if (2 * err <= dx) {
            err += dx; 
            y0 += sy;
            if (sy > 0 && y0 > y1) break;
            if (sy < 0 && y0 < y1) break;
        }
    }

    UPDATE_REQUEST = true;
}

void
epdgl_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, epd_color_t c)
{
    int32_t xw = x + w;
    int32_t yh = y + h;
    epdgl_absolute(&x, &y);
    epdgl_absolute(&xw, &yh);

    int32_t minx = xw < x ? xw : x;
    int32_t miny = yh < y ? yh : y;
    int32_t maxx = xw < x ? x : xw;
    int32_t maxy = yh < y ? y : yh;

    w = maxx - minx;
    h = maxy - miny;

    epdgl_draw_abs_horizontal(minx, miny, w, c);
    epdgl_draw_abs_horizontal(minx, maxy - 1, w, c);
    epdgl_draw_abs_vertical(minx, miny, h, c);
    epdgl_draw_abs_vertical(maxx - 1, miny, h, c);

    UPDATE_REQUEST = true;
}

void
epdgl_fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, epd_color_t c)
{
    int32_t xw = x + w;
    int32_t yh = y + h;
    epdgl_absolute(&x, &y);
    epdgl_absolute(&xw, &yh);

    int32_t minx = xw < x ? xw : x;
    int32_t miny = yh < y ? yh : y;
    int32_t maxx = xw < x ? x : xw;
    int32_t maxy = yh < y ? y : yh;

    w = maxx - minx;
    h = maxy - miny;

    while (h > 0) {
        epdgl_draw_abs_horizontal(minx, miny, w, c);
        ++miny;
        --h;
    }

    UPDATE_REQUEST = true;
}

void
epdgl_draw_circle(int32_t x, int32_t y, int32_t r, epd_color_t c)
{
    epdgl_absolute(&x, &y);
    epdgl_abs_update(x - r, y - r);
    epdgl_abs_update(x + r, y + r);
    /* Bresenham algorithm */
    int x_pos = -r;
    int y_pos = 0;
    int err = 2 - 2 * r;
    int e2;

    do {
        epdgl_draw_abs_pixel(x - x_pos, y + y_pos, c);
        epdgl_draw_abs_pixel(x + x_pos, y + y_pos, c);
        epdgl_draw_abs_pixel(x + x_pos, y - y_pos, c);
        epdgl_draw_abs_pixel(x - x_pos, y - y_pos, c);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
              e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);

    UPDATE_REQUEST = true;
}

void
epdgl_fill_circle(int32_t x, int32_t y, int32_t r, epd_color_t c)
{
    epdgl_absolute(&x, &y);
    epdgl_abs_update(x - r, y - r);
    epdgl_abs_update(x + r, y + r);
    /* Bresenham algorithm */
    int x_pos = -r;
    int y_pos = 0;
    int err = 2 - 2 * r;
    int e2;

    do {
        epdgl_draw_abs_pixel(x - x_pos, y + y_pos, c);
        epdgl_draw_abs_pixel(x + x_pos, y + y_pos, c);
        epdgl_draw_abs_pixel(x + x_pos, y - y_pos, c);
        epdgl_draw_abs_pixel(x - x_pos, y - y_pos, c);
        epdgl_draw_abs_horizontal(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, c);
        epdgl_draw_abs_horizontal(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, c);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
              e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);

    UPDATE_REQUEST = true;
}

void
epdgl_set_cursor(int32_t x, int32_t y)
{
    CURSOR_X = x;
    CURSOR_Y = y;
}

void
epdgl_draw_char(char s, text_config_t * cfg)
{
    font_t * f = cfg->font;
    uint32_t fbytes = f->GlyphHeight * f->GlyphBytesWidth;
    const uint8_t * charmap = &f->GlyphBitmaps[(s - f->FirstAsciiCode) * fbytes];
    int32_t x = CURSOR_X;
    int32_t y = CURSOR_Y;

    for (int32_t i = 0; i < f->GlyphHeight; ++i) {
        for (int32_t j = 0; j < f->GlyphBytesWidth; ++j) {
            uint8_t data = *charmap;
            for (uint8_t k = 0x80; k > 0; k = k >> 1) {
                if (data & k) epdgl_draw_pixel(x, y, cfg->color);
                ++x;
            }
            ++charmap;
        }
        x = CURSOR_X;
        ++y;
    }

    if (f->FixedWidth) {
        CURSOR_X += f->FixedWidth;
    } else {
        CURSOR_X += f->GlyphWidth[s - f->FirstAsciiCode];
    }
}

void
epdgl_draw_int(int32_t i, text_config_t * cfg)
{
    if (i < 0) {
        i *= -1;
        epdgl_draw_char('-', cfg);
        epdgl_draw_int(i, cfg);
    } else {
        if (i/10) epdgl_draw_int(i/10, cfg);
        epdgl_draw_char(0x30 + (i%10), cfg);
    }
}

void
epdgl_draw_string(const char * s, text_config_t * cfg)
{
    while (*s != 0) {
        epdgl_draw_char(*s, cfg);
        ++s;
    }
}

void
epdgl_plot(int32_t * x, int32_t * y, int32_t l, plot_config_t cfg)
{
    int32_t i;
    int32_t min_x = INT32_MAX, min_y = INT32_MAX;
    int32_t max_x = INT32_MIN, max_y = INT32_MIN;

    for (i = 0; i < l; ++i) {
        if (x[i] > max_x) max_x = x[i];
        if (x[i] < min_x) min_x = x[i];
        if (y[i] > max_y) max_y = y[i];
        if (y[i] < min_y) min_y = y[i];
    }

    int32_t cfg_xr = cfg.max_x - cfg.min_x;
    int32_t xr = max_x - min_x;
    int32_t bx = cfg.min_x - min_x;

    int32_t cfg_yr = cfg.max_y - cfg.min_y;
    int32_t yr = max_y - min_y;
    int32_t by = cfg.min_y - min_y;

    for (i = 0; i < l - 1; ++i) {
        int32_t x0 = ((x[i] + bx) * cfg_xr) / xr;
        int32_t x1 = ((x[i + 1] + bx) * cfg_xr) / xr;
        int32_t y0 = ((max_y - y[i] + by) * cfg_yr) / yr;
        int32_t y1 = ((max_y - y[i + 1] + by) * cfg_yr) / yr;
        epdgl_draw_line(x0, y0, x1, y1, cfg.color);
    }
}
