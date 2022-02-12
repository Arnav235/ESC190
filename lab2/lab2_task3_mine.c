#include <stdio.h>

int main(){
	int y[2] = {1, 5};
	int x[3];
	for (int i = 0; i < 3; i++){
		x[i] = y[i];
	}
	x[2] = 9;
	y[2] = 9;
	fprintf(stdout ,
	"Addresses of x 0: %p, 1: %p, 2: %p\n", x, x + 1, x + 2);
	fprintf(stdout , "Values of x through dereferencing 0: %d, 1: %d, 2: %d\n", *x, *(x + 1), *(x + 2));
	fprintf(stdout ,
	"Values of x through indexing 0: %d, 1: %d, 2: %d\n",
	x[0], x[1], x[2]
	);
	fprintf(stdout ,
	"Indexing out of range: %d\n", x[3]
	);
}

/*
When you attempt to access an element that is out of range, it still compiles but gives a warning.
It then prints a random value (it's printing 1 for me right now), and there's an error and aborts

When we try to extend the size of array by doing y[2] = 9, it just gives another warning

Diagram:
x:
0x7ff7b31f23c4: 1
0x7ff7b31f23c8: 5
0x7ff7b31f23cc: 9

y:
0x7ff7b5a7a3d0: 1
0x7ff7b5a7a3d4: 5
0x7ff7b5a7a3d8: 9 -- not apart of the initially allocated block of memory

*/