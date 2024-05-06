#ifndef __DSLVA_TEST_CIRCULAR_BUFFER_H__
#define __DSLVA_TEST_CIRCULAR_BUFFER_H__

#include "../src/circular_buffer.h"

void test_circular_buffer_init();

void test_circular_buffer_reset();

void test_circular_buffer_get_success();

void test_circular_buffer_get_empty();

void test_circular_buffer_put_success();

void test_circular_buffer_put_full();

void test_circular_buffer_puts();

void test_circular_buffer_puts_partial();

void test_circular_buffer_puts_full();

void test_circular_buffer_empty_is_empty();

void test_circular_buffer_empty_is_not_empty();

void test_circular_buffer_full_is_full();

void test_circular_buffer_full_is_not_full();

#endif 
