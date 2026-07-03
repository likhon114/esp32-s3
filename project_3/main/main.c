/*
This project is for debouncing test. It will count
each button press just once and toggle the LED.
................................
Button ---GPIO41 (D12)
LED ---- GPIO21 (on board)
Board:Xiao esp32-s3 sense
.................................
Writer: Hasib Fakir
Last modified: 3rd July 2026
*/
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON GPIO_NUM_41
#define LED GPIO_NUM_21

void app_main(void)
{
    gpio_config_t io_button={
        .pin_bit_mask=(1ULL<<BUTTON),
        .mode=GPIO_MODE_INPUT,
        .pull_down_en=GPIO_PULLDOWN_DISABLE,
        .pull_up_en=GPIO_PULLUP_ENABLE,
        .intr_type=GPIO_INTR_DISABLE
    };
    gpio_config_t io_led={
        .pin_bit_mask=(1ULL<<LED),
        .mode=GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en=GPIO_PULLUP_DISABLE,
        .intr_type=GPIO_INTR_DISABLE
    };
    gpio_config(&io_button);
    gpio_config(&io_led);

    int last_state=1;
    int led_state=0;

    while (1)
    {
        int current_state=gpio_get_level(BUTTON);
        if(current_state==0&&last_state==1){
            vTaskDelay(pdMS_TO_TICKS(20));
            current_state=gpio_get_level(BUTTON);  //confirm the press
            if(current_state==0){
            printf("Button Pressed..Toggling LED\n");
            led_state=!led_state;
            gpio_set_level(LED,led_state);
            }
           
           

        }
         last_state=current_state;
         vTaskDelay(pdMS_TO_TICKS(20)); 
    }
    

}