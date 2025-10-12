#include "fifo.h"

void fifo_worker_handler(QueueHandle_t requests, QueueHandle_t results, int id) {
    while (1) 
    {
        struct request_msg data = {};
        BaseType_t res = xQueueReceive(requests, &data, portMAX_DELAY);
        // Handle request
        data.output = data.input + 5;
        data.handled_by = id;
        xQueueSend(results, &data, portMAX_DELAY);
    }

}
