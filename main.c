// ST7735TestMain.c
// Runs on LM4F120/TM4C123
// Test the functions in ST7735.c by printing basic
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// March 30, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"
#include "../inc/PLL.h"
#include "gdew042t2.h"
#include "epdgl.h"

#include "image.h"

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

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

void demo_text(void)
{
    // clear frame buffer
    epdgl_clear();

    // black text
    epdgl_draw_string(10,10,"Hello, World!", &CascadiaMono24, EPD_BLACK);

    // grey text
    epdgl_draw_string(10,40,"Dreary World...", &CascadiaMono24, EPD_GREY);
    
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

    // request screen update
    while (!epdgl_update_screen(EPD_FAST));
}

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
    while (((~(PF4) & 0x10) >> 4) == 0){

    }
    while (((~(PF4) & 0x10) >> 4) == 1){
        
    }
}

int main (void)
{
	PLL_Init(Bus80MHz);   // 80 MHz
    DisableInterrupts();  // Disable interrupts until finished with inits
    
    epd_init(); // initialize e-paper display
    portFinit();

	EnableInterrupts();

    // clear frame buffer and refresh screen
    epdgl_clear();
    epdgl_update_screen(EPD_SLOW);  // slow update prevents ghosting, but is ~3s
                                    // fast update is ~0.3s but lower contrast

    // set display orientation (see epdgl.h for options)
    epdgl_set_orientation(LANDSCAPE);
    
    while (1) {
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
}
