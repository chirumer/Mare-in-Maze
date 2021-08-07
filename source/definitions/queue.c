#include <stddef.h>
#include <stdlib.h>
#include "queue.h"

struct Queue* queue_create() {

    struct Queue* queue = malloc(sizeof(struct Queue));
    *queue = (struct Queue){ 0, NULL, NULL };

    return queue;
}

void queue_push(struct Queue* queue, void* data) {

    if (queue->size == 0) {
            // first node
        queue->head = malloc(sizeof(struct Queue_node));
        queue->tail = queue->head;
    }
    else {
        queue->tail->next = malloc(sizeof(struct Queue_node));
        queue->tail = queue->tail->next;
    }
    *queue->tail = (struct Queue_node){ NULL, data };

    queue->size++;
}

void* queue_pop(struct Queue* queue) {

    void* data = queue->head->data;
    queue->head = queue->head->next;

    queue->size--;

    return data;
}

void* queue_peek(struct Queue* queue) {
    return queue->head->data;
}

int queue_size(struct Queue* queue) {
    return queue->size;
}
