// SDCFile.c
// Runs on TM4C123
// This program is a simple demonstration of the SD card,
// file system, and ST7735 LCD.  It will read from a file,
// print some of the contents to the LCD, and write to a
// file.
// Daniel Valvano
// Feb 22, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2016
   Program 4.6, Section 4.3
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2016
   Program 2.10, Figure 2.37

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
// 1  ground
// 2  Vcc +3.3V
// 3  PA7 TFT reset
// 4  PA6 TFT data/command
// 5  PD7/PB0 SDC_CS, active low to enable SDC
// 6  PA3 TFT_CS, active low to enable TFT
// 7  PA5 MOSI SPI data from microcontroller to TFT or SDC
// 8  PA2 Sclk SPI clock from microcontroller to TFT or SDC
// 9  PA4 MISO SPI data from SDC to microcontroller
// 10 Light, backlight connected to +3.3 V

// **********wide.hk ST7735R*******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - PD7/PB0 SDC_CS, active low to enable SDC
// MISO - PA4 MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

#include <stdint.h>
#include "sdc.h"
#include "ff.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"

void EnableInterrupts(void);

static FATFS g_sFatFs;
FIL Handle,Handle2;
FRESULT MountFresult;
FRESULT Fresult;
unsigned char buffer[512];
#define MAXBLOCKS 100
// Describe the error with text on the LCD and then stall.  If
// the error was caused by a mistake in configuring SSI0, then
// the LCD will probably not work.
void diskError(char *errtype, int32_t code, int32_t block){
//  ST7735_DrawString(0, 0, "Error:", ST7735_Color565(255, 0, 0));
//  ST7735_DrawString(7, 0, errtype, ST7735_Color565(255, 0, 0));
//  ST7735_DrawString(0, 1, "Code:", ST7735_Color565(255, 0, 0));
//  ST7735_SetCursor(6, 1);
//  ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
//  ST7735_OutUDec(code);
//  ST7735_DrawString(0, 2, "Block:", ST7735_Color565(255, 0, 0));
//  ST7735_SetCursor(7, 2);
//  ST7735_OutUDec(block);
  return;
}
// The simple unformatted test will destroy the formatting and
// erase everything on the SD card.
void SimpleUnformattedTest(void){ DSTATUS result; uint16_t block; int i; uint32_t n; uint32_t errors = 0;
  // simple test of eDisk
  result = disk_initialize(0);  // initialize disk
  if(result) diskError("disk_initialize", result, 0);
  n = 1;    // seed
  for(block = 0; block < MAXBLOCKS; block++){
    for(i=0; i<512; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      buffer[i] = 0xFF&n;
    }
    result = disk_write (0,buffer, block, 1);
    if(result) diskError("disk_write", result, block); // save to disk
  }
  n = 1;  // reseed, start over to get the same sequence
  for(block = 0; block < MAXBLOCKS; block++){
    result = disk_read (0,buffer, block,1);
    if(result) diskError("disk_read ", result, block); // read from disk
    for(i=0; i<512; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      if(buffer[i] != (0xFF&n)){
        errors = errors + 1;
      }
    }
  }
}
#define FILETESTSIZE 10000
int FileSystemTest(void){
  UINT successfulreads, successfulwrites;
  char c, d;
  int16_t x, y; int i; uint32_t n;
  c = 0;
  x = 0;
  y = 10;
  n = 1;    // seed
  Fresult = f_open(&Handle2, "testFile.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult){
    // error
    return -1;
  } else{
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      c = ((n>>24)%10)+'0'; // random digit 0 to 9
      Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
      if((successfulwrites != 1) || (Fresult != FR_OK)){
        // f_write error
        return -1;
      }
    }
    Fresult = f_close(&Handle2);
    if(Fresult){
      // f_close error
      return -1;
    }
  }
  n = 1;  // reseed, start over to get the same sequence
  Fresult = f_open(&Handle, "testFile.txt", FA_READ);
  if(Fresult == FR_OK){
		// opened
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      d = ((n>>24)%10)+'0'; // expected random digit 0 to 9
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
      if((successfulreads == 1) && (Fresult == FR_OK) && (c == d)){
        //ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
        x = x + 6;
        if(x > 122){
          x = 0;                          // start over on the next line
          y = y + 10;
        }
        if(y > 150){
          y = 10;                         // the screen is full
        }
      } else{
        // f_read error
        return -1;
      }

    }
  } else{
    /*ST7735_DrawString(0, 0, "Error testFile.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(20, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
		*/
    return -1;
  }
  // test passed
  Fresult = f_close(&Handle);
}

