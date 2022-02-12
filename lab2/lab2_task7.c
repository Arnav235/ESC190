#include <stdlib.h>
#include <stdio.h>

void append(int** x, int* n, int val){
	*x = realloc(*x, (*n + 1) * sizeof(int));
	(*x)[*n] = val;
	*n = *n+1;
}

/*int main(){
	int *x = (int *)(calloc(3, sizeof(int)));
	int n = 3;
	int val = 12;
	append(&x, &n, val);
	printf("Nums: ");
	for (int i = 0; i < n; i++){
		printf(" %d ", *(x+i));
	}
	printf("\nVal of n: %d\n", n);
}*/

/*
If the first parameter was *x, then I would not be able to implement this function 
becaue then when *x is reallocated, *x might be equal to a new array somewhere else in
memory, and *x is not updated outside of the function

If the second parameter was int n, then again we would not be able to implement the function 
as then we would not be able to update the value of n to be the new length of the array,
as n is a copy of the original number
*/