/* SDCardDemo for PIC 32MX795F512L 
 * NO USB Version
 * 
 * Compiled for PIC32MX795F512L using XC32 V1.30 compiler
 * MPLABX Version 4.10
 *  
 * 08-05-18 JBS: Adapted from SD Pots project. Updated Defs.h to work for SNAD PIC or UBW32 boards.
 * 08-06-21 JBS: Works on Rev 2 Brain Board
 * 08-07-21 JBS: Added USE_BRAINBOARD
 * 12-17-22 JBS: Demo now works on REV 3 CYTRON CONTROLLER as well as Olimex PIC32_PINMGUINO_MICRO
 *                  and BRAIN BOARD REV 1. Select board in Defs.h
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "FSIO.h"
#include "Delay.h"
#include "Defs.h"

#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"

/** CONFIGURATION **************************************************/

#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select


unsigned char HOSTRxBuffer[MAXBUFFER+1];
unsigned char HOSTTxBuffer[MAXBUFFER+1];
unsigned char HOSTRxBufferFull = FALSE;

void InitializeSystem(void);

BYTE intFlag = false;
void main(void) 
{
unsigned short i=0, numBytes;
FSFILE *filePtr;
char filename[] = "TestFile.txt";
int length;
char ch;

    InitializeSystem();
    printf("\rPIC 32 SD Card Test #111 no diagnostics");
    
    DelayMs(100);
    
#ifdef USE_PIC32_PINGUINO_MICRO
    printf("\r>>Starting SD card demo for Olimex PIC32 PINGUINO MICRO\r");
#elif defined USE_BRAINBOARD
    printf("\r>>Starting SD card demo for '795 BRAIN BOARD  #1");
#elif defined USE_CYTRON_CTRL    
    printf("\r>>Starting SD card demo for UBW32 BOARD");
#else     
    printf("\r>>Starting SD card demo for SNAD PIC BOARD  #1");
#endif
    
    printf("\rWait for Media Detect...");
    while (!MDD_MediaDetect());     // Wait for SD detect to go low    
    printf("\rInitializing SD card...");
    while (!FSInit());
    printf("\rOpening test file...");
    filePtr = FSfopen(filename, FS_READ);    

    if (filePtr==NULL) printf("\rError: could not open %s", filename);
    else
    {
        printf("\rSuccess! Opened %s. Reading data: \r\r", filename);    
        do {           
            numBytes = FSfread(&ch, 1, 1, filePtr);
            if (ch != '\n') putchar(ch);
        } while (numBytes);    
        printf("\rClosing file");
        FSfclose(filePtr); 
        printf("\rDONE");        
    }
    DelayMs(10);        // Initialize SD card    
    
    while(1)
    {
        DelayMs(1);    
        if (HOSTRxBufferFull)
        {
            HOSTRxBufferFull = false;        
            printf("\rOpening %s to append...", filename);
            filePtr = FSfopen(filename, FS_APPEND);
            length = strlen(HOSTRxBuffer);
            printf("\rWriting %d bytes: %s", length, HOSTRxBuffer);        
            numBytes = FSfwrite(HOSTRxBuffer, 1, length, filePtr);
            printf("\r%d bytes written", numBytes);
            FSfclose(filePtr); 
            printf("\rDONE");            
        }                
    }
}


