/*
    status and function bar globals

    Author: Pete Fan
    Date: Nov 22
*/

#ifndef BAR_H
#define BAR_H

#include "epdgl.h"

/*
    status bar variables
*/
extern char * appString;
extern char * keyModeString;

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
    struct bar * subitems;
    void (* function)(void);
} bar_fn_t;

typedef struct bar {
    uint16_t index;
    uint16_t max;
    const bar_fn_t * functions;
} bar_t;

const bar_fn_t * get_active_fn(int32_t i);

extern bar_t MODE_BAR;

extern bar_t FUNCTION_BAR;

extern bar_t SETTINGS_BAR;

extern bar_t * ACTIVE_BAR;

extern text_config_t bar_fnt;

#endif
