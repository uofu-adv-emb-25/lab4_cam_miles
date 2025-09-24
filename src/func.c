#include "func.h"
#include <stdio.h>


int increment_counter(SemaphoreHandle_t counter_semaphore, int* counter_ptr)
{
    int result = xSemaphoreTake(counter_semaphore, (TickType_t) 10);
    if (result == pdTRUE)
    {
        *counter_ptr += 1;
        xSemaphoreGive(counter_semaphore);
    }
    
    return result;
}

//Deadlocked Code
void taskA(void *arg)
{
    deadlockParams *args = (deadlockParams *) arg;

    // Acquire lock A  
    args->counter += xSemaphoreTake(args->A, portMAX_DELAY);

    // Delay to allow task B to acquire lock B
    vTaskDelay(1000);

    // Attempt to acquire lock B
    args->counter += xSemaphoreTake(args->B, portMAX_DELAY);
    
    xSemaphoreGive(args->B);
    xSemaphoreGive(args->A);

    vTaskSuspend(NULL);
}

void taskB(void *arg) 
{
    deadlockParams *args = (deadlockParams *) arg;
    // Acquire lock B
    args->counter += xSemaphoreTake(args->B, portMAX_DELAY);

    // Attempt to acquire lock A
    args->counter += xSemaphoreTake(args->A, portMAX_DELAY);

    xSemaphoreGive(args->A);
    xSemaphoreGive(args->B);

    vTaskSuspend(NULL);
}

void orphaned_lock(void *arg)
{
    orphanParams *args = (orphanParams *) arg;
    while (1) {
        xSemaphoreTake(arg->semaphore, portMAX_DELAY);
        arg->counter++;
        if (arg->counter % 2) {
            continue;
        }
        printf("Count %d\n", arg->counter);
        xSemaphoreGive(arg->semaphore);
    }
}

void orphaned_lock_fix(void *arg)
{
    orphanParams *args = (orphanParams *) arg;
    while (1) {
        xSemaphoreTake(arg->semaphore, portMAX_DELAY);
        arg->counter++;
        if (arg->counter % 2) {
            xSemaphoreGive(arg->semaphore);
            continue;
        }
        printf("Count %d\n", arg->counter);
        xSemaphoreGive(arg->semaphore);
    }
}