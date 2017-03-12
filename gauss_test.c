#include<stdio.h>

#include "gauss_functions.h"
#include "gauss_test.h"

int get_largest_test(void)
{
    int success = 1;

    pair pr;

    matrix_1d mat = read_matrix_1d("test1.dat");

    //write_matrix_1d(mat, "stdout");   Debug code.

    // Test invalid start position.
    pr = get_largest(mat, 3);
    if(-1==pr.num2)
        success &= 1;
    else
    {
        fprintf(stderr, "Start position check failed!\n");
        success &= 0;
    }

    // Test for largest positive value.
    pr = get_largest(mat, 0);
    if((2==pr.num1)&&(0==pr.num2))
        success &= 1;
    else
    {
        fprintf(stderr, "Failed to choose the largest absolute value!\n");
        success &= 0;
    }

    // Test for a "larger" negative value.
    set_value(0, 1, mat, -5.12);
    pr = get_largest(mat, 0);
    if((0==pr.num1)&&(1==pr.num2))
        success &= 1;
    else
    {
        fprintf(stderr, "Failed to choose the largest absolute value!\n");
        success &= 0;
    }

    // Test for a smaller negative value.
    set_value(2, 0, mat, -12.462);
    pr = get_largest(mat, 0);
    if((2==pr.num1)&&(0==pr.num2))
        success &= 1;
    else
    {
        fprintf(stderr, "Failed to choose the largest absolute value!\n");
        success &= 0;
    }

    // Test for an even larger positive value.
    set_value(0, 2, mat, 122.47);
    pr = get_largest(mat, 0);
    if((0==pr.num1)&&(2==pr.num2))
        success &= 1;
    else
    {
        fprintf(stderr, "Failed to choose the largest absolute value!\n");
        success &= 0;
    }

    // Test for singularity.
    pr = get_largest(mat, 1);
    if(-1==pr.num1)
        success &= 1;
    else
    {
        fprintf(stderr, "Singularity not detected!\n");
        success &= 0;
    }

    return success;
}

int switch_rows_test(void)
{
    int i, temp = 0, success = 1;

    matrix_1d mat = read_matrix_1d("test2.dat");

    //write_matrix_1d(mat, "stdout");   Debug code.

    // Test for invalid rows to switch.
    if(!switch_rows(3, 5, mat))
        success &= 1;
    else
    {
        fprintf(stderr, "Invalid row switch not detected!\n");
        success &=0;
    }

    // Test the switch for every element.
    switch_rows(0, 1, mat);
    for(i=0;i<mat.columns;i++)
    {
        temp = (int )get_value(0, i, mat);
        if(temp)    // These values should be 0.
        {
            fprintf(stderr, "Value switch failed!\n");
            success &=0;
            break;
        }

        temp = (int )get_value(1, i, mat);
        if(!temp)    // These values should be 1.
        {
            fprintf(stderr, "Value switch failed!\n");
            success &=0;
            break;
        }
    }

    //write_matrix_1d(mat, "stdout");   Debug code.

    return success;
}

int switch_columns_test(void)
{
    int i, temp = 0, success = 1;
    pair pr;

    matrix_1d mat = read_matrix_1d("test3.dat");

    //write_matrix_1d(mat, "stdout");   //Debug code.

    // Test for invalid rows to switch.
    pr = switch_columns(3, 5, mat);
    if((-1==pr.num1))
        success &= 1;
    else
    {
        fprintf(stderr, "Invalid column switch not detected!\n");
        success &= 0;
    }

    // Test the switch for every element.
    pr = switch_columns(0, 1, mat);
    for(i=0;i<mat.columns;i++)
    {
        temp = (int )get_value(i, 0, mat);
        if(temp)    // These values should be 0.
        {
            fprintf(stderr, "Value switch failed!\n");
            success &= 0;
            break;
        }

        temp = (int )get_value(i, 1, mat);
        if(!temp)    // These values should be 1.
        {
            fprintf(stderr, "Value switch failed!\n");
            success &= 0;
            break;
        }
    }
    if((0!=pr.num1)&&(1!=pr.num2))  // Function should return a 0 in num1 and a 1 in num2.
    {
        fprintf(stderr, "Function did not correctly return the switched columns!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");   //Debug code.

    return success;
}

int mult_row_test(void)
{
    int i, success = 1;
    int temp;

    matrix_1d mat = read_matrix_1d("test4.dat");

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test invalid row.
    if(!mult_row(1.3, 3, mat))
        success &= 1;
    else
    {
        fprintf(stderr, "Invalid row argument not detected!\n");
        success &= 0;
    }

    // Test multiplication with a positive integer.
    if(mult_row(2, 0, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(2==temp))     // temp should be equal to 3.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test multiplication with a negative integer.
    if(mult_row(-3, 0, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(-6==temp))     // temp should be equal to 3.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test multiplication with a double.
    if(mult_row(1.5, 0, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(-9==temp))     // temp should be equal to 3.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test multiplication with a value less than 1 (division).
    if(mult_row(-1./9., 0, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(1==temp))     // temp should be equal to 3.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    return success;
}

int sub_row_test(void)
{
    int i, success = 1;
    int temp;

    matrix_1d mat = read_matrix_1d("test5.dat");

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test invalid rows.
    if(!sub_row(5, 10, mat))
        success &= 1;
    else
    {
        fprintf(stderr, "Invalid row arguments not detected!\n");
        success &= 0;
    }

    // Test subtraction with positive integers.
    if(sub_row(0, 1, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(0==temp))     // temp should be equal to 0.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test subtraction with negative integers(addition).
    if(sub_row(0, 2, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(0, i, mat);
            if(!(1==temp))     // temp should be equal to 1.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    // Test subtraction with doubles.
    if(sub_row(3, 4, mat))
    {
        for(i=0;i<mat.columns;i++)
        {
            temp = (int )get_value(3, i, mat);
            if(!(-1==temp))     // temp should be equal to -1.
            {
                fprintf(stderr, "Value did not match the expected!\n");
                success &= 0;
                break;
            }
        }
    }
    else    // If for some reason, the function didn't return 1.
    {
        fprintf(stderr, "Function failed to return expected value!\n");
        success &= 0;
    }

    //write_matrix_1d(mat, "stdout");     // Debug code.

    return success;
}

void test_functions(void)
{
    fprintf(stderr, "Testing functions...\n");

    if(get_largest_test())
        fprintf(stderr, "Function get_largest is OK.\n");

    if(switch_rows_test())
        fprintf(stderr, "Function switch_rows is OK.\n");

    if(switch_columns_test())
        fprintf(stderr, "Function switch_columns is OK.\n");

    if(mult_row_test())
        fprintf(stderr, "Function mult_row is OK.\n");

    if(sub_row_test())
        fprintf(stderr, "Function sub_row is OK.\n");

    fprintf(stderr, "Function tests complete.\n");
}
