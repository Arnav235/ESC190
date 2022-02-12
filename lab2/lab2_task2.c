#include <stdio.h>

int main(){
	int x = 0;
	int y = x;
	int* ptr_x = &x;
	int* ptr_y = &y;
	int** pptr_x = &ptr_x;
	int** pptr_y = &ptr_y;
	fprintf(stdout ,
	"x contains the value %d at location %p\n", *ptr_x , ptr_x
	);
	fprintf(stdout ,
	"y contains the value %d at location %p\n", *ptr_y , ptr_y
	);
}

/*
Memory model:
x is at 0x7ff7b923d3dc (contains 0)
y is at 0x7ff7b923d3d8 (contains 0)
ptr_x -> x (at 0x7ff7b923d3dc)
ptr_y -> y (at 0x7ff7b923d3d8)
pptr_x -> ptr_x (at 0x7ff7b915d3dc)
pptr_y -> ptr_y (at 0x7ff7b953e3dc)

Values:
*pptr_x = 0
*pptr_y = 0
**pptr x = 0
**pptr y = 0

You actually can't make x and y share the same memory location, unless you make 
y a pointer to x. However then y would be a pointer not an int variable
*/