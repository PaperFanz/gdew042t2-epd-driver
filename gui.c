/*
    easi graphical interface source file

    Authors: Pete Fan and Sai Koukuntla
    Date: Nov 22
*/

#include <string.h>

#include "keypad.h"
#include "epdgl.h"
#include "easi_globals.h"
#include "gui.h"
#include "bar.h"
#include "rpn.h"
#include "graph.h"
#include "voltmeter.h"

void
draw_fn_bar()
{
    epdgl_fill_rect(0, SCREEN_H - FN_BAR_H, FN_BAR_W, FN_BAR_H, EPD_BLACK);
    for (int i = 0; i < 6; ++i) {
        epdgl_set_cursor(10 + i * 50, 382);
        epdgl_draw_string(get_active_fn(i)->name, &bar_fnt);
    }
}

void
update_fn_bar(key_t k)
{
    const bar_fn_t * fn = get_active_fn(k);
    switch (fn->type) {
    case FOLDER:
        ACTIVE_BAR = fn->subitems;
        break;
    case MODE:
        EASI_MODE = (easi_mode_t) k;
		ACTIVE_BAR = &FUNCTION_BAR[EASI_MODE];
        epdgl_fill_rect(0, STATUS_BAR_H, 300, 400 - FN_BAR_H, EPD_WHITE);
        switch (k) {
        case RPN: rpn_init(); break;
        case GRPH: graph_init(); break;
        }
        update_status_bar();
        break;
    case FUNCTION:
        (*fn->function)();
        break;
    case ESC:
        ACTIVE_BAR = &FUNCTION_BAR[EASI_MODE];
        break;
		default:
				break;
    }
    draw_fn_bar();
}

void
draw_status_bar(){
    epdgl_fill_rect(0, 0, STATUS_BAR_W, STATUS_BAR_H, EPD_BLACK);

    epdgl_set_cursor(8, 2);
    epdgl_draw_string(keyModeString, &bar_fnt);

    // Draw app string
    int app_start = strlen(appString) * 8;
    if(app_start%2) app_start = 146 - app_start/2;
    else app_start = 150 - app_start/2;

    epdgl_set_cursor(app_start, 2);
    epdgl_draw_string(appString, &bar_fnt);

    // Draw battery %
    int bat_dig = (GLOB_BATTERY == 100) ? 3 : ((GLOB_BATTERY >= 10) ? 2 : 1);
    epdgl_set_cursor(291 - (bat_dig + 1) * 8, 2);
    epdgl_draw_int(GLOB_BATTERY, &bar_fnt);
    epdgl_draw_string("%", &bar_fnt);

    //TODO: 
    //TODO: battery symbol 
}

// PD1 has battery status
void
update_status_bar()
{
    // update key string
    switch(KEY_MODE){
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
