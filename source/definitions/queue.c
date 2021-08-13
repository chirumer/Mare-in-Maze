#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct Queue* queue_create() {

    struct Queue* queue = malloc(sizeof(struct Queue));
    *queue = (struct Queue){ 0, NULL, NULL };

    return queue;
}

void queue_destroy(struct Queue* queue) {

    struct Queue_node* temp;

    while (queue->head != NULL) {
        
        temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    queue->tail = NULL;
    queue->size = 0;
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

    if (queue_size(queue) <= 0) {
        fprintf(stderr, "queue_pop() tried to pop empty queue\n");
        exit(EXIT_FAILURE);
    }

    struct Queue_node* prev_head = queue->head;
    void* data = prev_head->data;

    queue->head = queue->head->next;
    free(prev_head);

    queue->size--;

    return data;
}

void* queue_peek(struct Queue* queue) {

    if (queue_size(queue) <= 0) {
        fprintf(stderr, "queue_peek() tried to peek empty queue\n");
        exit(EXIT_FAILURE);
    }

    return queue->head->data;
}

int queue_size(struct Queue* queue) {
    return queue->size;
}
