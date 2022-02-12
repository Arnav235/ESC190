#include <stdlib.h>
#include <stdio.h>

int main(){
	int *p = (int *) malloc(sizeof(int));
	int *q = (int *) malloc(sizeof(int) * 2);
	*p = 1;
	*q = 10;

	q = p;
	free(q);
	free(p);
}

/*
Memory model
p -> [?]
q -> [?, ?]
then:
p -> 1
q -> 10
then:
q -> 1

the memory malpractise is that the originally allocated blocks of memory using malloc have 
not been freed. And there is an invalid free on the variables p and q which now point to memory which 
weren't allocated through malloc
*/