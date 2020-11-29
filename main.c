/*
    entry point for the ePaper graphing calculator project

    Author: Pete Fan
    Date: Nov 21
*/

// stdlib includes
#include <stdio.h>
#include <stdint.h>

// valvanoware includes
#include "../inc/tm4c123gh6pm.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"
#include "../inc/PLL.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer1A.h"

// local includes
#include "easi.h"
#include "gdew042t2.h"
#include "epdgl.h"
#include "keypad.h"
#include "voltmeter.h"
#include "ExpressionTree.h"
#include "AlphaValues.h"

void portFinit(void)
{
    SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
    while((SYSCTL_PRGPIO_R&0x20) == 0){};// allow time for clock to stabilize
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
    // only PF0 needs to be unlocked, other bits can't be locked
    GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
    GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
    GPIO_PORTF_DATA_R = 0;            // LEDs off
}

void waitPF4(void)
{
    while (((~(PF4) & 0x10) >> 4) == 0);
    while (((~(PF4) & 0x10) >> 4) == 1);
}

text_config_t t_cfg = {&Consolas20, EPD_BLACK};

const char * KEY_STRINGS[] = {
    "F1",      "F2",    "F3",   "F4",   "F5",   "F6",    
             "ENTER", "ALPHA", "CTRL", "UP",   "BACKSPACE",     
    "SIN",     "COS",   "TAN",  "LEFT", "DOWN", "RIGHT",    
    "CONST",   "VAR",   "POW",  "LOG",  "ROOT",   
    "EE",      "N7",    "N8",   "N9",   "DIV",        
    "PARENTH", "N4",    "N5",   "N6",   "MUL",       
    "BASE",    "N1",    "N2",   "N3",   "SUB",      
    "MATH",    "N0",    "DEC",  "SIGN", "ADD",
    "KEY_NUM",
    /* alpha mode */
    "A",       "B",      "C",
    "D",       "E",      "F",     "G",      "H", 
    "I",       "J",      "K",     "L",      "M", 
    "N",       "O",      "P",     "Q",      "R", 
    "S",       "T",      "U",     "V",      "W", 
    "X",       "Y",      "Z",     "SPACE",  "SHIFT",
};

const char * MODE_STRINGS[] = {
    "NORM",
    "ALPH",
    "CTRL",
    "A_LK",
    "C_LK"
};

void
print_key(key_event_t * e)
{
    epdgl_draw_string(KEY_STRINGS[KEYMAP[e->key]], &t_cfg);
    if (e->k_action == KEY_UP) {
        epdgl_draw_string(", up", &t_cfg);
    } else {
        epdgl_draw_string(", dn", &t_cfg);
    }
}

void
demo_keys(void)
{
    int y = 10;
    key_event_t ev;
    epdgl_set_cursor(10,y);
    while(1) {
        while (key_fifo_get(&ev) != FIFO_EMPTY_ERR) {
            if (y > 370) {
                y = 10;
                epdgl_clear();
            }
            epdgl_set_cursor(10, y);
            print_key(&ev);
            y+=20;
        }

        epdgl_update_screen(EPD_FAST);
    }
}

void demo_alg(void){
	ExpressionTree exp;
	
	ExpressionTree_Clear(&exp);
	set_key_alpha_value(A, 3.4);
	
	//2.2 * A * cos(0) = 7.48
	ExpressionTree_ModifyExpression(&exp, N2);
	ExpressionTree_ModifyExpression(&exp, DEC);
	ExpressionTree_ModifyExpression(&exp, N2);
	ExpressionTree_ModifyExpression(&exp, MUL);
	ExpressionTree_ModifyExpression(&exp, A);
	ExpressionTree_ModifyExpression(&exp, MUL);
	ExpressionTree_ModifyExpression(&exp, COS);
	ExpressionTree_ModifyExpression(&exp, N0);
	
	char exp_string[20];
	Expression_ToString(exp_string, exp);
	
    epdgl_set_cursor(20, 20);
	epdgl_draw_string(exp_string, &t_cfg);
	while (!epdgl_update_screen(EPD_FAST));
	
	waitPF4();
	
	ExpressionTree_Evaluate(&exp);
	
    epdgl_set_cursor(20, 40);
	Expression_ToString(exp_string, exp);
	
	epdgl_draw_string(exp_string, &t_cfg);
	while (!epdgl_update_screen(EPD_FAST));
	
	while(1){
	}
}

int main (void)
{
	PLL_Init(Bus80MHz);   // 80 MHz
	
    DisableInterrupts();  // Disable interrupts until finished with inits
	
    epd_init(); // initialize e-paper display
    portFinit();
    keypad_init();
    voltmeter_init();

    Timer1A_Init(&keypad_scan, 80000000/160, 2);

		EnableInterrupts();

    epdgl_clear();
    while(!epdgl_update_screen(EPD_SLOW));

    // set display orientation (see epdgl.h for options)
    epdgl_set_orientation(PORTRAIT);
	
	demo_alg();
	
    easi_init();
    while (1) {
        easi_run();
    }
}

