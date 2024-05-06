#include <assert.h>
#include "circular_buffer.h"
#include <avr/io.h>
#include <util/delay.h>
#define __ASSERT_USE_STDERR

#define BLINK()                   \
  PORTB |= (1 << PINB5);          \
  for (int i = 0; i < 20000; i++) \
  {                               \
    PORTB |= (1 << PINB5);        \
  }                               \
  PORTB &= ~(1 << PINB5);

void circular_buffer_init(circular_buffer_handle_t self, char *buffer, size_t size)
{
  assert(self != NULL);
  assert(buffer != NULL);
  assert(size > 0);
  self->buffer = buffer;
  self->head = 0;
  self->tail = 0;
  self->size = size;
}

bool circular_buffer_empty(circular_buffer_handle_t self)
{
  assert(self != NULL);
  return self->head == self->tail;
}

bool circular_buffer_full(circular_buffer_handle_t self)
{
  assert(self != NULL);
  return ((self->head + 1) % self->size) == self->tail;
}

int circular_buffer_put(circular_buffer_handle_t self, char data)
{
  assert(self != NULL);

  int ret = 0;

  if (!circular_buffer_full(self))
  {
    self->buffer[self->head] = data;
    self->head = (self->head + 1) % self->size;
    ret = 1;
  }

  return ret;
}

int circular_buffer_puts(circular_buffer_handle_t self, char *str, size_t len)
{
  assert(self != NULL);
  assert(str != NULL);
  assert(len > 0);

  int count = 0;

  while (len > 0 && circular_buffer_put(self, str[count]))
  {
    count++;
    len--;
  }

  return 0;
}

int circular_buffer_get(circular_buffer_handle_t self, char *data)
{
  assert(self != NULL);
  assert(data != NULL);

  if (circular_buffer_empty(self))
  {
    return 0;
  }

  *data = self->buffer[self->tail];
  self->tail = (self->tail + 1) % self->size;

  return 1;
}

void circular_buffer_reset(circular_buffer_handle_t self)
{
  self->head = 0;
  self->tail = 0;
}