const char inFilename[] = "test.txt";   // 8 characters or fewer
const char outFilename[] = "out.txt";   // 8 characters or fewer

int SDCFileTest(void){	
  UINT successfulreads, successfulwrites;
  uint8_t c, x, y;
//  SimpleUnformattedTest();              // comment this out if continuing to the advanced file system tests
  MountFresult = f_mount(&g_sFatFs, "", 0);
  if(MountFresult){
    // f_mount error
    return -1;
  }
  FileSystemTest();                     // comment this out if file system works
	
	// f_read test
  Fresult = f_open(&Handle, inFilename, FA_READ);
  if(Fresult == FR_OK){
    // opened
    //ST7735_DrawString(7, 0, (char *)inFilename, ST7735_Color565(0, 255, 0));
    // get a character in 'c' and the number of successful reads in 'successfulreads'
    Fresult = f_read(&Handle, &c, 1, &successfulreads);
    x = 0;                              // start in the first column
    y = 10;                             // start in the second row
    while((Fresult == FR_OK) && (successfulreads == 1) && (y <= 130)){
      if(c == '\n'){
        x = 0;                          // go to the first column (this seems implied)
        y = y + 10;                     // go to the next row
      } else if(c == '\r'){
        x = 0;                          // go to the first column
      } else{                           // the character is printable, so print it
        //ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 255), 0, 1);
        x = x + 6;                      // go to the next column
        if(x > 122){                    // reached the right edge of the screen
          x = 0;                        // go to the first column
          y = y + 10;                   // go to the next row
        }
      }
      // get the next character in 'c'
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
    }
    // close the file
    Fresult = f_close(&Handle);
  } else{
    // 'failure opening file'
		return -1;
  }

  // open the file to be written
  // Options:
  // FA_CREATE_NEW    - Creates a new file, only if it does not already exist.  If file already exists, the function fails.
  // FA_CREATE_ALWAYS - Creates a new file, always.  If file already exists, it is over-written.
  // FA_OPEN_ALWAYS   - Opens a file, always.  If file does not exist, the function creates a file.
  // FA_OPEN_EXISTING - Opens a file, only if it exists.  If the file does not exist, the function fails.
  Fresult = f_open(&Handle, outFilename, FA_WRITE|FA_OPEN_ALWAYS);
  if(Fresult == FR_OK){
    // 'opened'
    // epd - ST7735_DrawString(7, 14, (char *)outFilename, ST7735_Color565(0, 255, 0));
    // jump to the end of the file
    Fresult = f_lseek(&Handle, Handle.fsize);
    // write a message and get the number of successful writes in 'successfulwrites'
    Fresult = f_write(&Handle, "Another successful write.\r\n", 27, &successfulwrites);
    if(Fresult == FR_OK){
      // print the number of successful writes
      // expect: third parameter of f_write()
      
      // print the byte offset from the start of the file where the writes started
      // expect: (third parameter of f_write())*(number of times this program has been run before)
			
    } else{
      // 'unsuccessful write'
			return -1;
    }
    // close the file
    Fresult = f_close(&Handle);
  } else{
    // 'failed to open'
		return -1;
  }
  return 0;
}
