#include <stdint.h>
#include <string.h>
#include "arm_atsam_protocol.h"
#include "config_hw_version.h"
#include "config_adc.h"


void  update_adc_config(uint8_t *p_dst_config)
{

    // Prepare New ADC config for hs version 1.5 
    uint8_t local_adc_config[][ADC_SAMPLEN + 1] = {
        {   ADC_CONFIG_5V  },
        {   ADC_CONFIG_C1A5_V1_5  },
        {   ADC_CONFIG_C1B5_V1_5  },
        {   ADC_CONFIG_C2A5_V1_5  },
        {   ADC_CONFIG_C2B5_V1_5  },
        {   ADC_CONFIG_C1I },
        {   ADC_CONFIG_C2I }
    };

    hw_version_init();

    if (is_hw_version_1())
    {
        // Old HW 
        // initialized ADC config is correct, return
        return;
    }
    uint8_t * p_src_config = &local_adc_config[0][0];
   // uint8_t * p_dst_config = &adc_config[0][0];
    uint8_t size = (ADC_SAMPLEN+1)*(ADC_C2I+1);
    memcpy((void*) p_dst_config, (void*) p_src_config, size);
}
