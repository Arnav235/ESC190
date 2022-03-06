#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void intset_create(struct intset **s){
	*s = (struct intset*)malloc(sizeof(struct intset));
}

void intset_add(struct intset* s, int* elems, int num_elems){
	// getting last element in array
	while (s->head->next != NULL){
		s->head = s->head->next;
	}
	for (int i = 0; i < num_elems; i++){
		struct node* new_node = (struct node*)malloc(sizeof(struct node));
		new_node->data = *elems;
		s->head->next = new_node;
		s->head = s->head->next;
		elems = elems+1;
	}
}

int intset_iselem(struct intset *s, int elem){
	while (s->head != NULL){
		if (s->head->data == elem)return 1;
	}
	return 0;
}

void intset_remove(struct intset *s, int elem){
	struct node* prev = s->head;
	struct node* curr = s->head;
	while (curr != NULL){
		if (curr->data == elem){
			if (curr == s->head){
				s->head = s->head->next;
			} else{
				prev->next = curr->next;
			}
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

//Compute the union of sets s1 and s2, and place the
//result in a new intset *s3.
void intset_union(struct intset *s1, struct intset *s2, struct intset **s3){
	struct intset* s3_temp = (struct intset*)malloc(sizeof(struct intset));
	struct node* curr = s1->head;
	while (curr != NULL){
		intset_add(s3_temp, curr->data, 1);
		curr = curr->next;
	}
	curr = s2->head;
	while (curr != NULL){
		if (!intset_iselem(s3_temp, curr->data)){
			intset_add(s3_temp, curr->data, 1);
		}
		curr = curr->next;
	}
	*s3 = s3_temp;
}


/* ------
A different problem
----- */
int** create_array(int num_rows, int num_cols[]){
	int* arr[num_rows];
	for (int i = 0; i < num_rows; i++){
		arr[i] = (int*)malloc(sizeof(int)*num_cols[i]);
	}
	return &arr;
}

void resize_array(int num_rows, int num_cols[], int **dyn_arr){
	for (int i = 0; i < num_rows; i++){
		(*dyn_arr)[i] = realloc((*dyn_arr)[i], sizeof(int)*num_cols[i]);
	}
}

void delete_array(int num_rows, int** dyn_arr){
	for (int i = 0; i < num_rows; i++){
		free((*dyn_arr)[i]);
	}
}