#include "lab3.h"
#include <stdio.h>

//Add TA party item request to the list
int add_request(struct party_node **head, char *item, double price, char *ta){
    if (strcmp(item, "IDE") == 0){
        return -1; // request not added
    }
    struct party_node* new_node = (struct party_node *)malloc(sizeof(struct party_node));
    new_node->item = (char *)malloc(sizeof(char) * (strlen(item) +1));
    new_node->ta = (char *)malloc(sizeof(char) * (strlen(ta) +1));
    for (int i = 0; i < strlen(item) +1; i++) {new_node->item[i] = item[i];}
    for (int i = 0; i < strlen(ta) +1; i++) {new_node->ta[i] = ta[i];}
    new_node->price = price;

    new_node->next = *head;
    *head = new_node; 
    return 0;
}

//Remove the last item added
void remove_request(struct party_node **head){
    struct party_node* temp = *head;
    if (temp == NULL){
        return;
    }
    (*head) = (*head)->next;
    free(temp->item);
    free(temp->ta);
    free(temp);
}

//Sort party item requests - in place?
void make_sorted(struct party_node **head){
    struct party_node* prev = (struct party_node*)malloc(sizeof(struct party_node));
    struct party_node* curr = *head;
    prev->next = *head;
    *head = prev;
    int swapped = 1;
    while (swapped){
        swapped = 0;
        prev = *head;
        curr = prev->next;
        while (curr != NULL && curr->next != NULL){
            if (curr->price < curr->next->price){
                prev->next = curr->next;
                curr->next = curr->next->next;
                prev->next->next = curr;
                swapped = 1;
            } else {
                curr = curr->next;
            }
            prev = prev->next;
        }
    }
    prev = *head;
    *head = (*head)->next;
    free(prev);
}

//Trim list to fit the budget
double finalize_list(struct party_node **head, double budget){
    struct party_node* prev = (struct party_node*)malloc(sizeof(struct party_node));
    struct party_node* curr = *head;
    prev->next = *head;
    *head = prev;
    while (curr != NULL){
        if (curr->price > budget){
            if (curr->next != NULL){
                prev->next = curr->next;
            } else {
                prev->next = NULL;
            }
            free(curr->item);
            free(curr->ta);
            free(curr);
            curr = prev->next;

        } else {
            budget -= curr->price;
            prev = prev->next;
            curr = curr->next;
        }
    }
    prev = *head;
    *head = (*head)->next;
    free(prev);
    return budget;
}

//Print the current list - hope this is helpful!
void print_list(struct party_node *head){
    int count = 1;
    printf("The current list contains:\n");
    while(head!=NULL){
        printf("Item %d: %s, %.2lf, requested by %s\n",
            count, head->item, head->price, head->ta);
        count++;
        head = head->next;
    }
    printf("\n\n");
    return;
}


/*
LIFO is a decent structure for handling party requests. It allows us to easily access the first element, so it's very easy 
to delete it if we added a request by mistake.
Linked lists are not a good datastructure for this task, as sorting them is very ineffecient.

Another way to sort the data is using merge-sort of quick-sort on an array. This is faster than what we've done 
(O(n^2) time complexity). Another way to trim the list is to create another data structure, and while going from 
most expensive to least expensive item, add items to the structure if it allows us to remain under budget. Then 
delete the original data structure and return the new one. This would require more memory, but perhaps it would have 
better time complexity.
*/