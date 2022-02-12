#include <stdlib.h>
#include <stdio.h>

int main(){
	int** p;

	int x = 10;
	int y = 11;
	int z = 12;

	int* ptr = (int *) malloc(sizeof(int) * 3);
	*ptr = x;
	*(ptr + 1) = y;
	*(ptr + 2) = z;

	p = &ptr;

	free(ptr);
}

/*
x = 10
y = 11
z = 12
ptr -> [?, ?, ?]
then: ptr -> [10, 11, 12]. These are not the same memory addresses as x,y,z
p -> ptr
*/