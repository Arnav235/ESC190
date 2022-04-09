#include "lab5.h"

// data structure for the priority queue
typedef struct priority_queue {
    Vnode** arr;
    int arr_size;
    int size;
} priority_queue;

// swaps two nodes in the priority queue
void swap_nodes(priority_queue* queue, int i, int j){
    Vnode* temp = queue->arr[i];
    queue->arr[i] = queue->arr[j];
    queue->arr[j] = temp; 
}

// extracts a node from the priority queue
Vnode* extract_min(priority_queue* queue){
    Vnode* min = queue->arr[0];
    queue->size--;
    if (queue->size == 0){return min;}
    queue->arr[0] = queue->arr[queue->size];
    queue->arr[queue->size] = NULL; // not neccesary but good in case of debugging

    // heapifying down
    int idx = 0;
    while (2*idx +1 < queue->size){
        if (2*idx +2 >= queue->size){
            if (queue->arr[idx]->cost > queue->arr[2*idx +1]->cost){
                swap_nodes(queue, idx, 2*idx +1);
                idx = 2*idx +1;
            } else {break;}
        } else {
            if (queue->arr[2*idx +2]->cost <= queue->arr[2*idx +1]->cost){
                if (queue->arr[idx]->cost > queue->arr[2*idx +2]->cost){
                    swap_nodes(queue, 2*idx +2, idx);
                    idx = 2*idx +2;
                } else {break;}
            } else {
                if (queue->arr[idx]->cost > queue->arr[2*idx +1]->cost){
                    swap_nodes(queue, 2*idx +1, idx);
                    idx = 2*idx +1;
                } else {break;}
            }
        }
    }
    return min;
}

// inserts a node into the priority queue
void insert (priority_queue* queue, Vnode* insert_node){
    if (queue->size == queue->arr_size){
        printf("reallocating queue. Current size:%d\n", queue->arr_size);
        queue->arr = realloc(queue->arr, sizeof(Vnode*) * queue->arr_size * 2);
        queue->arr_size *= 2;
    }
    queue->arr[queue->size] = insert_node;
    int idx = queue->size;
    queue->size++;

    // heapifying up
    Vnode* temp;
    while (idx > 0 && queue->arr[(idx-1) / 2]->cost > queue->arr[idx]->cost){
        swap_nodes(queue, (idx-1)/2, idx);
        idx = (idx-1)/2;
    }
}

// gets the Vnode with a particular station name
Vnode* get_vnode(Graph *gr, char *station){
    for (int i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, station) == 0){
            return gr->adj_list[i];
        }
    }
    return NULL;
}

char **plan_route(Graph *gr, char *start, char *dest){
    // resetting the parameters in all of the vertices
    for (int i = 0; i < gr->count; i++){
        gr->adj_list[i]->prev = NULL;
        gr->adj_list[i]->cost = 5002;
        gr->adj_list[i]->visited = 0;
    }

    priority_queue* queue = (priority_queue*)malloc(sizeof(priority_queue));
    queue->arr = (Vnode**)malloc(sizeof(Vnode*));
    queue->size = 1;
    queue->arr_size = 1;

    // getting the first Vnode
    queue->arr[0] = get_vnode(gr, start); 
    queue->arr[0]->cost = 0;

    Vnode* curr_node;
    Vnode* temp;
    Enode* edge;
    int queue_idx;
    while (queue->size > 0){
        curr_node = extract_min(queue);
        if (strcmp(curr_node->station, dest) == 0){break;}; // we have found the current station
        curr_node->visited = 1;
        edge = curr_node->edges;
        while (edge != NULL){ // going through all of the edge of the current node
            temp = get_vnode(gr, edge->vertex);
            if (!temp->visited && temp->cost > curr_node->cost + edge->weight){
                // updating the parameters of the node we're at
                temp->cost = curr_node->cost + edge->weight;
                temp->prev = curr_node;
                // finding the node in the priority queue
                queue_idx = 0;
                while (queue_idx < queue->size){
                    if (strcmp(queue->arr[queue_idx]->station, temp->station) == 0){break;}
                    queue_idx++;
                }
                // if we didn't find the node in the queue
                if (queue_idx == queue->size){
                    insert(queue, temp);
                } else {
                    while (queue_idx > 0 && queue->arr[(queue_idx-1)/2]->cost > queue->arr[queue_idx]->cost){
                        swap_nodes(queue, (queue_idx-1)/2, queue_idx);
                        queue_idx = (queue_idx-1)/2;
                    }
                }
            }
            edge = edge->next;
        }
        if (queue->size==0){
            free(queue->arr);
            free(queue);
            return NULL;
        }
    }

    char** char_arr = (char**)malloc(sizeof(char*));
    int cur_size = 0;
    while (curr_node != NULL){
        cur_size++;
        char_arr = realloc(char_arr, sizeof(char*) * cur_size);
        char_arr[cur_size-1] = (char*)malloc(sizeof(char) * (strlen(curr_node->station)+1));
        strcpy(char_arr[cur_size-1], curr_node->station);
        curr_node = curr_node->prev;
    }
    free(queue->arr);
    free(queue);
    return char_arr;
}

