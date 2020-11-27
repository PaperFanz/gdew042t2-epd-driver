/*

    gdew042t2.c - source file for the gdew042t2 4.2 inch e-paper display

    http://www.e-paper-display.com/products_detail/productId=321.html

    author: Pete Fan
    date: Oct 16, 2020

    Hardware Connections (TM4C123GHPM)
        VCC -> 3v3
        GND -> GND
        DIN -> PA5
        CLK -> PA2
        CS  -> PA3 (GND if single-slave SPI)
        DC  -> PA6
        RST -> PA7
        BSY -> PA4

*/

#include "gdew042t2.h"
#include "../inc/tm4c123gh6pm.h"

#define TM4C123GHPM  // comment out if using different mcu
#define SINGLE_SLAVE // comment out if using multiple slaves

typedef enum lut_mode {
    LUT_FAST,
    LUT_PART,
    LUT_SLOW
} lut_mode_t;

static lut_mode_t ACTIVE_LUT = LUT_SLOW;

#define FT1 7
#define FT2 14
#define FT3 0
#define FT4 0

const unsigned char lut_vcom0_fast[] = {
    0x00, FT1, FT2, FT3, FT4, 1, // 00 00 00 00    
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0, 0, 0,
};

const unsigned char lut_ww_fast[] = {
    0xA0, FT1, FT2, FT3, FT4, 1, // 10 10 00 00
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
};

const unsigned char lut_bw_fast[] = {
    0xA0, FT1, FT2, FT3, FT4, 1, // 10 10 00 00
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
};

const unsigned char lut_bb_fast[] = {
    0x50, FT1, FT2, FT3, FT4, 1, // 01 01 00 00
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
};

const unsigned char lut_wb_fast[] = {
    0x50, FT1, FT2, FT3, FT4, 1, // 01 01 00 00
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
};

#define ST1 8  // orig 0x08
#define ST2 18  // orig 0x12
#define ST3 20  // orig 0x14
#define ST4 40  // orig 0x28

const uint8_t lut_vcom0_slow[] = {
    0x00, ST1,   0,   0,   0, 2,
    0x60, ST4, ST4,   0,   0, 1,
    0x00, ST3,   0,   0,   0, 1,
    0x00, ST2, ST2,   0,   0, 1,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0, 0, 0
};

const uint8_t lut_ww_slow[] = {
    0x40, ST1,   0,   0,   0, 2,
    0x90, ST4, ST4,   0,   0, 1,
    0x40, ST3,   0,   0,   0, 1,
    0xA0, ST2, ST2,   0,   0, 1,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
};

const uint8_t lut_bw_slow[] = {
    0x40, ST1,   0,   0,   0, 2,
    0x90, ST4, ST4,   0,   0, 1,
    0x40, ST3,   0,   0,   0, 1,
    0xA0, ST2, ST2,   0,   0, 1,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0, 
};

const uint8_t lut_bb_slow[] = {
    0x80, ST1,   0,   0,   0, 2,
    0x90, ST4, ST4,   0,   0, 1,
    0x80, ST3,   0,   0,   0, 1,
    0x50, ST2, ST2,   0,   0, 1,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,       
};

const uint8_t lut_wb_slow[] = {
    0x80, ST1,   0,   0,   0, 2,
    0x90, ST4, ST4,   0,   0, 1,
    0x80, ST3,   0,   0,   0, 1,
    0x50, ST2, ST2,   0,   0, 1,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,
    0x00,   0,   0,   0,   0, 0,     
};

// new waveform created by Jean-Marc Zingg for the actual panel
#define T1 4  // color change charge balance pre-phase
#define T2 8  // color change or sustain charge balance pre-phase
#define T3 0 // color change or sustain phase
#define T4 4  // color change phase

const unsigned char lut_vcom0_partial[] = {
    0x00, T1, T2, T3, T4, 1, // 00 00 00 00
    0x00,  1,  0,  0,  0, 1, // gnd phase       
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0, 0, 0,
};


const unsigned char lut_ww_partial[] = {
    0xA0, T1, T2, T3, T4, 1, // 10 10 00 00
    0x00,  1,  0,  0,  0, 1, // gnd phase   
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,
};

const unsigned char lut_bw_partial[] = {
    0xA0, T1, T2, T3, T4, 1, // 10 10 00 00
    0x00,  1,  0,  0,  0, 1, // gnd phase
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0, 
};

