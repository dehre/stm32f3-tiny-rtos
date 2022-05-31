/**
 * The module fifo_queue shows how to use three semaphores to create a multiple-producer
 * multiple-consumer FIFO queue: producer threads will block when the FIFO is full, and
 * consumer threads will block when the FIFO is empty.
 *
 * Example:
 * ```c
 * #include "fifo_queue.h"
 *
 * FifoQueue_t fifo;
 * FifoQueue_Init(&fifo);
 *
 * FifoQueue_Put(&fifo, 234);
 * FifoQueue_Put(&fifo, 567);
 *
 * uint32_t item
 * item = FifoQueue_Get(&fifo);
 * item = FifoQueue_Get(&fifo);
 * ```
 */

#pragma once

#include "os.h"
#include <stdint.h>

#define FIFOQUEUE_SIZE 10

typedef struct
{
    uint32_t data[FIFOQUEUE_SIZE];
    uint32_t *put_pt;
    uint32_t *get_pt;
    Semaphore_t current_size;
    Semaphore_t room_left;
    Semaphore_t mutex;
} FifoQueue_t;

void FifoQueue_Init(FifoQueue_t *fifo);

void FifoQueue_Put(FifoQueue_t *fifo, uint32_t item);

uint32_t FifoQueue_Get(FifoQueue_t *fifo);
