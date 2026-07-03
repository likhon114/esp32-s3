/*this code is for button input using pulling method. It will use
the GPIO41 (D12) on Xiao esp32-s3 sense module
*/

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON GPIO_NUM_41



void app_main(void)
{
    gpio_config_t io_config={
    .pin_bit_mask=(1ULL<<BUTTON),
    .intr_type=GPIO_INTR_DISABLE,
    .mode=GPIO_MODE_INPUT,
    .pull_down_en=GPIO_PULLDOWN_DISABLE,
    .pull_up_en=GPIO_PULLUP_ENABLE
};
gpio_config(&io_config);
while (1)
{
   int a=gpio_get_level(BUTTON);
   if(a){
    printf("Button not pressed\n");

   }
   else printf("Button Pressed\n");

   vTaskDelay(pdMS_TO_TICKS(200));
}


}