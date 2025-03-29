#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int dados[5] = {0,0,0,0,0};
    int cont = 0;

    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            // implementar filtro aqui!
            if (cont < 5) {
                dados[cont] = data;
                if (cont == 4){
                    float y = (dados[4] + dados[3] + dados[2] + dados[1] + dados[0])/5;
                    printf("Média movel: %f\n", y);
                }
            } else {
                dados[0] = dados[1];
                dados[1] = dados[2];
                dados[2] = dados[3];
                dados[3] = dados[4];
                dados[4] = data;
                float y = (dados[4] + dados[3] + dados[2] + dados[1] + dados[0])/5;
                printf("Média movel: %f\n", y);
            }
            cont ++;
            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
