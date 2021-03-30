/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.02.23.55
** 
*/

#include <stdio.h>
#include "hw5_lib.h"

/* for getting input in test_operate_polynomials */
void read_polynomial(double *c3, double *c2, double *c1, double *c0){
	int i, t1;
	double t2;

	for(i = 0; i < 4; i++){
		if(i != 3){
			scanf("(%d, %lf), ", &t1, &t2);
		}
		else{
			scanf("(%d, %lf)", &t1, &t2);
		}

		if(t1 == 0){
			*c0 = t2;
		}
		else if(t1 == 1){
			*c1 = t2;
		}
		else if(t1 == 2){
			*c2 = t2;
		}
		else if(t1 == 3){
			*c3 = t2;
		}
		else{
			printf("Invalid degree: %d, assuming as coefficient 0\n", t1);
			*c0 = t2;
		}
	}
	while (getchar() != '\n'); /* sweep unnecessary inputs */
}
void test_operate_polynomials()
{
	int i;
	int t1;
	double t2;
	double a3 = 0, a2 = 0, a1 = 0, a0 = 0, b3 = 0, b2 = 0, b1 = 0, b0 = 0;
	char op = '+';

	printf("\n# Polynomial input format = (3, a), (2, b), (1, c), (0, d) #\n\n");
	printf("Enter first polynomial: ");
	read_polynomial(&a3, &a2, &a1, &a0);

	printf("Enter second polynomial: ");
	read_polynomial(&b3, &b2, &b1, &b0);

	printf("\nEnter operand (-, +, *): ");
	scanf("%c", &op);

	operate_polynomials(&a3, &a2, &a1, &a0, &b3, &b2, &b1, &b0, op);
	printf("%.2f x^6 + %.2f x^5 + %.2f x^4 + %.2f x^3 + %.2f x^2 + %.2f x + %.2f\n", a2, a1, a0, b3, b2, b1, b0);
}

void test_four_d_vectors()
{
	double mean_a0 = 0.0, mean_a1 = 0.0, mean_a2 = 0.0, mean_a3 = 0.0, longest_distance = 0.0;
	int N = 5;
	four_d_vectors(&mean_a0, &mean_a1, &mean_a2, &mean_a3, &longest_distance, N);
	printf("Mean a0: %f\nMean a1: %f\nMean a2: %f\nMean a3: %f\nThe longest distance between two points: %f\n\n\n", mean_a0, mean_a1, mean_a2, mean_a3, longest_distance);
}

void test_dhondt_method()
{
	int partyA = 100000, partyB = 80000, partyC = 30000, partyD = 0, partyE = 0, numberOfSeats = 8;
	/* test_2: partyA = 400, partyB = 300, partyC = 180, partyD = 80, partyE = 45, numberOfSeats = 250;*/
	/* test_3: partyA = 100000, partyB = 80000, partyC = 30000, partyD = 20000, partyE = 10000, numberOfSeats = 550;*/
	dhondt_method(&partyA, &partyB, &partyC, &partyD, &partyE, numberOfSeats);
	printf("Party A: %d seat(s).\nParty B: %d seat(s).\nParty C: %d seat(s).\nParty D: %d seat(s).\nParty E: %d seat(s).\n\n\n", partyA, partyB, partyC, partyD, partyE);
}

void test_order_2d_points_cc()
{
	double x1 = -1.0, y1 = -1.0, x2 = -1.0, y2 = 1.0, x3 = 1.0, y3 = 1.0;
	/* test_2: x1 = -2.0, y1 = -3.0, x2 = 1.0, y2 = -2.0, x3 = 2.0, y3 = -1.0; */
	order_2d_points_cc(&x1, &y1, &x2, &y2, &x3, &y3);
	printf("Counter-Clockwise Order: (%f,%f) - (%f,%f) - (%f,%f)\n\n\n", x1, y1, x2, y2, x3, y3);
}

void test_number_encrypt()
{
	unsigned char number = 125;
	number_encrypt(&number);
	printf("Encrypted number: %d\n\n\n", number);
}

/*
** main function for testing the functions...
**
*/
int main(void)
{
	test_operate_polynomials();
	test_four_d_vectors();
	test_dhondt_method();
	test_order_2d_points_cc();
	test_number_encrypt();
	return (0);
} /* end main */
