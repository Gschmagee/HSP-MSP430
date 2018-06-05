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
int getTrashcanStatus(void);

//***** Defines ***************************************************************

//***** Global Variables ******************************************************
unsigned char* facReset="sys factoryRESET\r\n";
unsigned char* sysReset="sys reset\r\n";
unsigned char* macPause="mac pause\r\n";
unsigned char* macAppeui = "mac set appeui 70B3D57ED000F246\r\n";
unsigned char* macAppkey= "mac set appkey F15F2E102F0405DB3E5108E65721257C\r\n";
unsigned char* macDr="mac set dr 5\r\n";
unsigned char* macResume="mac resume\r\n";
unsigned char* macJoin="mac join otaa\r\n";
unsigned char* macTx="mac tx uncnf 2 1232\r\n";
unsigned char* radioSetFreq="radio set freq 869100000\r\n";
unsigned char* radioSetSf="radio set sf sf7\r\n";
unsigned char* radioSetPwr="radio set pwr 15\r\n";
unsigned char* radioSetWdt="radio set wdt 60000\r\n";


char val[6];
char commRx2[80] = {0};
unsigned int i = 0;

unsigned long wert0;
unsigned long wert1;
unsigned long USekunden;
unsigned long Luftconst = 333;
unsigned long  Umeter;
unsigned long  AbstandMM;

Timer_A_initContinuousModeParam a0_contin =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
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
     0x4,
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
     TIMER_A_DO_CLEAR,
     false
};

Timer_B_initContinuousModeParam b_contin =
{
     TIMER_B_CLOCKSOURCE_ACLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     TIMER_B_TBIE_INTERRUPT_ENABLE,
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

//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{

    //myLCD_init();

    WDT_A_hold( WDT_A_BASE );    // Stop watchdog timer

    initGPIO();

    P3SEL1|=    (0x01<<6);                  //1
    P3SEL0|=    (0x00<<6);                  //1

    P1DIR |= 0x01;
    P1DIR &= ~(0x01<<5);                    //Pin 1.5 is Input
    P1SEL1|=    (0x01<<5);                  //1
    P1SEL0|=    (0x01<<5);                  //1

    initTimer();
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);
    //CCI2A 3.6 - || input A2 - 1.1
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Timer_B_setCompareValue(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0, 0x0000);
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);

    initClocks();


    if ( myUart_init( BACKCHANNEL_UART, 115200, &myUart_Param_115200_8N1_SMCLK8MHz ) >> 0 )    // Initialize UART (for backchannel communications)
        return;
    else
        myUart_writeBuf( BACKCHANNEL_UART, "Backchannel opened", NULL, CRLF );


    if ( myUart_init( CHANNEL_0, 57600, &myUart_Param_57600_8N1_SMCLK8MHz ) >> 0 )    // Initialize UART for serial comm with another LaunchPad
        return;
    else
        myUart_writeBuf( BACKCHANNEL_UART, "Serial Comm opened", NULL, CRLF );

    initLora();

    __enable_interrupt();

    volatile unsigned int x;
    volatile unsigned int i;
    char tx[32];
    while(1)
    {
        for(i=4; i>0;i--)
        {
            getSensorData();
            x = AbstandMM;

            snprintf(tx,32,"mac tx uncnf 2 %d\r\n",x);
            //myUart_rxtx(tx);
        }

    }
    //__low_power_mode_3();
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
}

void initLora()
{
    myUart_rxtx(sysReset);
    myUart_rxtx(macPause);
    myUart_rxtx(macAppeui);
    myUart_rxtx(macAppkey);
    myUart_rxtx(macDr);
    myUart_rxtx(radioSetFreq);
    myUart_rxtx(radioSetSf);
    myUart_rxtx(radioSetPwr);
    myUart_rxtx(radioSetWdt);
    myUart_rxtx(macResume);
    myUart_rxtx(macJoin);
}

void getSensorData()
{
    volatile unsigned long i;            // volatile to prevent optimization
    volatile unsigned int j;            // volatile to prevent optimization

    P1OUT |= 0x01;                      // Toggle P1.0 using exclusive-OR

    i = 100000;
    j = 20;
    for(j;j>0;j--);

    P1OUT &= ~0x01;

    do i--;
    while(i != 0);
}

int getTrashcanStatus(void)
{
    if(AbstandMM < 12000 && AbstandMM > 7500)
        return 1;                                   //zu 1/4 gefüllt
    else if(AbstandMM <= 7500 && AbstandMM > 5000)
        return 2;                                   //zu 1/2 gefüllt
    else if(AbstandMM <= 5000 && AbstandMM > 2500)
        return 4;                                   //zu 3/4 gefüllt
    else if(AbstandMM <= 2500 && AbstandMM > 0)
        return 8;                                   //voll
    else
        return 16;
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A (void)
{
    if(P1IN&(0x01<<5))
    {
        wert0 = TA0R;
    }
    else
    {
        wert1 = TA0R;
        if(wert0>wert1)
        {
            wert1= wert1 + 65536;
        }

        USekunden = wert1-wert0;
        /*if(USekunden>8000 | USekunden<150)
        {
            Umeter = 0;
            AbstandMM = 0;
        }
        else
        {*/
        Umeter = USekunden * Luftconst;
        AbstandMM = ((Umeter)/2)/1000;

        //}
    }


}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)
{
     switch(TA0IV)
     {
       case  2:
           break;                          // CCR1 not used
       case  4:                            // CCR 2
           P1OUT ^= 0x01;
           break;
       default: break;
     }
}


#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB0_ISR(void)
{
    switch(__even_in_range(TB0IV,14))
    {
    case 14:                                 // overflow
        GPIO_toggleOutputOnPin(GPIO_PORT_P3, GPIO_PIN6);
        break;
    default: break;
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    switch(__even_in_range(TA1IV,14))
    {
    case 14:                                 // overflow
        break;
    default: break;
    }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
    switch(__even_in_range(TA1IV,14))
    {
    case 14:                                 // overflow
        break;
    default: break;
    }
}
