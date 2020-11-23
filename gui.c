/*
    easi graphical interface source file

    Authors: Pete Fan and Sai Koukuntla
    Date: Nov 22
*/
#include "keypad.h"
#include "epdgl.h"
#include "easi_globals.h"
#include "gui.h"
#include <string.h>


/*
    status bar variables
*/
static char * appString = "algebraic";
static char * keyModeString = "normal";

/*
    function bar variables
*/
typedef enum bar_item {
    FOLDER,
    FUNCTION,
    CONSTANT,
    MODE,
    ESC
} bar_item_t;

typedef struct bar_function {
    bar_item_t type;
    char * name;
    struct bar_function * subitems;
} bar_fn_t;

bar_fn_t MODE_ARR[] = {
    {
        MODE,
        "ALG",
        0
    },
    {
        MODE,
        "RPN",
        0
    },
    {
        MODE,
        "GRPH",
        0
    },
    {
        MODE,
        "VOLT",
        0
    },
    {
        MODE,
        "PP",
        0
    },
    {
        ESC,
        "ESC",
        0
    },
};

bar_fn_t FN_ARR[] = {
    {
        FOLDER,
        "MODE",
        MODE_ARR,
    },
    {
        CONSTANT,
        "null",
        0
    },
    {
        CONSTANT,
        "null",
        0
    },
    {
        CONSTANT,
        "null",
        0
    },
    {
        CONSTANT,
        "null",
        0
    },
    {
        CONSTANT,
        "null",
        0
    },
};


static bar_fn_t * ACTIVE_BAR = FN_ARR;

static text_config_t bar_fnt = {&Consolas14, EPD_WHITE};

void
draw_fn_bar()
{
    epdgl_fill_rect(0, 380, 300, 20, EPD_BLACK);
    for (int i = 0; i < 6; ++i) {
        epdgl_set_cursor(10 + i * 50, 382);
        epdgl_draw_string(ACTIVE_BAR[i].name, &bar_fnt);
    }
}

void
update_fn_bar(key_t k)
{
    bar_fn_t fn = ACTIVE_BAR[k];
    switch (fn.type) {
    case FOLDER:
        ACTIVE_BAR = fn.subitems;
        break;
    case MODE:
        ACTIVE_BAR = FN_ARR;
        EASI_MODE = k;
        update_status_bar();
        break;
    case ESC:
        ACTIVE_BAR = FN_ARR;
        break;
    }
    draw_fn_bar();
}

void
draw_status_bar(){
    epdgl_fill_rect(0, 0, 300, 20, EPD_BLACK);

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
// PD1 has battery status
void
update_status_bar()
{
    // update key string
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

    // update app string
    switch(EASI_MODE){
        case ALG:
            appString = "algebraic";
            break;
        case RPN:
            appString = "postfix";
            break;
        case GRPH:
            appString = "graph";
            break;
        case VOLT:
            appString = "voltmeter";
            break;
        case PP:
            appString = "pee pee";
            break;
    }

    draw_status_bar();

}
