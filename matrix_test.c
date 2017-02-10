#include"matr_functions.h"

int main(int argc, char *argv[])
{
	matrix mat;
	
	mat = read_matrix(argv[1]);
	write_matrix(mat, "stdout");
	
	return 0;
}
