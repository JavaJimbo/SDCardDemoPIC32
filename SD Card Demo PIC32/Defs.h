/* 
 * File:   Defs.h
 * Author: Jim
 *
 * Created on December 15, 2014, 12:50 PM
 * 8-5-18 JBS: Updated to work for SNAD PIC or UBW32 boards.
 * 
 */
#include <xc.h>
#ifndef DEFS_H
#define	DEFS_H

// #define SD_DIAGNOSTICS
// #define USE_BRAINBOARD 
// #define USE_PIC32_PINGUINO_MICRO
#define USE_CYTRON_CTRL


#define	STX '>'
#define	DLE '/'
#define	ETX '\r'

#define MAXBITMAP 10000
#define TIMER1_ROLLOVER 1200 // 1200 // 2400

// #define BALANCE 40000
// #define BALANCE 36840
// #define BALANCE 24840
#define BALANCE 48840

#define MAXPIXELS (64*48)
#define MAXCOL 64
#define MAXROW 48
#define MAXLINE 8
#define RESOLUTION 6

#define MINUTES 0
#define HOURS 1

#define FREEZETIME 0

#define NUMPOTS 4


#define HOSTuart UART2
#define HOSTbits U2STAbits
#define XBEEuart UART4
#define XBEEbits U4STAbits
#define XBEE_VECTOR _UART_4_VECTOR
#define HOST_VECTOR _UART_2_VECTOR


#define RS485uart UART5

#define XBEE_SLEEP PORTBbits.RB15

#ifdef USE_PIC32_PINGUINO_MICRO
    #define TEST_OUT LATCbits.LATC14
    #define LED LATDbits.LATD1
#elif defined USE_BRAINBOARD
    #define TEST_OUT LATEbits.LATE3
    #define LED LATEbits.LATE6
#elif defined USE_CYTRON_CTRL
    #define TEST_OUT LATCbits.LATC1
    #define LED LATAbits.LATA0
#endif

#define TRIG_OUT PORTBbits.RB1

#define TRIG_HI PORTSetBits(IOPORT_B, BIT_1)
#define TRIG_LOW PORTClearBits(IOPORT_B, BIT_1)

#define XBEE_SLEEP_ON PORTSetBits(IOPORT_B, BIT_0)
#define XBEE_SLEEP_OFF PORTClearBits(IOPORT_B, BIT_0)

#define TEST_HI PORTSetBits(IOPORT_B, BIT_0)
#define TEST_LOW PORTClearBits(IOPORT_B, BIT_0)


#define START 1

#define MAXBUFFER 256 // 512
#define MAXHOSTBUFFER 128

#define XBEE_SLEEP PORTBbits.RB15

#define NUMWRITES (MAXCOL * MAXROW / 16)  // The number of writes to Port D = Total columns * total rows / number of rows per panel
#define ADVANCE_PTR (NUMWRITES*4)

//#define MAXCOLx4 (MAXCOL * 4)
//#define MAXCOLx32 (MAXCOL * 32)
//#define MAXCOLx60 (MAXCOL * 15 * 4)
#define COLMASK (MAXCOL-1)


/*
#define RED_LSB 0x000008  // For resolution = 5
#define GREEN_LSB 0x000800
#define BLUE_LSB 0x080000
*/

#define RED_LSB (0x01<<(8-RESOLUTION))
#define GREEN_LSB (RED_LSB*256)
#define BLUE_LSB  (RED_LSB*65536)


#define CLKbit 0x0010
#define OEbit 0x0020
#define LATbit 0x0040
#define R1bit 0x0080
#define B1bit  0x0100
#define G1bit 0x0200
#define R2bit 0x0400
#define B2bit 0x0800
#define G2bit 0x1000

#define CLKOUT  0x0010  // PORTDbits.RD4
#define OE_ENB  0xFFDF  // PORTDbits.RD5
#define LATCH   0x0040  // PORTDbits.RD6

#define DARKEN //& 0x7F7F7F
#define MAGENTA 0x8000A0 DARKEN
#define PURPLE 0xA00040 DARKEN
#define CYAN 0x405000 DARKEN
#define LIME 0x00A060 DARKEN
#define YELLOW 0x007090 DARKEN
#define ORANGE 0x0020FF DARKEN
#define RED 0x0000FF DARKEN
#define GREEN 0x00FF00 DARKEN
#define BLUE 0xFF0000 DARKEN
#define PINK 0x1020F0 DARKEN
#define LAVENDER 0x602040 DARKEN
#define BLACK 0x000000 DARKEN
#define TURQUOISE 0x30A000 DARKEN
#define WHITE 0x707070 DARKEN
#define GRAY 0x303030 DARKEN
#define DARKGRAY 0x101010 DARKEN

#define MAXCOLOR 15


#define MAXRANDOM (RAND_MAX+1)
#define true	TRUE
#define false 	FALSE

#define RS485_CTRL PORTGbits.RG0

// Pin defs for UBW32:
#define CLKOUT  0x0010  // PORTDbits.RD4
#define OE_ENB  0xFFDF  // PORTDbits.RD5
#define LATCH   0x0040  // PORTDbits.RD6
#define OEpin   PORTDbits.RD5


#define RIGHT 1
#define LEFT  2
#define UP    3
#define DOWN  4
#define UPRIGHT 5
#define UPLEFT 6
#define DOWNRIGHT 7
#define DOWNLEFT 8

#endif	/* DEFS_H */

