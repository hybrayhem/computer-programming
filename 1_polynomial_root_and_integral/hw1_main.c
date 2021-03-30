/*
** main.c:
**
** The test/driver program for the homework.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.03.03.20.16
** 
*/


#include <stdio.h>


#include "hw1_lib.h"
#include "hw1_io.h"



/*
** Testing root finding..
**
*/

/*
1.23x^4+5.6x^2+x-1 = 0

-3.2x^4+1.23x^3+5.6x^2+1 = 0

1.23x^4-x^3+5.6x^2+1 = 0

5.6x^4-5.72x^3-x^2+x = 0

-5.72x^4+x^3+1.23x-3.2 = 0

-3.2x^4+x^3-5.72x^2+5.6 = 0

-x^4+5.6x^3+1.23x^2-3.2x+1 = 0

1.23x^3-5.72x^2-x-3.2 = 0

1.23x^4-3.2x^3+x^2-x = 0

1.23x^4-5.72x^3-3.2x^2+5.6 = 0
*/
void test_write_polynomial3(){
	int i;
	for(i = 0; i < 10; i++)
	{
		float a1, a2, a3, a4, a5;
		scanf("%f %f %f %f %f", &a1, &a2, &a3, &a4, &a5);
		write_polynomial3(a2, a3, a4, a5);
		printf("\n");
	}
}

void test_write_polynomial4(){
	int i;
	for(i = 0; i < 10; i++)
	{
		float a1, a2, a3, a4, a5;
		scanf("%f %f %f %f %f", &a1, &a2, &a3, &a4, &a5);
		write_polynomial4(a1, a2, a3, a4, a5);
		printf("\n");
	}
}

void test_integral3(){
	printf("integral "); write_polynomial3(1.2,0,0.8,1); printf(" from -2 to 13");
	printf(" = %.2f\n", integral3(1.2,0,0.8,1, -2,13, 0.001));
	printf("should be 8644.5\n\n");

	printf("integral "); write_polynomial3(-3.2,1.2,0.6,1); printf(" from 3 to 19");
	printf(" = %.2f\n", integral3(-3.2,1.2,0.6,1, 3,19, 0.001));
	printf("should be -101338\n\n");

	printf("integral "); write_polynomial3(1.2,-1,4.1,0); printf(" from 81 to 90");
	printf(" = %.2f\n", integral3(1.2,-1,4.1,0, 81,90, 0.001));
	printf("should be 6706290\n\n");
}

void test_integral4(){
	printf("integral "); write_polynomial4(1.2,0,0.8,1,2.4); printf(" from -2 to 13");
	printf(" = %.2f\n", integral4(1.2,0,0.8,1,2.4, -2, 13, 0.0001));
	printf("should be 89824.5\n\n");

	printf("integral "); write_polynomial4(-3.2,1.2,0.6,1,0); printf(" from 3 to 19");
	printf(" = %.2f\n", integral4(-3.2,1.2,0.6,1,0, 3, 19, 0.0001));
	printf("should be -1543930\n\n");

	printf("integral "); write_polynomial4(1.2,-1,4.1,0,-1); printf(" from -10 to -4");
	printf(" = %.2f\n", integral4(1.2,-1,4.1,0,-1, -10, -4, 0.0001));
	printf("should be 27463.4\n\n");
}

void test_root_finder3() 
{
	double root_known = 0.34;
	double root_found;
	write_polynomial3(0, 5.6, 1, -1);
	root_found = root3(0, 5.6, 1, -1, root_known-0.1, root_known+0.1);
	printf("\nGround truth root: %f vs found root: %f\n\n", root_known, root_found);
}

void test_root_finder4() 
{
	double root_known = 1.34522;
	double root_found;
	write_polynomial4(-3.20, 0, 5.6, 1, -1);
	root_found = root4(-3.20, 0, 5.6, 1, -1, root_known-0.1, root_known+0.1);
	printf("\nGround truth root: %f vs found root: %f\n\n", root_known, root_found);

	root_known = 0.875906;
	write_polynomial4(-3.2, 1, -5.7227, 0, 5.6);
	root_found = root4(-3.2, 1, -5.7227, 0, 5.6, 0.5, 1.0);
	printf("\nGround truth root: %f vs found root: %f\n\n", root_known, root_found);
}
/*
** main function for testing the functions...
**
*/
int main(void) {
	/* test_write_polynomial3();
	test_write_polynomial4(); */
	printf("\n### TEST INTEGRAL 3 ###\n");
	test_integral3(); 
	printf("\n### TEST INTEGRAL 4 ###\n");
	test_integral4();  
	printf("\n### TEST ROOT FINDER 3 ###\n");
	test_root_finder3(); 
	printf("\n### TEST ROOT FINDER 4 ###\n");
	test_root_finder4(); 
	return (0);
} /* end main */
