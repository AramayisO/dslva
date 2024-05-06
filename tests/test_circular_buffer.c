#include <assert.h>
#include "test_circular_buffer.h"

void test_circular_buffer_init() {
  size_t size = 10;
  char buffer[size];
  circular_buffer_t circular_buffer;

  circular_buffer_init(&circular_buffer, buffer, size);

  assert(circular_buffer.buffer == buffer);
  assert(circular_buffer.head == 0);
  assert(circular_buffer.head == 0);
  assert(circular_buffer.size == size);
}

void test_circular_buffer_reset() {
  size_t size = 10;
  char buffer[size];
  circular_buffer_t circular_buffer;
  circular_buffer.head = buffer + 5;
  circular_buffer.tail = buffer + size - 1;
  circular_buffer.size = size;

  circular_buffer_reset(&circular_buffer);

  assert(circular_buffer.buffer == buffer);
  assert(circular_buffer.head == 0);
  assert(circular_buffer.head == 0);
  assert(circular_buffer.size == size);
}
