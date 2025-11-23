#include <stm32f4xx.h>
#include <FreeRTOS.h>
#include <task.h>

static TaskHandle_t blinkyTaskHandle;

void blinkyTask(void* pvArg) {
    while(1) {
        GPIOC->ODR ^= GPIO_ODR_OD13; // toggle PC13
        vTaskDelay(pdMS_TO_TICKS(1000)); // delay for 1000ms
    }
}

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // turn on GPIOC clock
    // initialize PC13 to output (01: General purpose output mode in MODER)
    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE13, GPIO_MODER_MODE13_0); 
    /* create task with 256 stack elements (=1KB) space, dynamically allocated */
    if ( xTaskCreate(&blinkyTask, "blinky", 256U, NULL, tskIDLE_PRIORITY + 2, &blinkyTaskHandle) != pdPASS) {
        while(1); // failed to create task
    } 
    vTaskStartScheduler();
    // should never reach this line unless scheduler stops!
    while(1);
    return 0;
}

/* hook functions (only one implemented) */
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName ) { while(1); }