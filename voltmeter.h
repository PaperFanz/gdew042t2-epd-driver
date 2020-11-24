#ifndef _VOLTMETER_H_
#define _VOLTMETER_H_

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// Voltmeter hardware configuration
#define VM_PORT_NUM   0x8
#define VM_DIR_R      GPIO_PORTD_DIR_R
#define VM_AFSEL_R    GPIO_PORTD_AFSEL_R
#define VM_DEN_R      GPIO_PORTD_DEN_R
#define VM_AMSEL_R    GPIO_PORTD_AMSEL_R 
#define VM_GPIO_MASK  0x3   //PD1-0
#define VEXT0_CHAN    0x7   //PD0 - channel 7
#define VEXT1_CHAN    0x6   //PD1 - channel 6

extern uint32_t GLOB_BATTERY;

/**
 * Initializes the voltmeter (set ADC configuration, etc).
 * @return Nothing
 */
void voltmeter_init(void); 

/**
 * Returns the voltage from the voltmeter pin ()
 * @return volts - digital voltage difference between input pins.  
 * (-4096 to 4096 = -6.6V to 6.6V). 
 */
int32_t voltmeter_read(void);


/**
 * Returns the percentage of battery charge
 * @return percent - percent of battery charged
 * (3.0 V = 0%, 4.16 V = 100%)
 */
int32_t battery_read(void);

/**
 * Updates voltmeter reading globals and sets voltmeter data ready flag. 
 * @return Nothing
 */
void ADC0Seq2_Handler(void);
	
#endif
