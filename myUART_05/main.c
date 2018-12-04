// ----------------------------------------------------------------------------
// main.c  (for myUart project) ('FR6989 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myUart.h"
#include "myLcd.h"
#include "stdio.h"


//***** Prototypes ************************************************************
void initGPIO(void);
void initTimer(void);
void initLora(void);
void getSensorData(void);
char getTrashcanStatus(void);
void getData(void);

//***** Defines ***************************************************************
#define GATEWAYMODE
//#define MOTEMODE
#define SlEEPMODE

//***** Global Variables ******************************************************
unsigned int trashBinID = 0x0001;

unsigned char* sysReset="sys reset\r\n";
unsigned char* macPause="mac pause\r\n";
unsigned char* macDevui="mac set deveui 0000000000000001\r\n";		//muss angepasst werden
unsigned char* macAppeui="mac set appeui 0000000000000000\r\n";
unsigned char* macAppkey="mac set appkey 00000000000000000000000000000001\r\n"; //muss angepasst werden
unsigned char* macDr="mac set dr 5\r\n";
unsigned char* macResume="mac resume\r\n";
unsigned char* macJoin="mac join otaa\r\n";
unsigned char* radioSetFreq="radio set freq 868100000\r\n";
unsigned char* radioSetSf="radio set sf sf7\r\n";
unsigned char* radioSetPwr="radio set pwr 15\r\n";
unsigned char* radioSetWdt="radio set wdt 60000\r\n";

unsigned long wert0;
unsigned long wert1;
unsigned long USekunden;
unsigned long Luftconst = 343;
unsigned long Umeter;
unsigned long AbstandMM;

Timer_A_initContinuousModeParam a0_contin =
{
     TIMER_B_CLOCKSOURCE_ACLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_DO_CLEAR,
     false
};

Timer_A_initCaptureModeParam a0_capture =
{
     TIMER_A_CAPTURECOMPARE_REGISTER_0,
     TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE,
     TIMER_A_CAPTURE_INPUTSELECT_CCIxA,
     TIMER_A_CAPTURE_ASYNCHRONOUS,
     TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
     TIMER_A_OUTPUTMODE_OUTBITVALUE
};

Timer_A_initUpModeParam a1_up =
{
     TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     0x6,                                       //6 = ca 15 min with b divider 64 d.h 12 = 30, 24 = 1h
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
     TIMER_A_DO_CLEAR,
     false
};

Timer_B_initContinuousModeParam b_contin =
{
     TIMER_B_CLOCKSOURCE_ACLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_4,
     TIMER_B_TBIE_INTERRUPT_DISABLE,
     TIMER_A_DO_CLEAR,
     false
};

Timer_B_initCompareModeParam b_compare =
{
     TIMER_B_CAPTURECOMPARE_REGISTER_2,
     TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE,
     TIMER_B_OUTPUTMODE_RESET_SET,
     0xFFFF
};

//*******************************************AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA**********************************
// Main
//*****************************************************************************
void main (void)
{
    //TODO FACTORY reset when busy gelesen wird
    WDT_A_hold(WDT_A_BASE);    // Stop watchdog timer

    initGPIO();
    initTimer();
    initClocks();
    myUart_init(CHANNEL_0, 57600, &myUart_Param_57600_8N1_SMCLK8MHz);
    __enable_interrupt();
    initLora();

    while(1)
    {
        getData();
    }
}

void initTimer()
{
    // init A0
    Timer_A_initContinuousMode(TIMER_A0_BASE, &a0_contin);
    Timer_A_initCaptureMode(TIMER_A0_BASE,&a0_capture);
    Timer_A_enableInterrupt(TIMER_A0_BASE);
    // init A2
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
    Timer_A_initUpMode(TIMER_A1_BASE, &a1_up);
    // init B0
    Timer_B_initContinuousMode(TIMER_B0_BASE, &b_contin);
    Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_2);
    Timer_B_initCompareMode(TIMER_B0_BASE, &b_compare);
    // start Timers
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Timer_B_setCompareValue(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0, 0x0000);
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);
}

