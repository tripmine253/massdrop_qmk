#include <stdint.h>
#include "arm_atsam_protocol.h"
#include "config_hw_version.h"
#include "config_adc.h"



static uint8_t hw_version_1 = 1; 




void hw_version_init(void)
{
    //Configure Ports for EIC
    PORT->Group[HW_VERSION_CHECK_PORT].DIRCLR.reg = 1 << HW_VERSION_CHECK_PIN; //Input
    PORT->Group[HW_VERSION_CHECK_PORT].OUTCLR.reg = 1 << HW_VERSION_CHECK_PIN; // Make it pull down
    PORT->Group[HW_VERSION_CHECK_PORT].PINCFG[HW_VERSION_CHECK_PIN].bit.INEN = 1; //Input Enable
    PORT->Group[HW_VERSION_CHECK_PORT].PINCFG[HW_VERSION_CHECK_PIN].bit.PULLEN = 1; //Pull Enable

    //  Init SRC_2_A5 and SRC_2_B5 and set them at default
    SRC_2_A5_INIT;
    SRC_2_B5_INIT;
    SRC_2_A5_ON;
    SRC_2_B5_ON;

    wait_us(1);

    //Read input 
    // This PIN shall read 0 for HW version 1 and 1 for HW version 1.5
    hw_version_1 = ~((PORT->Group[HW_VERSION_CHECK_PORT].IN.reg & (1 << HW_VERSION_CHECK_PIN))>>HW_VERSION_CHECK_PIN);  //Read PB port on Pin 
    hw_version_1 = hw_version_1 & 0x01; // Consider only LSB bit
     

}

// This function returns 1 is hw versio is 1 and 0 for version >= 1.5
uint8_t is_hw_version_1(void)
{
    return hw_version_1;
}