const unsigned char lut_bb_partial[] = {
    0x50, T1, T2, T3, T4, 1, // 01 01 00 00
    0x00,  1,  0,  0,  0, 1, // gnd phase
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0, 
};

const unsigned char lut_wb_partial[] = {
    0x50, T1, T2, T3, T4, 1, // 01 01 00 00
    0x00,  1,  0,  0,  0, 1, // gnd phase 
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
    0x00,  0,  0,  0,  0, 0,        
};

// Display resolution
#define EPD_WIDTH                       400
#define EPD_HEIGHT                      300

// EPD4IN2 commands
#define PANEL_SETTING                   0x00
#define POWER_SETTING                   0x01
#define POWER_OFF                       0x02
#define POWER_OFF_SEQUENCE_SETTING      0x03
#define POWER_ON                        0x04
#define POWER_ON_MEASURE                0x05
#define BOOSTER_SOFT_START              0x06
#define DEEP_SLEEP                      0x07
#define DATA_START_TRANSMISSION_1       0x10
#define DATA_STOP                       0x11
#define DISPLAY_REFRESH                 0x12
#define DATA_START_TRANSMISSION_2       0x13
#define LUT_FOR_VCOM                    0x20 
#define LUT_WHITE_TO_WHITE              0x21
#define LUT_BLACK_TO_WHITE              0x22
#define LUT_WHITE_TO_BLACK              0x23
#define LUT_BLACK_TO_BLACK              0x24
#define PLL_CONTROL                     0x30
#define TEMPERATURE_SENSOR_COMMAND      0x40
#define TEMPERATURE_SENSOR_SELECTION    0x41
#define TEMPERATURE_SENSOR_WRITE        0x42
#define TEMPERATURE_SENSOR_READ         0x43
#define VCOM_AND_DATA_INTERVAL_SETTING  0x50
#define LOW_POWER_DETECTION             0x51
#define TCON_SETTING                    0x60
#define RESOLUTION_SETTING              0x61
#define GSST_SETTING                    0x65
#define GET_STATUS                      0x71
#define AUTO_MEASUREMENT_VCOM           0x80
#define READ_VCOM_VALUE                 0x81
#define VCM_DC_SETTING                  0x82
#define PARTIAL_WINDOW                  0x90
#define PARTIAL_IN                      0x91
#define PARTIAL_OUT                     0x92
#define PROGRAM_MODE                    0xA0
#define ACTIVE_PROGRAMMING              0xA1
#define READ_OTP                        0xA2
#define POWER_SAVING                    0xE3

#ifdef TM4C123GHPM

// pin definitions for TM4C123GHPM
#define EPD_DIN (*((volatile uint32_t *)0x40004080)) // PA5
#define EPD_CLK (*((volatile uint32_t *)0x40004010)) // PA2
#define EPD_CS  (*((volatile uint32_t *)0x40004020)) // PA3
#define EPD_DC  (*((volatile uint32_t *)0x40004100)) // PA6
#define EPD_RST (*((volatile uint32_t *)0x40004200)) // PA7
#define EPD_BSY (*((volatile uint32_t *)0x40004040)) // PA4

#define EPD_CS_LO   0
#define EPD_CS_HI   0x08
#define EPD_DC_CMD  0
#define EPD_DC_DATA 0x40
#define EPD_RST_LO  0
#define EPD_RST_HI  0x80
#define EPD_BSY_LO  0
#define EPD_BSY_HI  0x10

#define SSI_CR0_SCR_M           0x0000FF00  // SSI Serial Clock Rate
#define SSI_CR0_SPH             0x00000080  // SSI Serial Clock Phase
#define SSI_CR0_SPO             0x00000040  // SSI Serial Clock Polarity
#define SSI_CR0_FRF_M           0x00000030  // SSI Frame Format Select
#define SSI_CR0_FRF_MOTO        0x00000000  // Freescale SPI Frame Format
#define SSI_CR0_DSS_M           0x0000000F  // SSI Data Size Select
#define SSI_CR0_DSS_8           0x00000007  // 8-bit data
#define SSI_CR1_MS              0x00000004  // SSI Master/Slave Select
#define SSI_CR1_SSE             0x00000002  // SSI Synchronous Serial Port
                                            // Enable