void add(Graph *gr, char *station){
    // if the adjacency list has not been initialized, we malloc it
    if (gr->adj_list == NULL){
        gr->adj_list = (Vnode**)malloc(sizeof(Vnode*));
    } else{
        // checking if the vertex is already in the graph
        for (int i = 0; i < gr->count; i++){
            if (strcmp(gr->adj_list[i]->station, station) == 0){return;}
        }
        // if the vertex is not already in the graph, then we allocate more memory
        gr->adj_list = realloc(gr->adj_list, sizeof(Vnode*) * (gr->count+1));
    }
    gr->adj_list[gr->count] = (Vnode*)malloc(sizeof(Vnode));
    strcpy(gr->adj_list[gr->count]->station, station);
    gr->adj_list[gr->count]->edges = NULL;
    
    gr->count++;
}

void update(Graph *gr, char *start, char *dest, int weight){
    add(gr, start);
    add(gr, dest);
    for (int i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, start) == 0){ // we have found the starting station

            // if the current vertex doesn't have any edges
            if (gr->adj_list[i]->edges == NULL){
                if (weight != 0){
                    gr->adj_list[i]->edges = (Enode*)malloc(sizeof(Enode));
                    gr->adj_list[i]->edges->next = NULL;
                    gr->adj_list[i]->edges->weight = weight;
                    strcpy(gr->adj_list[i]->edges->vertex, dest);
                }
                return;
            }
            Enode* cur_edge = gr->adj_list[i]->edges;

            // if the edge that we're looking for is the first edge
            if (strcmp(cur_edge->vertex, dest) == 0){
                if (weight == 0){
                    gr->adj_list[i]->edges = cur_edge->next;
                    free(cur_edge);
                } else {
                    gr->adj_list[i]->edges->weight = weight;
                }
                return;
            }

            // if the current destination is in the list, then cur_edge->next is that node
            // Othersie cur_edge->next = NULL
            while (cur_edge->next != NULL){
                if (strcmp(cur_edge->next->vertex, dest) == 0){
                    break;
                }
                cur_edge = cur_edge->next;
            }
            // if dest is not in the list, then we allocate more memory
            if (cur_edge->next == NULL){
                if (weight == 0){return;}
                cur_edge->next = (Enode*)malloc(sizeof(Enode));
                cur_edge->next->next = NULL;
                strcpy(cur_edge->next->vertex, dest);
                cur_edge->next->weight = weight;
                return;
            }
            // if dest is in the list and weight = 0
            if (weight == 0){
                Enode* temp = cur_edge->next;
                cur_edge->next = cur_edge->next->next;
                free(temp);
                return;
            }
            // if dest is in the list and weight != 0
            cur_edge->next->weight = weight;
            return;
        }
    }
}

void disrupt(Graph *gr, char *station){
    // loops through to find the station in the graph
    for (int i = 0; i < gr->count; i++){
        if (strcmp(gr->adj_list[i]->station, station) == 0){
            for (int j = 0; j < gr->count; j++){
                // removing the edges between all of the vertices and the station we want to remove
                update(gr, gr->adj_list[j]->station, gr->adj_list[i]->station, 0);
            }
            // remove all of the edge nodes associated with the vertex
            Enode* temp_enode = gr->adj_list[i]->edges;
            while (gr->adj_list[i]->edges != NULL){
                gr->adj_list[i]->edges = gr->adj_list[i]->edges->next;
                free(temp_enode);
                temp_enode = gr->adj_list[i]->edges;
            }
            // removing the vertex itself
            Vnode* removed_vertex = gr->adj_list[i];
            for (int j = i+1; j < gr->count; j++){
                gr->adj_list[j-1] = gr->adj_list[j];
            }
            free(removed_vertex);
            gr->count--;
            if (gr->count == 0){
                free(gr->adj_list);
                gr->adj_list = NULL;
            } else {
                gr->adj_list = realloc(gr->adj_list, gr->count * sizeof(Vnode*));
            }
        }
    }
}