void initLora()
{
    volatile unsigned long i;
    volatile unsigned int j;
    unsigned char commRx[20];
    j=0;
#ifdef GATEWAYMODE
    myUart_tx(sysReset);
    myUart_tx(macPause);
    myUart_tx(macDeveui);
    myUart_tx(macAppeui);
    myUart_tx(macAppkey);
    myUart_tx(macDr);
    myUart_tx(radioSetFreq);
    myUart_tx(radioSetSf);
    myUart_tx(radioSetPwr);
    myUart_tx(radioSetWdt);
    myUart_tx(macResume);
    myUart_tx(macJoin);
    /*myUart_rxtx(macJoin,commRx);

    while(commRx[0] != 'o' && j < 5)
    {
        for(i=1000000;i>0;i--);
        myUart_rxtx(macJoin,commRx);
        j++;
    }*/
#endif
#ifdef MOTEMODE
    myUart_tx("mac pause\r\n");
    myUart_tx("radio set mod lora\r\n");
    myUart_tx("radio set freq 868100000\r\n");
    myUart_tx("radio set pwr 14\r\n");
    myUart_tx("radio set sf sf7\r\n");
    myUart_tx("radio set afcbw 41.7\r\n");
    myUart_tx("radio set rxbw 125\r\n");
    myUart_tx("radio set crc on\r\n");
    myUart_tx("radio set iqi off\r\n");
    myUart_tx("radio set prlen 8\r\n");
    myUart_tx("radio set cr 4/5\r\n");
    myUart_tx("radio set wdt 60000\r\n");
    myUart_tx("radio set sync 12\r\n");
    myUart_tx("radio set bw 125\r\n");
#endif
}

void getSensorData()
{
    volatile unsigned long i;
    volatile unsigned int j;

    P1OUT |= 0x01;      //toggle Pin 1.1 to activate the sensor
    i = 100000;
    j = 20;
    for(j;j>0;j--);
    P1OUT &= ~0x01;

    do i--;
    while(i != 0);
}

char getTrashcanStatus()
{
    if(AbstandMM < 1200 && AbstandMM > 750)
        return '1';                                   //1/4 filled
    else if(AbstandMM <= 750 && AbstandMM > 500)
        return '2';                                   //1/2 filled
    else if(AbstandMM <= 500 && AbstandMM > 250)
        return '3';                                   //3/4 filled
    else if(AbstandMM <= 250 && AbstandMM > 0)
        return '4';                                   //full
    else
        return 'F';                                   //Error
}

void getData()
{
    unsigned char fillLevel;
    volatile unsigned int i;
    volatile unsigned long j;
    char tx[32];

#ifdef GATEWAYMODE
    for(i=6; i>0;i--)
    {
        getSensorData();
        fillLevel = getTrashcanStatus();
        snprintf(tx,32,"mac tx uncnf 2 0%c0%d\r\n",fillLevel,trashBinID);
        myUart_tx(tx);
        j = 1000000;
        do j--;
        while(j != 0);
    }
#endif
#ifdef MOTEMODE
    getSensorData();
    fillLevel = getTrashcanStatus();
    snprintf(tx,32,"radio tx 0%c0%d\r\n",fillLevel,trashBinID);
    myUart_tx(tx);
    for(i=6; i>0;i--){
        j = 1000000;
        do j--;
        while(j != 0);
    }
#endif
#ifdef SlEEPMODE
    P4OUT |= (0x01<<2);     //switch of lora mote and sensor
    P2DIR = (0x00<<0);      //switch of high pull from uartpins to lower energy consumption
    P2SEL0 = (0x00);
    P2IN = (0x00);
    __low_power_mode_3();
#endif
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A (void)
{
    if(P1IN&(0x01<<5))
    {
        wert0 = TA0R*30;
    }
    else
    {
        wert1 = TA0R*30;
        if(wert0>wert1)
        {
            wert1= wert1 + 65536;
        }
        USekunden = wert1-wert0;
        Umeter = USekunden * Luftconst;
        AbstandMM = ((Umeter)/2)/1000;
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)
{
     switch(TA0IV)
     {
       case  4:
           P1OUT ^= 0x01;
           break;
       default: break;
     }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
    switch(TA1IV)
    {
    case 2:
        break;
    default:
        break;
    }
    __low_power_mode_off_on_exit();
    P4OUT = (0x00<<2);
    initGPIO();
    initLora();
}
