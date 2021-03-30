/*
** hw5_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018-2021
**
** Revision: 2021.04.02.23.55
** 
*/

#include <stdio.h>
#include <math.h>
#include "hw5_lib.h"

/* takes two 3rd degree polynomial and operates with + - or * */
void operate_polynomials(double *a3, double *a2, double *a1, double *a0, double *b3, double *b2, double *b1, double *b0, char op)
{
	double c6, c5, c4, c3, c2, c1, c0; /* temp values for * calculation */
	if (op == '*')
	{
		c6 = (*a3) * (*b3);
		c5 = (*a3) * (*b2) + (*a2) * (*b3);
		c4 = (*a3) * (*b1) + (*a2) * (*b2) + (*a1) * (*b3);
		c3 = (*a3) * (*b0) + (*a2) * (*b1) + (*a1) * (*b2) + (*a0) * (*b3);
		c2 = (*a2) * (*b0) + (*a1) * (*b1) + (*a0) * (*b2);
		c1 = (*a1) * (*b0) + (*a0) * (*b1);
		c0 = (*a0) * (*b0);

		*a2 = c6;
		*a1 = c5;
		*a0 = c4;
		*b3 = c3;
		*b2 = c2;
		*b1 = c1;
		*b0 = c0;
	}
	else if (op == '+')
	{
		*b3 = *a3 + *b3;
		*b2 = *a2 + *b2;
		*b1 = *a1 + *b1;
		*b0 = *a0 + *b0;

		*a3 = 0, *a2 = 0, *a1 = 0, *a0 = 0;
	}
	else if (op == '-')
	{
		*b3 = *a3 - *b3;
		*b2 = *a2 - *b2;
		*b1 = *a1 - *b1;
		*b0 = *a0 - *b0;

		*a3 = 0, *a2 = 0, *a1 = 0, *a0 = 0;
	}
}


/* takes N 4d vectors from user, returns mean coordinates and max adjacent distance */
void four_d_vectors(double *mean_a0, double *mean_a1, double *mean_a2, double *mean_a3, double *longest_distance, int N)
{
	int i;
	double d0, d1, d2, d3;
	double euclidian_distance = 0, max_diff = 0, old_distance = 0;

	for(i = 0; i < N; i++){
		printf("\nPlease enter %d. 4D vector: ", i+1);
		scanf("%lf %lf %lf %lf", &d0, &d1, &d2, &d3);

		/* emergency stop */
		if(d0 == -1 && d1 == -1 && d2 == -1 && d3 == -1) break;

		/* adds values to means */
		*mean_a0 += d0;
		*mean_a1 += d1;
		*mean_a2 += d2;
		*mean_a3 += d3;
		
		/* check is a new record in max_diff */
		distance_between_4d_points(d0, d1, d2, d3, &euclidian_distance);
		if(euclidian_distance - old_distance > max_diff){
			max_diff = euclidian_distance - old_distance;
		}
		old_distance = euclidian_distance;

	}
	/* mean calculation */
	*mean_a0 /= i;
	*mean_a1 /= i;
	*mean_a2 /= i;
	*mean_a3 /= i;
	*longest_distance = max_diff;
}

/* euclidian distance between 4d vector and origin or magnitude of vector */
void distance_between_4d_points(double d0, double d1, double d2, double d3, double *euclidian_distance)
{
	/* assumes (0,0,0,0) as origin */
	*euclidian_distance = sqrt(d0*d0 + d1*d1 + d2*d2 + d3*d3);
}

/* if nMax is largest or equal returns 1 */
int is_max(int nMax, int n1, int n2, int n3, int n4){
	if(nMax >= n1 && nMax >= n2 && nMax >= n3 && nMax >= n4){
		return 1;
	}
	return 0;
}

/* the dhondt method is a highest averages method for allocating seats in election system*/
/* calculates seat number for each party using votes and total seats */
void dhondt_method(int *partyA, int *partyB, int *partyC, int *partyD, int *partyE, int numberOfSeats)
{
	int a = 1, b = 1, c = 1, d = 1, e = 1;
	int voteA = *partyA, voteB = *partyB, voteC = *partyC, voteD = *partyD, voteE = *partyE; /* copy votes for keep them same */
	int vA = voteA, vB = voteB, vC = voteC, vD = voteD, vE = voteE; /* to avoiding faulty max check, when two or more vote equal */

	/* go until all seats out */
	while (numberOfSeats > 0)
	{
		if (is_max(voteA, voteB, voteC, voteD, voteE))
		{
			a++;
			vA = *partyA / a;
			numberOfSeats--;
		}
		if (is_max(voteB, voteA, voteC, voteD, voteE))
		{
			b++;
			vB = *partyB / b;
			numberOfSeats--;
		}
		if (is_max(voteC, voteA, voteB, voteD, voteE))
		{
			c++;
			vC = *partyC / c;
			numberOfSeats--;
		}
		if (is_max(voteD, voteA, voteB, voteC, voteE))
		{
			d++;
			vD = *partyD / d;
			numberOfSeats--;
		}
		if(is_max(voteE, voteA, voteB, voteC, voteD))
		{
			e++;
			vE = *partyE / e;
			numberOfSeats--;
		}
		/* values stored when maximum comparisons end */
		voteA = vA;
		voteB = vB;
		voteC = vC;
		voteD = vD;
		voteE = vE;
	}

	/* return seats */
	*partyA = a-1;
	*partyB = b-1;
	*partyC = c-1;
	*partyD = d-1;
	*partyE = e-1;
}

