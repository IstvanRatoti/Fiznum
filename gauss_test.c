#include<stdio.h>

#include "gauss_functions.h"
#include "gauss_test.h"

void test_functions(void)
{
    if(get_largest_test())
        fprintf(stderr, "Function get_largest is OK.\n");
}

int get_largest_test(void)
{
    int success = 0;
    matrix_1d mat;

    pair pr = get_largest(mat);
    if((0==pr.num1)&&(0==pr.num2))
        success = 1;
    else
    {
        fprintf(stderr, "There was an error!\n");
        success = 0;
    }

    return success;
}