#define SSI_SR_BSY              0x00000010  // SSI Busy Bit
#define SSI_SR_TNF              0x00000002  // SSI Transmit FIFO Not Full
#define SSI_CPSR_CPSDVSR_M      0x000000FF  // SSI Clock Prescale Divisor
#define SSI_CC_CS_M             0x0000000F  // SSI Baud Clock Source
#define SSI_CC_CS_SYSPLL        0x00000000  // Either the system clock (if the
                                            // PLL bypass is in effect) or the
                                            // PLL output (default)
#define SYSCTL_RCGC1_SSI0       0x00000010  // SSI0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control

#define ONE_MS (72724*2/91) // 1msec, tuned at 80 MHz

/*
    Wait n milliseconds
*/
void delayms (uint32_t n)
{
    uint32_t volatile time;
    while(n--){
        time = ONE_MS;
        while(time--);
    }
}

/*
    Initialize Port A, SSI0 and relevant timers for safe EPD operation
*/
void tm4cInit (void)
{
    // clock init
    SYSCTL_RCGCSSI_R |= 0x01;               // activate SSI0
    SYSCTL_RCGCGPIO_R |= 0x01;              // activate port A
    while((SYSCTL_PRGPIO_R & 0x01) == 0);   // wait for clock to start

    // port A init
    GPIO_PORTA_DIR_R |= 0xC8;       // make PA3,6,7 out
    GPIO_PORTA_DIR_R &= ~0x10;      // make PA4 in
    GPIO_PORTA_AFSEL_R &= ~0xD8;    // disable alt funct on PA3,4,6,7
    GPIO_PORTA_DEN_R |= 0xD8;       // enable digital I/O on PA3,4,6,7
                                    // configure PA3,6,7 as GPIO
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0x00FF0FFF) + 0x00000000;
    GPIO_PORTA_AMSEL_R &= ~0xD8;    // disable analog func on PA3,4,6,7

    // initialize SSI0 in mode 0
    GPIO_PORTA_AFSEL_R |= 0x2C;     // enable alt funct on PA2,3,5
    GPIO_PORTA_DEN_R |= 0x2C;       // enable digital I/O on PA2,3,5
                                    // configure PA2,3,5 as SSI
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFF0F00FF) + 0x00202200;
    GPIO_PORTA_AMSEL_R &= ~0x2C;    // disable analog functionality on PA2,3,5
    SSI0_CR1_R &= ~SSI_CR1_SSE;     // disable SSI
    SSI0_CR1_R &= ~SSI_CR1_MS;      // master mode

    // configure for system clock/PLL baud clock source
    SSI0_CC_R = (SSI0_CC_R & ~SSI_CC_CS_M) + SSI_CC_CS_SYSPLL;
    // clock divider for 3.125 MHz SSIClk (50 MHz PIOSC/16)
    // SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+16;
    // clock divider for 8 MHz SSIClk (80 MHz PLL/24)
    // SysClk/(CPSDVSR*(1+SCR))
    // 80/(10*(1+0)) = 8 MHz (slower than 4 MHz)
    // 80/(4*(1+0)) = 20 MHz, for EPD IC
    SSI0_CPSR_R = (SSI0_CPSR_R & ~SSI_CPSR_CPSDVSR_M) + 4; // must be even
    SSI0_CR0_R &= ~(SSI_CR0_SCR_M |     // SCR = 0 (8 Mbps data rate)
                    SSI_CR0_SPH |       // SPH = 0
                    SSI_CR0_SPO);       // SPO = 0
    // FRF = Freescale format
    SSI0_CR0_R = (SSI0_CR0_R & ~SSI_CR0_FRF_M) + SSI_CR0_FRF_MOTO;
    // DSS = 8-bit data
    SSI0_CR0_R = (SSI0_CR0_R & ~SSI_CR0_DSS_M) + SSI_CR0_DSS_8;
    SSI0_CR1_R |= SSI_CR1_SSE;          // enable SSI
}

// The Data/Command pin must be valid when the eighth bit is
// sent.  The SSI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.
// NOTE: These functions will crash or stall indefinitely if
// the SSI0 module is not initialized and enabled.
void writecmd (uint8_t cmd)
{
    // wait until SSI0 not busy/transmit FIFO empty
    while((SSI0_SR_R & SSI_SR_BSY) == SSI_SR_BSY);
    EPD_DC = EPD_DC_CMD;
    SSI0_DR_R = cmd;  // data out
    while((SSI0_SR_R & SSI_SR_BSY) == SSI_SR_BSY);
}


