/*this is to blink on board LED in gpio21*/

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LED_PIN GPIO_NUM_21




void app_main(void)
{
    gpio_config_t io_config={
    .pin_bit_mask=(1ULL<<LED_PIN),
    .pull_down_en=GPIO_PULLDOWN_DISABLE,
    .pull_up_en=GPIO_PULLUP_DISABLE,
    .mode=GPIO_MODE_OUTPUT,
    .intr_type=GPIO_INTR_DISABLE
};
    gpio_config(&io_config);

    while (1)
    {
        printf("On\n");
        gpio_set_level(LED_PIN,0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Off\n");
        gpio_set_level(LED_PIN,1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    

}