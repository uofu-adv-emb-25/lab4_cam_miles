#include "signaling.h"

void signal_handle_calculation(SemaphoreHandle_t request, SemaphoreHandle_t response, struct signal_data *data)
{
    // Wait for request
    xSemaphoreTake(request, 1000);

    // Handle request
    data->output = data->input + 5;

    // Send result
    xSemaphoreGive(response);
}

BaseType_t signal_request_calculate(SemaphoreHandle_t request, SemaphoreHandle_t response, struct signal_data *data)
{
    // Send calculation request (give semaphore)
    xSemaphoreGive(request);

    // Wait for response
    if (xSemaphoreTake(response, 1000) == 1)
        return pdFAIL;
    else 
        return pdTRUE;

}
