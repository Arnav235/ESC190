#include <stdlib.h>
#include <stdio.h>

int main(){
	int *p = (int *) malloc(sizeof(int));
	fprintf(stdout , "Value stored at address %p: %d\n", p, *p);
	*p = 1;
	fprintf(stdout , "Value stored at address %p: %d\n", p, *p);
	free(p);
	fprintf(stdout , "Value stored at address %p: %d\n", p, *p);
}

/*
No there is no memory leak here as p is being freed, however we have a segmentation fault
as we are reading p after it has been freed

valgrind says that there is a memory error, and that specifically there was the use of an uninitialized value

-g gives more debugging information that is generally used for GDB debugger
*/