#include "matr_functions.h"

int main(int argc, char *argv[])
{
	matrix mat = read_matrix(argv[1]);

	//mat = read_matrix("data.txt");
	write_matrix(mat, "stdout");

	return 0;
}
