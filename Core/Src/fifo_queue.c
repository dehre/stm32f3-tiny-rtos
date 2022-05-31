//==================================================================================================
// INCLUDES
//==================================================================================================

#include "fifo_queue.h"

#include <string.h>

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

/* These #defines improve code readability */

#define fifo_data (fifo->data)
#define fifo_put_pt (fifo->put_pt)
#define fifo_get_pt (fifo->get_pt)
#define fifo_current_size (fifo->current_size)
#define fifo_room_left (fifo->room_left)
#define fifo_mutex (fifo->mutex)

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

void FifoQueue_Init(FifoQueue_t *fifo)
{
    memset(fifo_data, 0x00, FIFOQUEUE_SIZE * sizeof(uint32_t));
    fifo_put_pt = fifo_get_pt = &fifo_data[0];
    fifo_current_size = 0;
    fifo_room_left = FIFOQUEUE_SIZE;
    fifo_mutex = 1;
}

void FifoQueue_Put(FifoQueue_t *fifo, uint32_t item)
{
    OS_Semaphore_Wait(&fifo_room_left);
    OS_Semaphore_Wait(&fifo_mutex);

    *fifo_put_pt = item;
    fifo_put_pt++;
    if (fifo_put_pt == &fifo_data[FIFOQUEUE_SIZE])
    {
        /* Wrap */
        fifo_put_pt = &fifo_data[0];
    }

    OS_Semaphore_Signal(&fifo_mutex);
    OS_Semaphore_Signal(&fifo_current_size);
}

uint32_t FifoQueue_Get(FifoQueue_t *fifo)
{
    OS_Semaphore_Wait(&fifo_current_size);
    OS_Semaphore_Wait(&fifo_mutex);

    uint32_t item = *fifo_get_pt;
    fifo_get_pt++;
    if (fifo_get_pt == &fifo_data[FIFOQUEUE_SIZE])
    {
        /* Wrap */
        fifo_get_pt = &fifo_data[0];
    }

    OS_Semaphore_Signal(&fifo_mutex);
    OS_Semaphore_Signal(&fifo_room_left);
    return item;
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================
