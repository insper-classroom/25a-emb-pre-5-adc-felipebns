#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"

const int PWM_0_PIN = 4;

const int PWM_1_PIN = 6;

void led_task(void *p) {
    gpio_set_function(PWM_0_PIN, GPIO_FUNC_PWM);
    gpio_set_function(PWM_1_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_0_PIN);
    uint slice_num_2 = pwm_gpio_to_slice_num(PWM_1_PIN);
    pwm_set_clkdiv(slice_num, 125);
    pwm_set_clkdiv(slice_num_2, 125);
    pwm_set_wrap(slice_num, 100);
    pwm_set_wrap(slice_num_2, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 80);
    pwm_set_enabled(slice_num, true);

    pwm_set_chan_level(slice_num_2, PWM_CHAN_B, 20);
    pwm_set_enabled(slice_num_2, true);

    while (true) {
    }
}

int main() {
    stdio_init_all();
    printf("Start RTOS \n");

    xTaskCreate(led_task, "LED_Task 1", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
