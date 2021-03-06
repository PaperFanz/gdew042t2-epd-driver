// Voltmeter.c
// Runs on TM4C123
// The Squad
// October 23, 2020

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "../inc/Timer2A.h"
#include "gui.h"
#include "epdgl.h"
#include "voltmeter.h"

#define PT_LOC 6

static int32_t vExt0;
static int32_t vExt1;
static uint8_t vmReady;
uint32_t GLOB_BATTERY;
static int32_t VOLT_RUNNING_AVG = 2048;
static int32_t BATT_RUNNING_AVG = 2048;

text_config_t vm_fnt = {&Consolas20, EPD_BLACK};

static int32_t
adc_to_volts(int32_t v)
{
    /* THIS FUNCTION ONLY WORKS WITH THE EXTERNAL DATA AQUISITION PORT */
    return (33 * (v << (PT_LOC))) / 40960;
}

static void
draw_ext_volts()
{
    /* Note: this function does not clear anything or set cursor */
    int32_t volts = (33 * (vExt0 << (PT_LOC))) / 40960;
    epdgl_draw_int(volts >> PT_LOC, &vm_fnt);
    epdgl_draw_char('.', &vm_fnt);
    epdgl_draw_int(volts & ~(0xffffffff << PT_LOC), &vm_fnt);
    epdgl_draw_char('V', &vm_fnt);
}

double
volt_get_val()
{
    double volts = (double) vExt0;
    volts = 33 * volts / 40960;
    return volts;
}

void
draw_voltmeter()
{
        epdgl_fill_rect(0, 20, 300, 360, EPD_WHITE);
        epdgl_set_cursor(10, 40);
        draw_ext_volts();
        epdgl_set_cursor(10, 60);
        epdgl_draw_int(vExt0, &vm_fnt);
}

void
volt_handle_input(key_t k)
{
    switch (k) {
    case ENTER:
        draw_voltmeter();
        break;
    default:
        break;
    }
}

/**
 * Initializes the voltmeter (set ADC configuration, etc).
 * @return Nothing
 */
void voltmeter_init(){
  // Enable clocks and wait for stabilization
  SYSCTL_RCGCADC_R |= 0x01; 
  SYSCTL_RCGCGPIO_R |= VM_PORT_NUM; 
  SYSCTL_RCGCTIMER_R |= 0x01; 
  __asm__(
        "NOP\n"
        "NOP"
  );

  // Set voltmeter pins to analog inputs
  VM_DIR_R &= ~VM_GPIO_MASK; 
  VM_AFSEL_R |= VM_GPIO_MASK; 
  VM_DEN_R &= ~VM_GPIO_MASK; 
  VM_AMSEL_R |= VM_GPIO_MASK; 

  // Set ADC sampling rate (125k), priority, 64 sample average
  ADC0_SAC_R = 6;
  ADC0_PC_R = 0x01;
  ADC0_SSPRI_R = 0x3210; 

  // Setup timer for 100 Hz but don't enable
  TIMER0_CTL_R = 0x00000000; 
  TIMER0_CTL_R |= 0x00000020; 
  TIMER0_CFG_R = 0; 
  TIMER0_TAMR_R = 0x00000002; 
  TIMER0_TAPR_R = 0; 
  TIMER0_TAILR_R = 799999; 
  TIMER0_IMR_R = 0x00000000;

  // Configure sample sequencer 2 with interrupts
  ADC0_ACTSS_R &= ~0x04; 
  ADC0_EMUX_R = (ADC0_EMUX_R & 0xFFFFF0FF)+0x0500; 
  ADC0_SSMUX2_R = (VEXT0_CHAN << 4) | VEXT1_CHAN;
  ADC0_SSCTL2_R = 0x0060; 
  ADC0_IM_R |= 0x04; 
  ADC0_ACTSS_R |= 0x04; 
  NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000040; 
  NVIC_EN0_R = 1<<16;
  EnableInterrupts();

  vmReady = 0;
  GLOB_BATTERY = battery_read();
  Timer2A_Init((void (*)(void)) &battery_read, 80000000, 7);
} 

/**
 * Returns the difference in voltage between the two VEXT pins
 * @return volts - digital voltage difference between input pins.  
 * (-4096 to 4096 = -6.6V to 6.6V). 
 */
int32_t voltmeter_read(void){
  // Enable timer
   TIMER0_CTL_R |= 0x00000021; 
   Clock_Delay1ms(10);

  // Read value
  while(!vmReady){
    Clock_Delay1ms(5);
  }
  int32_t volts = vExt0;

  // Disable timer
  TIMER0_CTL_R = 0x00000000; 
  vmReady = 0;

  // Convert to decimal fixed point
  return (6600 * volts)/4096;
}

int32_t battery_read(void){
    static uint32_t count = 0;
    // Enable timer
    TIMER0_CTL_R |= 0x00000021; 
    Clock_Delay1ms(10);

    // Read value
    while(!vmReady){
        Clock_Delay1ms(5);
    }

    // Disable timer
    TIMER0_CTL_R = 0x00000000; 
    vmReady = 0;

    if (count++ == 127) {
        count = 0;

        // Convert to percentage: 3.0 V = 0% = 1861/4096, 4.16 V = 100% = 2594/4096
        GLOB_BATTERY = ((BATT_RUNNING_AVG - 1861) * 100)/733;
        update_status_bar();
    }

    return 0;
}

/**
 * Updates voltmeter reading globals and sets voltmeter data ready flag.
 * @return Nothing 
 */
void ADC0Seq2_Handler(void){
  // Acknowledge sequencer 2 completion
  ADC0_ISC_R = 0x04;

  // Update globals with new ADC reading
  vmReady = 1;
  vExt1 = ADC0_SSFIFO2_R; //PD1
  vExt0 = ADC0_SSFIFO2_R; //PD0
  BATT_RUNNING_AVG = (vExt1 + BATT_RUNNING_AVG * 15) >> 4;
  VOLT_RUNNING_AVG = (vExt0 + VOLT_RUNNING_AVG * 15) >> 4;
}

