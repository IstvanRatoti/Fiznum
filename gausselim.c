#include "gauss_test.h"
#include "gauss_functions.h"

int main(int argc, char *argv[])
{
    test_functions();

    matrix_1d matrix1 = read_matrix_1d("singular2.dat");
    matrix_1d matrix2 = read_matrix_1d("identity.dat");

    gauss_eliminator(matrix1, matrix2);

    return 0;
}