void writedata (uint8_t data)
{
    // wait until transmit FIFO not full
    while((SSI0_SR_R & SSI_SR_TNF) == 0);
    EPD_DC = EPD_DC_DATA;
    SSI0_DR_R = data;   // data out
}

void select (void)
{
    // wait until SSI0 not busy/transmit FIFO empty
    while((SSI0_SR_R & SSI_SR_BSY) == SSI_SR_BSY);
    EPD_CS = EPD_CS_LO;
}

void deselect (void)
{
    // wait until SSI0 not busy/transmit FIFO empty
    while((SSI0_SR_R & SSI_SR_BSY) == SSI_SR_BSY);
    EPD_CS = EPD_CS_HI;
}

bool epd_is_idle(void)
{
    return (EPD_BSY == EPD_BSY_HI);
}

void wait_until_idle (void)
{
    while (EPD_BSY == EPD_BSY_LO) delayms(50);
}

void reset (void)
{
    EPD_RST = EPD_RST_LO;
    delayms(200);
    EPD_RST = EPD_RST_HI;
    delayms(200);
}

#endif // TM4C123GHPM

void epd_init(void)
{
    #ifdef TM4C123GHPM
    tm4cInit(); // initialize epd on port A with SSI0 in mode 0 at 8MHz
    #endif

    select();
    reset();

    // begin startup sequence
    writecmd(POWER_SETTING);
    writedata(0x03);                  // VDS_EN, VDG_EN
    writedata(0x00);                  // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    writedata(0x2b);                  // VDH
    writedata(0x2b);                  // VDL
    writedata(0xff);                  // VDHR

    writecmd(BOOSTER_SOFT_START);
    writedata(0x17);
    writedata(0x17);
    writedata(0x17);                  //07 0f 17 1f 27 2F 37 2f

    writecmd(PANEL_SETTING);
    writedata(0xbf);    // KW-BF   KWR-AF  BWROTP 0f
    writedata(0x0b);

    writecmd(PLL_CONTROL);
    writedata(0x3c);        // 3A 100HZ   29 150Hz 39 200HZ  31 171HZ

    writecmd(RESOLUTION_SETTING);
    writedata(EPD_WIDTH >> 8);        
    writedata(EPD_WIDTH & 0xff);
    writedata(EPD_HEIGHT >> 8);
    writedata(EPD_HEIGHT & 0xff);

    writecmd(VCM_DC_SETTING);
    writedata(0x12);                   

    writecmd(VCOM_AND_DATA_INTERVAL_SETTING);
    writecmd(0xd7);    // border floating to avoid flashing

    writecmd(POWER_ON);
    wait_until_idle();  // comment out if debugging with logic analyzer

    epd_set_lut_slow();
    epd_clear_frame();
}

void epd_reset(void)
{
    wait_until_idle();
    reset();
}

