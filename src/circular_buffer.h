#ifndef __DSLVA_CIRCULR_BUFFER_H__
#define __DSLVA_CIRCULR_BUFFER_H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct circular_buffer {
  char * buffer;
  size_t head;
  size_t tail;
  size_t size;
} circular_buffer_t;

typedef circular_buffer_t * circular_buffer_handle_t;

/**
 * @param self
 * @param buffer
 * @param size
 */
void circular_buffer_init(circular_buffer_handle_t self, char * buffer, size_t size);

/**
 * @param self
 */
void circular_buffer_reset(circular_buffer_handle_t self);

/**
 * Pop a single character from the front of the buffer.
 *
 * @param self
 * @param data
 * @return 0 on success, -1 if the buffer is empty.
 */
int circular_buffer_get(circular_buffer_handle_t self, char * data);

/**
 * Pushes a single character to the end of the buffer.
 *
 * @param self
 * @param data
 * @return 0 on success, -1 if the buffer is full.
 */
int circular_buffer_put(circular_buffer_handle_t self, char data);

/**
 * Pushes a string to 
 *
 * @param self
 * @param str
 * @param len
 * @return Number of characters written.
 */
int circular_buffer_puts(circular_buffer_handle_t self, char * str, size_t len);

/**
 * @param self
 * @return `true` if the buffer is empty, `false` otherwise.
 */
bool circular_buffer_empty(circular_buffer_handle_t self);

/**
 * @param self
 * @return `true` if the buffer is empty, `false` otherwise.
 */
bool circular_buffer_full(circular_buffer_handle_t self);

#endif 