/* returns region of 2d point on coordinat system as 0 1 2 3 */
int point_region(double x, double y){
	if(x >= 0 && y >= 0) return 0;
	if(x < 0 && y > 0) return 1;
	if(x < 0 && y < 0) return 2;
	if(x > 0 && y < 0) return 3;
}
/* orders point and return index of (x1, y1), check of a is point1 comes before in counter-clockwise */
int compare_2d_points(double x1, double y1, double x2, double y2){
	int r1, r2;
	double h1, h2;
	
	r1 = point_region(x1,y1);
	r2 = point_region(x2, y2); 
	if(r1 == r2){
		h1 = sqrt(x1*x1 + y1*y1);
		h2 = sqrt(x2*x2 + y2*y2);
		/*convert to unit circle coordinat system */
		x1 /= h1;
		y1 /= h1;
		x2 /= h2;
		y2 /= h2;

		/* up of x axis */
		if(r1 == 0 || r1 == 1){
			/* returns 0 if x1 is before */
			return (x1 < x2);
		}
		/* down of x axis */
		else if(r1 == 2 || r1 == 3){
			/* returns 0 if x1 is before */
			return (x1 > x2);
		}
		
	}
	else{
		return (r1 > r2); /* return point that has smallest region */
	}
	return 0;
}
void order_2d_points_cc(double *x1, double *y1, double *x2, double *y2, double *x3, double *y3)
{
	double t1, t2; /* temp */

	/* compare and sort three points */
	if(compare_2d_points(*x1, *y1,*x3, *y3) == 1){
		t1 = *x1;
		t2 = *y1;
		*x1 = *x3;
		*y1 = *y3;
		*x3 = t1;
		*y3 = t2;
	}

	if(compare_2d_points(*x1, *y1,*x2, *y2) == 1){
		t1 = *x1;
		t2 = *y1;
		*x1 = *x2;
		*y1 = *y2;
		*x2 = t1;
		*y2 = t2;
	}

	if(compare_2d_points(*x2, *y2,*x3, *y3) == 1){
		t1 = *x2;
		t2 = *y2;
		*x2 = *x3;
		*y2 = *y3;
		*x3 = t1;
		*y3 = t2;
	}
}


/* encrypts numbers in a special way */
void number_encrypt(unsigned char *number)
{
	char b7 = '-', b6 = '-', b5 = '-', b4 = '-', b3 = '-', b2 = '-', b1 = '-', b0 = '-';
	get_number_components(*number, &b7, &b6, &b5, &b4, &b3, &b2, &b1, &b0);
	reconstruct_components(number, b7, b6, b5, b4, b3, b2, b1, b0);
}
/* convert number to 8 bit binary */
void get_number_components(unsigned char number, char *b7, char *b6, char *b5, char *b4, char *b3, char *b2, char *b1, char *b0)
{
	/* decimal to binary conversion */
	*b0 = number % 2;
	number /= 2;
	*b1 = number % 2;
	number /= 2;
	*b2 = number % 2;
	number /= 2;
	*b3 = number % 2;
	number /= 2;
	*b4 = number % 2;
	number /= 2;
	*b5 = number % 2;
	number /= 2;
	*b6 = number % 2;
	number /= 2;
	*b7 = number % 2;
}

/* encrypt binary number */
void reconstruct_components(unsigned char *number, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0)
{
	char t7, t6, t5, t4, t3, t2, t1, t0;
	t7 = b7, t6 = b6, t5 = b5, t4 = b4, t3 = b3, t2 = b2, t1 = b1, t0 = b0;
	/* encription result of = swap b7-b2, swap b6-b3, swap b5-b0, swap b4-b1 AND then shift bits two times left */
	b7 = t0;
	b6 = t1;
	b5 = t6;
	b4 = t7;
	b3 = t4;
	b2 = t5;
	b1 = t2;
	b0 = t3;

	/* binary to decimal conversion */
	*number = b7*128 + b6*64 + b5*32 + b4*16 + b3*8 + b2*4 + b1*2 + b0;
	
}
