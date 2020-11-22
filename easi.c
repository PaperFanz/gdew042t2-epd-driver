/*
    source file for the Embedded Arithmetic Shell Interface (EASI)

    Author: Pete Fan
    Date: Nov 21
*/

#include "keypad.h"
#include "epdgl.h"
#include <string.h>

int keyMode;
char * appString;

typedef enum bar_item {
    FOLDER,
    FUNCTION,
    CONSTANT
} bar_item_t;

typedef struct bar_function {
    bar_item_t type;
    char * name;
} bar_fn_t;

typedef enum key_mode {
    M_NORMAL,
    M_ALPHA,
    M_CTRL
} key_mode_t;

char * FN_ARR[] = {
    "fn1",
    "fn2",
    "fn3",
    "fn4",
    "fn5",
    "fn6",
};

text_config_t bar_fnt = {&Consolas14, EPD_WHITE};

void
easi_init()
{
    appString = "pee pee";
    keyMode = M_NORMAL;
}

static void
update_fn_bar()
{
    epdgl_fill_rect(0, 380, 300, 20, EPD_BLACK);
    for (int i = 0; i < 6; ++i) {
        epdgl_set_cursor(10 + i * 50, 382);
        epdgl_draw_string(FN_ARR[i], &bar_fnt);
    }
}

// PD1 has battery status
static void
update_status_bar()
{
    char* keyModeString;
    epdgl_fill_rect(0, 0, 300, 20, EPD_BLACK);

    // Draw key mode
    switch(keyMode){
        case M_NORMAL:
            keyModeString = "normal";
            break;
        case M_ALPHA:
            keyModeString = "alpha";
            break;
        case M_CTRL:
            keyModeString = "control";
            break;
    }
    epdgl_set_cursor(2, 2);
    epdgl_draw_string(keyModeString, &bar_fnt);

    // Draw app string
    int app_start = strlen(appString) * 8;
    if(app_start%2) app_start = 146 - app_start/2;
    else app_start = 150 - app_start/2;

    epdgl_set_cursor(app_start, 2);
    epdgl_draw_string(appString, &bar_fnt);

    // Draw battery %
    epdgl_set_cursor(275, 2);
    epdgl_draw_string("99%", &bar_fnt);

    //TODO: 
    //TODO: battery symbol 

}

static void
parse_input(uint8_t raw_key)
{
    key_t key = KEYMAP[raw_key];
    switch(key){
        case ALPHA:
            if(keyMode == M_ALPHA) keyMode = M_NORMAL;
            else if(keyMode == M_NORMAL) keyMode = M_ALPHA;
            break;
        case 
    }

}

void
easi_run()
{
    easi_init();
    update_status_bar();
    update_fn_bar();

    while (!epdgl_update_screen(EPD_FAST));
}
