#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "lab1.h"

double split_bill(double base_amount , double tax_rate , double tip_rate , int num_people) {
	double bill = base_amount * (1+tax_rate) * (1+tip_rate) / num_people;
	if ((int)(bill*100) != bill*100) {
		bill = ((int)(bill*100) + 1.0) / 100;
	}
	return bill;
}

double adjust_price(double original_price) {
	return pow(original_price, 0.5) * 10;
}

int sandy_eats(char menu_item []){
	if (strlen(menu_item) % 2 != 0) {return 0;} // checking if the string is odd or even
	char *ret = strstr(menu_item, "fish");
	if (ret != NULL) {return 0;}
	ret = strstr(menu_item, "Fish");
	if (ret != NULL) {return 0;}

	char element;
	for (int i = 0; i < strlen(menu_item); i++){
		element = tolower(menu_item[i]);
		if (element == 'j' || element == 'k' || element == 'l'){
			return 0;
		}
	}
	return 1;
}

void imagine_fish(char thing []){
	int len = strlen(thing);
	char fi[] = "fish";
	for (int i = 0; i < 4; i++){
		thing[len + i] = fi[i];
	}
	thing[len + 4] = '\0';
}