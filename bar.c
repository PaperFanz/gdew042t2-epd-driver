
#include "bar.h"
#include "easi.h"
#include "easi_globals.h"

/*
*/
char * appString = "algebraic";
char * keyModeString = "normal";

/*
    function bar variables
*/

const bar_fn_t FUNCTIONS[] = {
    {FOLDER, "MODE", &MODE_BAR, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {FUNCTION, "CLR", 0, &clear_input},
    {FOLDER, "OPT", &SETTINGS_BAR, 0},
};

#define FUNCTIONS_MAX (sizeof(FUNCTIONS) / sizeof(bar_fn_t))
bar_t FUNCTION_BAR = {
    0,
    FUNCTIONS_MAX,
    FUNCTIONS
};

const bar_fn_t MODES[] = {
    {MODE, "ALG", 0, 0},
    {MODE, "RPN", 0, 0},
    {MODE, "GRPH", 0, 0},
    {MODE, "VOLT", 0, 0},
    {MODE, "PP", 0, 0},
    {ESC, "ESC", 0, 0},
};

#define MODES_MAX (sizeof(MODES) / sizeof(bar_fn_t))
bar_t MODE_BAR = {
    0,
    MODES_MAX,
    MODES
};

void
req_full_update(void)
{
    FULL_UPDATE = true;
}

const bar_fn_t SETTINGS[] = {
    {FUNCTION, "REFR", 0, &req_full_update},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {ESC, "ESC", 0, 0},
};

#define SETTINGS_MAX (sizeof(SETTINGS) / sizeof(bar_fn_t))
bar_t SETTINGS_BAR = {
    0,
    SETTINGS_MAX,
    SETTINGS
};

bar_t * ACTIVE_BAR = &FUNCTION_BAR;

const bar_fn_t *
get_active_fn(int32_t i)
{
    return &(ACTIVE_BAR->functions[ACTIVE_BAR->index + i]);
}

text_config_t bar_fnt = {&Consolas14, EPD_WHITE};
