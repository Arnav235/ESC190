#include <stdio.h> //need this to use the printf () function
#include "lab1.h" //need this to link our Lab 1 functions

int main() {
	printf("Test 2: The split will be: $%.2f\n", split_bill(60, 0.13, 0.15, 2));
	printf("Test 3: The split will be: $%.2f\n", split_bill(80, 0.10, 0.20, 4));
	printf("Test 4: The split will be: $%.2f\n", split_bill(80, 0.13, 0.15, 2));

	printf("Test 2: The adjusted price is: $%.2f.\n", adjust_price (100.00));
	printf("Test 3: The adjusted price is: $%.2f.\n", adjust_price (25.00));
	printf("Test 4: The adjusted price is: $%.2f.\n", adjust_price (40.00));

	printf("Fish Soup: %d.\n", sandy_eats("Fish Soup"));
	printf("Fries: %d.\n", sandy_eats("Fries"));
	printf("Jalapeno: %d.\n", sandy_eats("Jalapeno"));
	printf("Tomatoes: %d.\n", sandy_eats("Tomatoes"));

	char test_p4 [30] = "zebra";
    char test2 [30] = "happy";
    char test3 [30] = "clown";
    char test4 [30] = "butterfly";
    imagine_fish(test_p4);
    printf("Just imagine, a %s!\n", test_p4 );
    // Just imagine , a zebrafish!

    imagine_fish(test2);
    imagine_fish(test3);
    imagine_fish(test4);

    printf("Just imagine, a %s!\n", test2);
    printf("Just imagine, a %s!\n", test3);
    printf("Just imagine, a %s!\n", test4);

	return 0;
}