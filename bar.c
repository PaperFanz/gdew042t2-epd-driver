
#include "bar.h"
#include "easi.h"
#include "easi_globals.h"
#include "rpn.h"
#include "alg.h"
#include "graph.h"

/*
*/
char * appString = "algebraic";
char * keyModeString = "normal";

/*
    function bar variables
*/

const bar_fn_t FUNCTIONS[5][6] = {{
    // Algebraic bar
    {FOLDER, "MODE", &MODE_BAR, 0},
    {FUNCTION, "ANS", 0, &alg_ans},
    {FUNCTION, "VOLT", 0, &alg_volt},
    {CONSTANT, "null", 0, 0},
    {FUNCTION, "CLR", 0, &clear_input},
    {FOLDER, "OPT", &SETTINGS_BAR, 0},},

    // RPN bar
    {{FOLDER, "MODE", &MODE_BAR, 0},
    {FUNCTION, "SWAP", 0, &rpn_swap},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {FUNCTION, "CLR", 0, &clear_input},
    {FOLDER, "OPT", &SETTINGS_BAR, 0},},

    // Graphing bar
    {{FOLDER, "MODE", &MODE_BAR, 0},
    {FUNCTION, "FIG", 0, &graph_view_figure},
    {FUNCTION, "EQN", 0, &graph_set_eqn},
    {FUNCTION, "WIN", 0, &graph_set_window},
    {CONSTANT, "VAL", 0, &graph_trace},
    {FUNCTION, "CLR", 0, &clear_input},},

    // Voltmeter bar
    {{FOLDER, "MODE", &MODE_BAR, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {FUNCTION, "CLR", 0, &clear_input},
    {FOLDER, "OPT", &SETTINGS_BAR, 0},},

    // PP bar
    {{FOLDER, "MODE", &MODE_BAR, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {CONSTANT, "null", 0, 0},
    {FUNCTION, "CLR", 0, &clear_input},
    {FOLDER, "OPT", &SETTINGS_BAR, 0},},
};

#define FUNCTIONS_MAX (sizeof(FUNCTIONS[0]) / sizeof(bar_fn_t))
bar_t FUNCTION_BAR[]= {
    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[0]},

    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[1]},

    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[2]},

    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[3]},

    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[4]},

    {0,
    FUNCTIONS_MAX,
    FUNCTIONS[5]}
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

text_config_t slp_fnt = {&Consolas20, EPD_BLACK};

void
sleep_mode(void)
{
    epdgl_clear();
    epdgl_set_cursor(150 - 50, 190);
    epdgl_draw_string("(UwU) zzZ", &slp_fnt);
    while(!epdgl_update_screen(EPD_SLOW));
    FULL_UPDATE = true;
    SLEEP = true;
    WAKE = false;
    ACTIVE_BAR = &FUNCTION_BAR[0];
    epd_sleep();
}

const bar_fn_t SETTINGS[] = {
    {FUNCTION, "REFR", 0, &req_full_update},
    {FUNCTION, "SLP", 0, &sleep_mode},
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
 
bar_t * ACTIVE_BAR = &FUNCTION_BAR[0];

const bar_fn_t *
get_active_fn(int32_t i)
{
    return &(ACTIVE_BAR->functions[ACTIVE_BAR->index + i]);
}

text_config_t bar_fnt = {&Consolas14, EPD_WHITE};