static void
epd_set_partial_area(int x0, int y0, int x1, int y1)
{
    if (x0 > x1 || y0 > y1) return;
    x1 |= 0x07;
    wait_until_idle();

    writecmd(PARTIAL_WINDOW);
    writedata(x0 >> 8);
    writedata(x0 & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    writedata(x1 >> 8);
    writedata(x1 | 0x07);
    writedata(y0 >> 8);        
    writedata(y0 & 0xff);
    writedata(y1 >> 8);        
    writedata(y1 & 0xff);
    writedata(0x01);         // Gates scan both inside and outside of the partial window. (default) 
    delayms(2);
}

void
epd_refresh_part(int x0, int y0, int x1, int y1)
{
    wait_until_idle();

    writecmd(PARTIAL_IN);

    epd_set_partial_area(x0,y0, x1, y1);

    if (ACTIVE_LUT != LUT_PART) {
        epd_set_lut_part();
        ACTIVE_LUT = LUT_PART;
    }
    writecmd(DISPLAY_REFRESH);

    wait_until_idle();
    writecmd(PARTIAL_OUT);
}

void
epd_update_part(const uint8_t* frame_buffer, int x, int y, int w, int l)
{
    wait_until_idle();
    writecmd(PARTIAL_IN);

    epd_set_partial_area(x, y, x + w - 1, y + l - 1);

    writecmd(DATA_START_TRANSMISSION_2);
    if (frame_buffer != 0) {
        for(int i = 0; i < w  / 8 * l; i++) {
            writedata(frame_buffer[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            writedata(0x00);  
        }  
    }
    delayms(2);
    writecmd(PARTIAL_OUT);
}

void epd_set_lut_slow()
{
    wait_until_idle();
    uint32_t count;

    writecmd(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        writedata(lut_vcom0_slow[count]);
    }
    
    writecmd(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        writedata(lut_ww_slow[count]);
    }   
    
    writecmd(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        writedata(lut_bw_slow[count]);
    } 

    writecmd(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        writedata(lut_wb_slow[count]);
    } 

    writecmd(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        writedata(lut_bb_slow[count]);
    } 
}

void epd_set_lut_part()
{
    wait_until_idle();
    uint32_t count;

    writecmd(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        writedata(lut_vcom0_partial[count]);
    }
    
    writecmd(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        writedata(lut_ww_partial[count]);
    }   
    
    writecmd(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        writedata(lut_bw_partial[count]);
    } 

    writecmd(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        writedata(lut_wb_partial[count]);
    } 

    writecmd(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        writedata(lut_bb_partial[count]);
    } 
}

void epd_set_lut_fast()
{
    wait_until_idle();
    uint32_t count;

    writecmd(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        writedata(lut_vcom0_fast[count]);
    }
    
    writecmd(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        writedata(lut_ww_fast[count]);
    }   
    
    writecmd(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        writedata(lut_bw_fast[count]);
    } 

    writecmd(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        writedata(lut_wb_fast[count]);
    } 

    writecmd(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        writedata(lut_bb_fast[count]);
    } 
}

void
epd_update_frame(const uint8_t * fr)
{
    wait_until_idle();

    writecmd(DATA_START_TRANSMISSION_1);
    for(int i = 0; i < (EPD_WIDTH / 8) * EPD_HEIGHT; i++) {
        writedata(0xFF);      // bit set: white, bit reset: black
    }
    delayms(2);

    writecmd(DATA_START_TRANSMISSION_2); 
    for(int i = 0; i < (EPD_WIDTH / 8) * EPD_HEIGHT; i++) {
        writedata(fr[i]);
    }
    delayms(2);
}

void
epd_refresh_fast()
{
    if (ACTIVE_LUT != LUT_FAST) {
        epd_set_lut_fast();
        ACTIVE_LUT = LUT_FAST;
    }

    writecmd(DISPLAY_REFRESH);
}

void
epd_refresh_slow()
{
    if (ACTIVE_LUT != LUT_SLOW) {
        epd_set_lut_slow();
        ACTIVE_LUT = LUT_SLOW;
    }

    writecmd(DISPLAY_REFRESH);
}

void
epd_display_frame_fast(const uint8_t * fr)
{
    wait_until_idle();

    epd_update_frame(fr);

    epd_refresh_fast();
}

void
epd_display_frame_slow(const uint8_t * fr)
{
    wait_until_idle();

    epd_update_frame(fr);

    epd_refresh_slow();
}

void epd_display_frame(void)
{
    wait_until_idle();
    writecmd(DISPLAY_REFRESH);
}

void epd_clear_frame(void)
{
    wait_until_idle();
    writecmd(RESOLUTION_SETTING);
    writedata(EPD_WIDTH >> 8);
    writedata(EPD_WIDTH & 0xff);
    writedata(EPD_HEIGHT >> 8);        
    writedata(EPD_HEIGHT & 0xff);

    writecmd(DATA_START_TRANSMISSION_1);           
    delayms(2);
    for(int i = 0; i < EPD_WIDTH / 8 * EPD_HEIGHT; i++) {
        writedata(0xFF);  
    }  
    delayms(2);
    writecmd(DATA_START_TRANSMISSION_2);           
    delayms(2);
    for(int i = 0; i < EPD_WIDTH / 8 * EPD_HEIGHT; i++) {
        writedata(0xFF);  
    }  
    delayms(2);
}

void epd_sleep(void)
{
    wait_until_idle();
    writecmd(VCOM_AND_DATA_INTERVAL_SETTING);
    writedata(0x17);                       //border floating    
    writecmd(VCM_DC_SETTING);          //VCOM to 0V
    writecmd(PANEL_SETTING);
    delayms(100);          

    writecmd(POWER_SETTING);           //VG&VS to 0V fast
    writedata(0x00);        
    writedata(0x00);        
    writedata(0x00);              
    writedata(0x00);        
    writedata(0x00);
    delayms(100);          
                
    writecmd(POWER_OFF);          //power off
    wait_until_idle();
    writecmd(DEEP_SLEEP);         //deep sleep
    writedata(0xA5);
}
