// ----------------------------------------------------------------------------
// myClocks.c  ('FR6989 Launchpad)
//
// This routine sets up the Low Frequency crystal (LFXT) and high-freq
// internal clock source (DCO). Then configures ACLK, SMCLK, and MCLK:
//    ACLK  = 32KHz
//    SMCLK =  8MHz
//    MCLK  =  8MHz
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "myClocks.h"


//***** Defines ***************************************************************
// See additional #defines in 'myClocks.h'


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;


//***** initClocks ************************************************************
void initClocks(void) {

    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the LFXT and HFXT crystal frequencies being used so that driverlib
    //   knows how fast they are (needed for the clock 'get' functions)
    CS_setExternalClockSource(
            LF_CRYSTAL_FREQUENCY_IN_HZ,
            HF_CRYSTAL_FREQUENCY_IN_HZ
    );

    // Verify if the default clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();

    // Initialize LFXT crystal oscillator without a timeout. In case of failure
    //   the code remains 'stuck' in this function.
    // For time-out instead of code hang use CS_turnOnLFXTWithTimeout(); see an
    //   example of this in lab_04c_crystals.
    // Note that this requires PJ.6 and PJ.7 pins to be connected (and configured) 
    //   as clock input pins.
    CS_turnOnLFXT( CS_LFXT_DRIVE_0 );

    // Set DCO to run at 8MHz
    CS_setDCOFreq(
            CS_DCORSEL_1,                                                       // Set Frequency range (DCOR)
            CS_DCOFSEL_3                                                        // Set Frequency (DCOF)
    );

    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use LFXT as its oscillator source (32KHz)
    // With a 32KHz crystal and a divide by 1, ACLK should run at that rate
    CS_initClockSignal(
            CS_ACLK,                                                            // Clock you're configuring
            CS_LFXTCLK_SELECT,                                                  // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set SMCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_SMCLK,                                                           // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set MCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_MCLK,                                                            // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}

