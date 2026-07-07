/*
This code test the interrupt for GPIO. It toggles LED based on interrupt.
It uses ISR handle and queue technique to send the information.
Input >> GPIO 8,9 (D9,10)
Output >> GPIO 21,22 (On board LED)
..........................................
Writer: Hasib Likhon
Last Modified: 6th July 2026
*/

#include "stdio.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static QueueHandle_t queue=NULL;
#define buttonA GPIO_NUM_8
#define buttonB GPIO_NUM_9
#define ledA GPIO_NUM_21
#define ledB GPIO_NUM_41

void handler(void *arg){
    gpio_num_t gpio=(gpio_num_t) (uint32_t) arg;
    xQueueSendFromISR(queue,&gpio,NULL);

}

void receiver(void *arg){
    gpio_num_t gpio;
    int ledstateA=0,ledstateB=0;
    while (1)
    {
       if (xQueueReceive(queue,&gpio,portMAX_DELAY)==pdTRUE){
        vTaskDelay(pdMS_TO_TICKS(30));
        switch (gpio)
        {
        case buttonA:
        gpio_intr_disable(buttonA);
            if(gpio_get_level(buttonA)==0){
            
            // ledstateA=gpio_get_level(ledA);  //HW read has some fallback.track it using SW
            ledstateA=!ledstateA;
            printf("Button A pressed: %d \n",ledstateA);
            gpio_set_level(ledA,ledstateA);
            }

            while (gpio_get_level(buttonA)==0)
            {
               vTaskDelay(pdMS_TO_TICKS(10));
            }
            gpio_intr_enable(buttonA);
            break;
        case buttonB:
            gpio_intr_disable(buttonB);
            if(gpio_get_level(buttonB)==0){
            
             //ledstateB=gpio_get_level(ledB);
            ledstateB=!ledstateB;
            gpio_set_level(ledB,ledstateB);
            printf("Button B pressed: %d \n",ledstateB);
            }
               while (gpio_get_level(buttonB)==0)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
               
            }
            gpio_intr_enable(buttonB);
            break;
        
        default:
            break;
        }

       }
      
    }
 
    
}

void app_main(void){
    gpio_config_t button_config={
    .pin_bit_mask=(1ULL<<buttonA) | (1ULL<<buttonB),
    .mode=GPIO_MODE_INPUT,
    .intr_type=GPIO_INTR_NEGEDGE,
    .pull_down_en=GPIO_PULLDOWN_DISABLE,
    .pull_up_en=GPIO_PULLUP_ENABLE
};

gpio_config_t led_config={
    .pin_bit_mask=(1ULL<<ledA) | (1ULL<<ledB),
    .mode=GPIO_MODE_OUTPUT,
    .intr_type=GPIO_INTR_DISABLE,
    .pull_down_en=GPIO_PULLDOWN_DISABLE,
    .pull_up_en=GPIO_PULLUP_DISABLE
};

gpio_config(&led_config);
gpio_config(&button_config);

queue=xQueueCreate(5,sizeof(gpio_num_t));
gpio_install_isr_service(0);
gpio_isr_handler_add(buttonA,handler,(void*) buttonA);
gpio_isr_handler_add(buttonB,handler,(void*) buttonB);

xTaskCreate(receiver,"queue receiver",1024*4,NULL,1,NULL);



}