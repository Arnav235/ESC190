#include "a1.h"

/**
	Add your functions to this file.
	Make sure to review a1.h.
	Do NOT include a main() function in this file
	when you submit.
*/

Menu* load_menu(char* fname){
	Menu* menu = (Menu *)malloc(sizeof(Menu));
	int num_items = 0;
	char** item_codes = (char**)malloc(sizeof(char*));
	char** item_names = (char**)malloc(sizeof(char*));	
	double* item_cost_per_unit = (double*)malloc(sizeof(double));
	int arr_size = 1;

	char* item_code_buff; 
	char* item_name_buff;
	char temp[ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH +2];
	char* tok;
	double d;

	FILE* file_ptr = fopen(fname, "r");
	if (file_ptr != NULL){
		while (1){
			if (fscanf(file_ptr," %[^$\n]", temp) != 1){
				break;
			}
			item_code_buff = (char*)malloc(sizeof(char) * (ITEM_CODE_LENGTH)); 
			item_name_buff = (char*)malloc(sizeof(char) * (MAX_ITEM_NAME_LENGTH));
			tok = strtok(temp, MENU_DELIM);
			strncpy(item_code_buff, tok, ITEM_CODE_LENGTH);
			tok = strtok(NULL, MENU_DELIM);
			strncpy(item_name_buff, tok, strlen(tok)+1);
			fscanf(file_ptr,"$%lf\n", &d);
			
			// adding everything to arrays
			item_codes[num_items] = item_code_buff;
			item_names[num_items] = item_name_buff;
			item_cost_per_unit[num_items] = d;
			num_items++;

			// changing size of array if neccesary
			if (num_items == arr_size){
				item_codes = (char**)realloc(item_codes, sizeof(char*)* arr_size*2);
				item_names = (char**)realloc(item_names, sizeof(char*)* arr_size*2);
				item_cost_per_unit = (double*)realloc(item_cost_per_unit, sizeof(double)* arr_size*2);
				arr_size *= 2;
			}
		}
	}
	fclose(file_ptr);
	menu->num_items = num_items;
	menu->item_codes = item_codes;
	menu->item_names = item_names;
	menu->item_cost_per_unit = item_cost_per_unit;
	return menu;
}

void clear_menu(Menu**menu){
	free((*menu)->item_cost_per_unit);
	for (int i = 0; i < (*menu)->num_items; i++){
		free((*menu)->item_codes[i]);
		free((*menu)->item_names[i]);
	}
	free((*menu)->item_codes);
	free((*menu)->item_names);
	free(*menu);

	*menu=NULL;
}

Restaurant* initialize_restaurant(char* name){
	Restaurant* restaurant = (Restaurant*)malloc(sizeof(Restaurant));

	restaurant->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	strncpy(restaurant->name, name, strlen(name)+1);

	restaurant->menu = load_menu(MENU_FNAME);
	restaurant->num_completed_orders = 0;
	restaurant->num_pending_orders = 0;
	restaurant->pending_orders = (Queue*)malloc(sizeof(Queue));
	restaurant->pending_orders->tail = NULL;
	restaurant->pending_orders->head = NULL;
	return restaurant;
}

void close_restaurant(Restaurant** restaurant){
	free((*restaurant)->name);
	clear_menu(&((*restaurant)->menu));

	QueueNode* head = (*restaurant)->pending_orders->head;
	QueueNode* temp;
	while (head != NULL){
		temp = head;
		clear_order(&(temp->order));
		free(temp);
		head = head->next;
	}
	free((*restaurant)->pending_orders);
	free(*restaurant);
	*restaurant = NULL;
}

Order* build_order(char* items, char* quantities){
	Order* order = (Order*)malloc(sizeof(Order));
	order->num_items = strlen(items)/(ITEM_CODE_LENGTH-1);
	order->item_codes = (char**)malloc(sizeof(char*) * order->num_items);
	order->item_quantities = (int*)malloc(sizeof(int) * order->num_items);
	
	char quantities_arr[strlen(quantities) +1];
	strcpy(quantities_arr, quantities);
	char* tok = strtok(quantities_arr, MENU_DELIM);
	int code_idx = 0;
	for (int i = 0; i < order->num_items; i++){
		order->item_quantities[i] = strtol(tok, NULL, 10);
		order->item_codes[i] = (char*)malloc(sizeof(char) * ITEM_CODE_LENGTH);
		strncpy(order->item_codes[i], &items[code_idx], ITEM_CODE_LENGTH-1);
		order->item_codes[i][ITEM_CODE_LENGTH-1] = '\0';
		tok = strtok(NULL, MENU_DELIM);
		code_idx += ITEM_CODE_LENGTH-1;
	}
	return order;
}

void clear_order(Order** order){
	for (int i = 0; i < (*order)->num_items; i++){
		free((*order)->item_codes[i]);
	}
	free((*order)->item_codes);
	free((*order)->item_quantities);
	free(*order);
	*order = NULL;
}

// We add orders to the tail of the linked list and remove them from the head;
void enqueue_order(Order* order, Restaurant* restaurant){
	QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
	new_node->order = order;
	new_node->next = NULL;
	if (restaurant->pending_orders->tail == NULL){
		restaurant->pending_orders->tail = new_node;
		restaurant->pending_orders->head = new_node;
	} else {
		restaurant->pending_orders->tail->next = new_node;
		restaurant->pending_orders->tail = new_node;
	}
	restaurant->num_pending_orders += 1;
}

Order* dequeue_order(Restaurant* restaurant){
	Order* dequed_order = restaurant->pending_orders->head;
	if (restaurant->pending_orders->head == restaurant->pending_orders->tail){
		restaurant->pending_orders->head = NULL;
		restaurant->pending_orders->tail = NULL;
	} else {
		restaurant->pending_orders->head = restaurant->pending_orders->head->next;
	}
	restaurant->num_pending_orders -= 1;
	restaurant->num_completed_orders += 1;
	return dequed_order;
}

void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}


void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %d%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}