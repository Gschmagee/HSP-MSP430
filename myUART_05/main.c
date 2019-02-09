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
//static const uint16_t trashBinID = 0x000A;

unsigned char* sysReset="sys reset\r\n";
unsigned char* macPause="mac pause\r\n";
unsigned char* macDeveui="mac set deveui 0000000000000004\r\n";		//muss angepasst werden
unsigned char* macAppeui="mac set appeui 0000000000000000\r\n";
unsigned char* macAppkey="mac set appkey 01020304050607080910111213141504\r\n"; //muss angepasst werden
unsigned char* macDr="mac set dr 5\r\n";
unsigned char* macResume="mac resume\r\n";
unsigned char* macJoin="mac join otaa\r\n";
unsigned char* radioSetFreq="radio set freq 868100000\r\n";
unsigned char* radioSetSf="radio set sf sf12\r\n";
unsigned char* radioSetPwr="radio set pwr 15\r\n";
unsigned char* radioSetWdt="radio set wdt 60000\r\n";

// Werte fuers Sensor-auslesen
volatile uint32_t wert0;
volatile uint32_t wert1;
volatile uint32_t USekunden;
volatile uint32_t Umeter;
volatile uint16_t AbstandMM;
static const uint16_t Luftconst = 343;



Timer_A_initContinuousModeParam a0_contin =
{
     TIMER_A_CLOCKSOURCE_ACLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     TIMER_A_TAIE_INTERRUPT_DISABLE,
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
     540,                                       //Periode zwischen Uploads ist clock periode von unten * diese Zahl
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
     TIMER_A_DO_CLEAR,
     false
};

Timer_B_initContinuousModeParam b_contin =
{
     TIMER_B_CLOCKSOURCE_ACLK,
     TIMER_B_CLOCKSOURCE_DIVIDER_10,         // Clock periode fuer timerA1 = 2s * timerB_divider
     TIMER_B_TBIE_INTERRUPT_DISABLE,
     TIMER_B_DO_CLEAR,
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
    //initLora();

    // wait a short time
    volatile uint16_t j = 0xFFFF;
    do j--;
    while (j > 0);

    while(1)
    {
        getData();
    }
}

void initTimer()
{
    // init A0
    //Timer_A_initContinuousMode(TIMER_A0_BASE, &a0_contin);
    //Timer_A_initCaptureMode(TIMER_A0_BASE,&a0_capture);
    //Timer_A_enableInterrupt(TIMER_A0_BASE);
    // init A1
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
    Timer_A_initUpMode(TIMER_A1_BASE, &a1_up);
    // init B0
    Timer_B_initContinuousMode(TIMER_B0_BASE, &b_contin);
    Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_2);
    Timer_B_initCompareMode(TIMER_B0_BASE, &b_compare);
    Timer_B_setCompareValue(TIMER_B0_BASE, TIMER_B_CAPTURECOMPARE_REGISTER_0, 0x0002);
    // start Timers
    //Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);
}

void initLora()
{
#ifdef GATEWAYMODE
    volatile uint32_t j = 0xFFFF;
    do j--;
    while (j > 0);

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

    j = 3500000;
    do j--;
    while (j > 0);

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
    volatile uint16_t i;
    volatile uint16_t j;

    GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );      //toggle Pin 1.0 to activate the sensor
    i = 0xFFFF;
    j = 10;
    do j--;
    while (j > 0);
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    do i--;
    while(i > 0);
}

char getTrashcanStatus(void)
{
    if ((AbstandMM < 1200) && (AbstandMM > 750)) {
        return '1';                                   //1/4 filled
    }
    else if ((AbstandMM <= 750) && (AbstandMM > 500)) {
        return '2';                                   //1/2 filled
    }
    else if ((AbstandMM <= 500) && (AbstandMM > 250)) {
        return '3';                                   //3/4 filled
    }
    else if ((AbstandMM <= 250) && (AbstandMM > 0)) {
        return '4';                                   //full
    }
    else {
        return 'F';                                   //Error
    }
}

void getData()
{
    char fillLevel;
    volatile uint16_t i;
    volatile uint32_t j;
    char tx[32];

    // Init LoRa module
    initLora();

    // Init Timer A0 for sensor distance measurement
    Timer_A_initContinuousMode(TIMER_A0_BASE, &a0_contin);
    Timer_A_initCaptureMode(TIMER_A0_BASE,&a0_capture);
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);

#ifdef GATEWAYMODE
    // Read sensor
    getSensorData();

    // Convert to fillLevel
    fillLevel = getTrashcanStatus();

    // Build tx message for LoRa module
    snprintf(tx,32,"mac tx uncnf 2 0%c01\r\n",fillLevel);

    // Send message 5 times
    for(i=3; i>0;i--)
    {
        myUart_tx((unsigned char*)tx);
        j = 2000000;
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
    //switch off lora mote and sensor
    GPIO_setOutputHighOnPin( GPIO_PORT_P4, GPIO_PIN2 );

    // disable timer for sensor distance measurement
    Timer_A_stop(TIMER_A0_BASE);

    // enter low power mode
    __low_power_mode_3();
#endif
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A (void)
{
    if(P1IN&(0x01<<5))
    {
        wert0 = TA0R * 30;
    }
    else
    {
        wert1 = TA0R * 30;

        if(wert0>wert1)
        {
           wert1= wert1 + 65536;
        }
        USekunden = wert1-wert0;
        Umeter = USekunden * Luftconst;
        AbstandMM = ((Umeter)/2)/1000;
    }
}


#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
    /* Read interrupt vector register to clear interrupt */
    volatile uint16_t dummyRead = TA1IV;

    // Exit low power mode
    __low_power_mode_off_on_exit();

    // enable Lora module and sensor
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN2 );
}