void InitializeSystem(void) 
{	   
    char ch;
    SYSTEMConfigPerformance(60000000);
    
    // UserInit();
    // Turn off JTAG so we get the pins back
    mJTAGPortEnable(false);
    
    SD_CS = 1;
    LED = 0;
    TEST_OUT = 0;
   
#ifdef USE_PIC32_PINGUINO_MICRO
    PORTSetPinsDigitalOut(IOPORT_B, BIT_13);  // SD_CS
    PORTSetPinsDigitalOut(IOPORT_C, BIT_14);  // TEST_OUT  
    PORTSetPinsDigitalOut(IOPORT_D, BIT_1);   // LED
#elif defined USE_BRAINBOARD
    PORTSetPinsDigitalOut(IOPORT_E, BIT_3 | BIT_4 | BIT_6);  // LED, SD_CS, TEST_OUT
    PORTSetPinsDigitalIn(IOPORT_G, BIT_9);  // SD_DETECT
    PORTSetPinsDigitalIn(IOPORT_A, BIT_0);  // SD_WP
#elif defined USE_CYTRON_CTRL
    PORTSetPinsDigitalOut(IOPORT_E,BIT_4);  // SD_CS
    PORTSetPinsDigitalIn(IOPORT_G, BIT_9);  // SD_DETECT
    PORTSetPinsDigitalOut(IOPORT_A, BIT_0); // LED    
    PORTSetPinsDigitalOut(IOPORT_C, BIT_1); // TEST_OUT 
#else
    #error "InitializeSystem() NOT DEFINED"
#endif    
    
    // Set up HOST UART    
    UARTConfigure(HOSTuart, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(HOSTuart, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(HOSTuart, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
#define SYS_FREQ 60000000
    UARTSetDataRate(HOSTuart, SYS_FREQ, 115200);
    UARTEnable(HOSTuart, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure HOST UART Interrupts
    INTEnable(INT_SOURCE_UART_TX(HOSTuart), INT_DISABLED);
    INTEnable(INT_SOURCE_UART_RX(HOSTuart), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(HOSTuart), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(HOSTuart), INT_SUB_PRIORITY_LEVEL_0);
    
    if (UARTReceivedDataIsAvailable(HOSTuart)) 
        ch = UARTGetDataByte(HOSTuart);
    
        // Set up Timer 2 for PWM time base    
    T2CON = 0x00;
    T2CONbits.T32 = 0; // 16 bit timer
    T2CONbits.TCKPS2 = 0; // 1:1 Prescaler
    T2CONbits.TCKPS1 = 0;
    T2CONbits.TCKPS0 = 0;    
    PR2 = 3000; // Use 50 microsecond rollover for 20 khz
    T2CONbits.TON = 1; // Let her rip   
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2); 
    
    // Turn on the interrupts
    INTEnableSystemMultiVectoredInt();

}//end UserInit


void __ISR(HOST_VECTOR, ipl2) IntHostUartHandler(void) {
static unsigned short HOSTRxIndex = 0;
unsigned char ch;    
    
    if (HOSTbits.OERR || HOSTbits.FERR){
        if (UARTReceivedDataIsAvailable(HOSTuart)) 
            ch = UARTGetDataByte(HOSTuart);   
        HOSTbits.OERR=0;
        HOSTRxIndex=0;
    }
    else if (INTGetFlag(INT_SOURCE_UART_RX(HOSTuart))) {
        INTClearFlag(INT_SOURCE_UART_RX(HOSTuart));
        if (UARTReceivedDataIsAvailable(HOSTuart)) {
            ch = UARTGetDataByte(HOSTuart);
            if(HOSTRxIndex < MAXBUFFER)
                HOSTRxBuffer[HOSTRxIndex++] = ch;                
            if (ch=='\r')
            {
                HOSTRxBuffer[HOSTRxIndex] = '\0';  // $$$$
                HOSTRxBufferFull = true;
                HOSTRxIndex=0;
            }                   
        }
    }
    if (INTGetFlag(INT_SOURCE_UART_TX(HOSTuart))) 
        INTClearFlag(INT_SOURCE_UART_TX(HOSTuart));    

}


// Timer 2 generates an interrupt every 50 microseconds approximately
void __ISR(_TIMER_2_VECTOR, IPL2 ) Timer2Handler(void) 
{            
    static int Timer2Counter = 0;
    static int MillisecondCounter = 0;
    
    Timer2Counter++;
    if (Timer2Counter >= 20)
    {        
        Timer2Counter = 0;
        intFlag = true;
        TEST_OUT = ~TEST_OUT;
        MillisecondCounter++;
        if (MillisecondCounter >= 200)
        {
            MillisecondCounter = 0;
            LED = ~LED;
        }
    }
    mT2ClearIntFlag(); // Clear interrupt flag            
}
