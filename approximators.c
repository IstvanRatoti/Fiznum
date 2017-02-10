/*
	Fifth-order Runge-Kutta-Fehlberg method, using values suggested by Cash and Karp. Source: Press, Teukolsky, Vetterling, Flannery - Numerical Recipes in C
*/
#include<math.h>

typedef double (* function) (double *init_values);

																	// data includes initial values, position, misc arguments, step size and error in order.
double *rkck_step(function *functions, function *derivs, int function_count, int misc_arg_count, double *data, double desired_error, double safety_factor)
{	
	const double a[5]={0.2, 0.3, 0.6, 1, 0.875};																								// Values provided by Cash and Karp.
	const double c[6]={(double)37/378, 0, (double)250/621, (double)125/594, 0, (double)512/1771};
	const double c_s[6]={(double)2825/27648, 0, (double)18575/48384, (double)13525/55296, (double)277/14336, 0.25};
	const double b[5][5]={{0.2, 0.075, 0.6, (double)-11/54, (double)1631/55296}, {0, 0.225, -0.9, 2.5, (double)175/512},
						  {0, 0, 1.2, (double)-70/27, (double)575/13824}, {0, 0, 0, (double)35/27, (double)44275/110592},
						  {0, 0, 0, 0, (double)253/4096}};
	
	int i,j,l,m, redo=0;
	//int waeva;
	int not_done=1;
	double step_size=data[function_count+1+misc_arg_count];
	double next_step_size=0;															// Just variables, notin' to do here...
	double chosen_error=0;
	
	double *desired_error_values=(double *)calloc(function_count, sizeof(double));
	double *out_data=(double *)calloc(function_count+3, sizeof(double));
	double *sum_arg=(double *)calloc(function_count+1+misc_arg_count, sizeof(double));
	double *error=(double *)calloc(function_count, sizeof(double));
	double *k=(double *)calloc(6*function_count, sizeof(double));
 
	do
	{		
		for(l=0;l<(function_count+1+misc_arg_count);l++)
			sum_arg[l]=data[l];							// Input data into a new array, so we dont mess up our initial values.
		
		for(i=0;i<6;i++)
		{	
			for(j=0;j<function_count;j++)
				k[j+function_count*i]=step_size*functions[j](sum_arg);	// Calculating each k for every function.
		
			if(i<5)		// We dont want to reset sum_arg for the last step, because we wont need it anymore.
			{
				for(l=0;l<function_count;l++)
				{
					sum_arg[l]=data[l];				// After each calculation of k, we reset sum_arg.
			
					for(m=0;m<(i+1);m++)
						sum_arg[l]+=b[m][i]*k[l+function_count*m];		// Recalculate sum_arg after reset, to be used in the next k calculation.
				}

				sum_arg[function_count]=data[function_count]+a[i]*step_size;	// And we reset the position too.
			}
		}
	
		for(i=0;i<function_count;i++)
		{
			out_data[i]=data[i];	// Setting output data for this step, again, not messing with the initial ones. We need them if we are not satisfied with the current one.
		
			for(j=0;j<6;j++)
			{
				if(j==0)
					error[i]=0;		// Makes sure no junk is in the error array.
			
				out_data[i]+=c[j]*k[i+function_count*j];	// Setting the data, in accordance with the formula.
				error[i]+=(c[j]-c_s[j])*k[i+function_count*j];	// Error too, so we can decide, if we accept this step.
			}
			
			error[i]=fabs(error[i]);	// Need an absolute value, for the code below to work properly.
			
			desired_error_values[i]=desired_error*(out_data[i]+step_size*derivs[i](sum_arg));	// We might not want the same precision for all of our values.
			
			if((!redo)&&(error[i]>desired_error_values[i]))
				redo=1;											// Redo step if the errors are not acceptable.
		
			error[i]=(double)error[i]/(fabs(out_data[i]));		// We want fractional error, so they can be compared.
		
			if(error[i]>chosen_error)
				chosen_error=error[i];		// Always chose the highest error, for safety.
		}
	
		if(redo)
		{
			step_size=safety_factor*step_size*pow(((double)desired_error/chosen_error), 0.25);		// Set and calculate step_size for retrying the step.
			
			redo=0;
			chosen_error=0;
		}
		else
		{
			next_step_size=safety_factor*step_size*pow(((double)desired_error/chosen_error), 0.2);		// Set and calculate step_size for the next step.
			if(next_step_size>(2.0*step_size))
				step_size=2.0*step_size;			// Safety measure, so we dont end up hopping too big next time.
			else
				step_size=next_step_size;
			
			out_data[function_count+1]=step_size;
			out_data[function_count]=data[function_count]+step_size;		// Setting the out_data.
			out_data[function_count+2]=chosen_error;
			not_done=0;
		}
	}while(not_done==1);
	
	for(i=0;i<(function_count+1);i++)
	{
		data[i]=out_data[i];		// Setting the accepted step's data to the input data, so we can free up all the allocated memory.
	}
	data[function_count+1+misc_arg_count]=out_data[function_count+1];	// Setting stepsize.
	data[function_count+2+misc_arg_count]=out_data[function_count+2];	// Setting error.
	
	free(out_data);
	free(sum_arg);		// 13th Ammendment
	free(error);
	free(k);
	
	return data;
}
