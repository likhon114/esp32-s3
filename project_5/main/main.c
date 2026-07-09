/*
This code convert an analog signal to digital using esp ADC unit.
There are 2 ADC unit in esp and I use adc1 here.
adc2 is not suggested while using with WiFi driver.
I converted the raw adc reading using the calibration driver of esp
.......................................................
Writer: Hasib Likhon
Last modified: 9th July 2026
*/

#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define ADC_UNIT ADC_UNIT_1
#define ADC_CHANNEL ADC_CHANNEL_6
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BITWIDTH ADC_BITWIDTH_DEFAULT


adc_oneshot_unit_handle_t adc_handle;
adc_cali_handle_t cali;

void app_main(void)
{
    adc_oneshot_unit_init_cfg_t unit={   //unit configuration
        .unit_id=ADC_UNIT,
        .ulp_mode=ADC_ULP_MODE_DISABLE
    };
    adc_oneshot_new_unit(&unit,&adc_handle);  

    adc_oneshot_chan_cfg_t chan={  //channel configuration
        .atten=ADC_ATTEN,
        .bitwidth=ADC_BITWIDTH
    };
    adc_oneshot_config_channel(adc_handle,ADC_CHANNEL,&chan); 

    adc_cali_curve_fitting_config_t cali_conf={
        .atten=ADC_ATTEN,
        .bitwidth=ADC_BITWIDTH,
        .chan=ADC_CHANNEL,
        .unit_id=ADC_UNIT
    };
    adc_cali_create_scheme_curve_fitting(&cali_conf,&cali);




    while (1)
    {
        int raw,voltage;
        adc_oneshot_read(adc_handle,ADC_CHANNEL,&raw);
        adc_cali_raw_to_voltage(cali,raw,&voltage);
        printf("Raw  %4d | Voltage %4d mV\n",raw,voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    


}