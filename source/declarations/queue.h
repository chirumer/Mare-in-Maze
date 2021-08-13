#ifndef QUEUE_H
#define QUEUE_H

struct Queue {
    int size;
    struct Queue_node* head;
    struct Queue_node* tail;
};

struct Queue_node {
    struct Queue_node* next;
    void* data;
};

struct Queue* queue_create();
    // create queue

void queue_destroy(struct Queue*);
    // destroy queue

void queue_push(struct Queue*, void* data);
    // push data to end of queue

void* queue_pop(struct Queue*);
    // pop data from front of queue

void* queue_peek(struct Queue*);
    // peek data from front of queue

int queue_size(struct Queue*);
    // get size of queue

#endif
