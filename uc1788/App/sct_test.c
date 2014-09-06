/**
 * sct_test.c
 *
 * External ROM Scatter Test
 *
 * Guo Jian
 * 2014.04.28
 */

#include "stdio.h"

void __attribute__((section ("TEST_SECTION"))) SctTest(void)
{
//    int i;
//    for (i = 0; i < 3; ++ i)
//        ;
    printf("test passed~~~~~~~~~~~~~~~~.\n");
}
