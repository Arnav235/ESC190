#if !defined(INTSET_H)
#define INTSET_H

struct intset{
    struct node *head;
};

struct node{
    int data;
    struct node *next;
};

//Create an intset, and place a pointer to it in *s.
void intset_create(struct intset **s);

//Add the elements from array elems to intset s. There are
//num_elems elements in elems
void intset_add(struct intset *s, int *elems, int num_elems);

//Return 1 if element elem is in intset s. Otherwise return 0.
int intset_iselem(struct intset *s, int elem);

//Remove element elem from s, of eleme is in s. If the element 
//is not in s, do nothing.
//free memory that's not needed anymore
void intset_remove(struct intset *s, int elem);

//Compute the union of sets s1 and s2, and place the
//result in a new intset *s3.
void intset_union(struct intset *s1, struct intset *s2, struct intset **s3);

#endif