/*
void
demo_calc(void)
{
    epdgl_clear();
    char * expr = "5*5";
    char * ans = "25";

    int cy = 360;
    int cx = 20;

    for (char * c = expr; *c != '\0'; ++c) {
        waitPF4();
        epdgl_draw_char(cx,cy,*c,&CascadiaMono24,EPD_BLACK);
        cx += CascadiaMono24.FixedWidth;
        while (!epdgl_update_screen(EPD_FAST));
    }
    
    epdgl_clear();
    waitPF4();

    epdgl_draw_string(20, 320, expr, &CascadiaMono24, EPD_BLACK);
    epdgl_draw_string(280 - 28, 360, ans, &CascadiaMono24, EPD_BLACK);
    while (!epdgl_update_screen(EPD_SLOW));
}

void
demo_status(void)
{
    epdgl_clear();
    epdgl_draw_string(20, 20, "Device ID: 0", &CascadiaMono24, EPD_BLACK);
    epdgl_draw_line(20, 46, 380, 46, EPD_BLACK);

    epdgl_draw_string(20, 54, "Scheduled Sensors:", &CascadiaMono24, EPD_BLACK);
    
    epdgl_draw_string(40, 15 + 68, "temperature..........5s", &CascadiaMono24, EPD_BLACK);
    epdgl_draw_string(40, 15 + 92, "pressure.....[disabled]", &CascadiaMono24, EPD_GREY);
    epdgl_draw_string(40, 15 + 116, "angle................1s", &CascadiaMono24, EPD_BLACK);
    
    epdgl_fill_rect(0, 300 - 36, 400, 36, EPD_BLACK);
    epdgl_draw_string(20, 300 - 30, "Connected: trident-5G", &CascadiaMono24, EPD_WHITE);
    while (!epdgl_update_screen(EPD_SLOW));
}

void demo_lines(void)
{
    // clear frame buffer
    epdgl_clear();

    // horizontal black line
    epdgl_draw_line(10,10,390,10,EPD_BLACK);

    // vertical black line
    epdgl_draw_line(10,10,10,290,EPD_BLACK);

    // diagonal black line
    epdgl_draw_line(10,10,390,290,EPD_BLACK);

    // horizontal grey line
    epdgl_draw_line(20,20,380,20,EPD_GREY);

    // vertical grey line
    epdgl_draw_line(20,20,20,280,EPD_GREY);

    // black text
    epdgl_draw_string(30,260,"demo_lines", &CascadiaMono24, EPD_BLACK);

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

void demo_rect(void)
{
    // clear frame buffer
    epdgl_clear();

    // draw black rectangle
    epdgl_draw_rect(10,10,30,40,EPD_BLACK);

    // draw grey rectangle
    epdgl_draw_rect(50,10,30,40,EPD_GREY);

    // fill black rectangle
    epdgl_fill_rect(10,60,30,40,EPD_BLACK);
    
    // fill grey rectangle
    epdgl_fill_rect(50,60,30,40,EPD_GREY);
    
    // inset white rectangle
    epdgl_fill_rect(60,70,10,20,EPD_WHITE);

    // black text
    epdgl_draw_string(40,260,"demo_rect", &CascadiaMono24, EPD_BLACK);

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

void demo_circle(void)
{
    // clear frame buffer
    epdgl_clear();

    // draw black circle
    epdgl_draw_circle(40,40,30,EPD_BLACK);

    // draw grey circle
    epdgl_draw_circle(120,40,30,EPD_GREY);

    // fill black circle
    epdgl_fill_circle(40,120,30,EPD_BLACK);

    // fill grey circle
    epdgl_fill_circle(120,120,30,EPD_GREY);

    // inset white circle
    epdgl_fill_circle(120,120,20,EPD_WHITE);

    // black text
    epdgl_draw_string(50,260,"demo_circle", &CascadiaMono24, EPD_BLACK);

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

void demo_plot(void)
{
    // clear frame buffer
    epdgl_clear();

    // x and y for plotting
    int32_t x[6] = {0,10,20,30,40,50};
    int32_t y[6] = {0,10,40,15,34,60};
    
    // plot configuration
    plot_config_t cfg = {
        0, // min x
        399, // max x
        0, // min y
        299, // max y
        EPD_BLACK // color
    };
    
    // plotting function
    epdgl_plot(x, y, 6, cfg);

    // black text
    epdgl_draw_string(70,260,"demo_plot", &CascadiaMono24, EPD_BLACK);

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

void demo_all(void)
{
    demo_lines();
    waitPF4();

    demo_rect();
    waitPF4();

    demo_circle();
    waitPF4();

    demo_text();
    waitPF4();

    demo_plot();
    waitPF4();
}
*/
