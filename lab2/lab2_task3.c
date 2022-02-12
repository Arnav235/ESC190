#include <stdio.h>
#include <stdlib.h>

int main()
{
	// # index  0 1
	int y[2] = {1,5};
	//           0 1 2 
	int x[3]; //{ , , }

	for(int i = 0; i < 3; ++i)
	{
		x[i] = y[i];
	}

	x[2] = 9;

	fprintf(stdout ,"Addresses of x 0: %p, 1: %p, 2: %p\n",x, x + 1, x + 2);

	fprintf(stdout ,"Values of x through dereferencing 0: %d, 1: %d, 2: %d\n",
	*x, *(x + 1), *(x + 2));
	fprintf(stdout ,"Values of x through indexing 0: %d, 1: %d, 2: %d\n", x[0], 
	x[1], x[2]);
	fprintf(stdout ,"Indexing out of range: %d\n", x[3] );

	/* Questions
	#1 When accesing an element out of range the causing a compilation error
	   that warns the coder that the index # is past the end of the array
	#2 Extending the array causes a compiler error as arrays are static and
	   therefore they cannot be edited once declared, therefore the compiler 
	   points to the place where the array was declared as a possible suggestion
	   to increase the size of the array. Why does clobbering not work ?
	   
	   Memory Model
	   
	   [Befor the loop only due to declaration]
	   Label 	Address		Value
	   x[0] 	400-407		  
	   x[1]		408-415		  
	   x[2]		416-423		  
	   y[0]		700-707		  1
	   y[1]		708-715		  5

	   [After the loop]
	   Label 	Address		Value
	   x[0] 	400-407		  1
	   x[1]		408-415		  5
	   x[2]		416-423		  
	   y[0]		700-707		  1
	   y[1]		708-715		  5

	   [After the assigment of 3 index in array x]
	   Label 	Address		Value
	   x[0] 	400-407		  1
	   x[1]		408-415		  5
	   x[2]		416-423		  9
	   y[0]		700-707		  1
	   y[1]		708-715		  5
	   */
}