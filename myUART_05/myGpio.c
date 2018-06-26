// ----------------------------------------------------------------------------
// myGpio.c  ('FR6989 Launchpad)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myGpio.h"
#include "myLcd.h"
#include "myUart.h"
#include "stdio.h"


//***** Defines ***************************************************************


//***** Global Variables ******************************************************
volatile int out = 1;
unsigned char str[80];


//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void initGPIO(void) {
    setAllPinsOutLow();
    //Setup TA2 Input Clock
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);
    //Setup TB0CCR2 Output
    //GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    //GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN6 );

    // Unlock pins (required for most FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing and enabling GPIO port interrupts
    PMM_unlockLPM5();

    // EUSCI_UART_A0 pin configuration
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    		myUart.Channels[0].TxPort,
    		myUart.Channels[0].TxPin,
    		myUart.Channels[0].TxSel
	);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
       		myUart.Channels[0].RxPort,
        	myUart.Channels[0].RxPin,
        	myUart.Channels[0].RxSel
	);

    // EUSCI_UART_A1 pin configuration
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    		myUart.Channels[1].TxPort,
    		myUart.Channels[1].TxPin,
    		myUart.Channels[1].TxSel
	);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
       		myUart.Channels[1].RxPort,
        	myUart.Channels[1].RxPin,
        	myUart.Channels[1].RxSel
	);

    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the
    // record, they are:
    //              GPIO_PIN6                            // HFXTIN on PJ.6
    //              GPIO_PIN7                            // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_PJ,
            GPIO_PIN4 +                                  // LFXIN  on PJ.4
            GPIO_PIN5 ,                                  // LFXOUT on PJ.5
            GPIO_PRIMARY_MODULE_FUNCTION
    );


    P3SEL1|=    (0x01<<6);                  //1
    P3SEL0|=    (0x00<<6);                  //1

    P1DIR |= 0x01;
    P1DIR &= ~(0x01<<5);                    //Pin 1.5 is Input
    P1SEL1|=    (0x01<<5);                  //1
    P1SEL0|=    (0x01<<5);                  //1
}

void setAllPinsOutLow()
{
    //Port1
    //GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    //GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN1 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
    //GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN5 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN7 );
    //Port2
    //GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN0 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
    //GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN1 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN7 );
    //Port3
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P3, GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN8 );
    //Port4
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN10 );
    //Port5
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN8 );
    //Port6
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P6, GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P6, GPIO_PIN8 );
    //Port7
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P7, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P7, GPIO_PIN7 );
    //Port8
    GPIO_setAsOutputPin( GPIO_PORT_P3, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN8 );
    //Port9
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN7 );
    //Port10
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN1 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN2 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN3 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN4 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN5 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN6 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN7 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P10, GPIO_PIN7 );
    GPIO_setAsOutputPin( GPIO_PORT_P10, GPIO_PIN8 );
}


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void)
{
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
        case P1IV_NONE:   break;                                                // None
        case P1IV_P1IFG0:                                                       // Pin 0
             __no_operation();
             break;
        case P1IV_P1IFG1:                                                       // Pin 1 (button 1)
        	GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );                  // Turn on LED 1
        	sprintf( (char *)str, "Pushed %d times!", out );
        	myLCD_displayNumber( (long) out );
        	myUart_writeBuf( BACKCHANNEL_UART, (unsigned char *)str, NULL, CRLF );
        	out++;
        	break;
       case P1IV_P1IFG2:                                                        // Pin 2 (button 2)
           GPIO_toggleOutputOnPin( GPIO_PORT_P9, GPIO_PIN7 );                   // Turn on LED 2
            break;
       case P1IV_P1IFG3:                                                        // Pin 3
            __no_operation();
            break;
       case P1IV_P1IFG4:                                                        // Pin 4
            __no_operation();
            break;
       case P1IV_P1IFG5:                                                        // Pin 5
            __no_operation();
            break;
       case P1IV_P1IFG6:                                                        // Pin 6
            __no_operation();
            break;
       case P1IV_P1IFG7:                                                        // Pin 7
            __no_operation();
            break;
       default:   _never_executed();
    }
}
