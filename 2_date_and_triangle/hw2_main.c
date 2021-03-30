/*
** main.c:
**
** The test/driver program for the homework.
**
*/


#include <stdio.h>
#include "hw2_lib.h"


void test_find_weekday_of_data() 
{
	int date_day = 14, date_month=3, date_year=2021, dayno; /*7*/
	printf("\n--------------------------------------------------------------------------\n");
	printf("| --> Testing test_find_weekday_of_data...                               |\n");
	printf("--------------------------------------------------------------------------\n");
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 13, date_month=8, date_year=2009; /*4*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 29, date_month=2, date_year=1980; /*5*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 28, date_month=2, date_year=1970; /*6*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 1, date_month=1, date_year=1; /*1*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 29, date_month=2, date_year=2021; /*-1*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = -1, date_month=12, date_year=1735; /*-1*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);

	date_day = 15, date_month=16, date_year=1735; /*-1*/
	dayno = find_weekday_of_data(date_day, date_month, date_year);
	printf("%02d-%02d-%04d is the %dth day of the week!\n\n",date_day, date_month, date_year, dayno);
}


void test_count_day_between_dates() 
{
	int start_day = 14, start_month=3, start_year=2021;
	int end_day = 23, end_month=3, end_year=2021;
	int numdays;
	printf("\n--------------------------------------------------------------------------\n");
	printf("| --> Testing test_count_day_between_dates...                            |\n");
	printf("--------------------------------------------------------------------------\n");
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);

	start_day = 14, start_month=5, start_year=2018;
	end_day = 23, end_month=3, end_year=2021;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);

	start_day = 14, start_month=5, start_year=2021;
	end_day = 23, end_month=3, end_year=2022;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);

	start_day = 30, start_month=9, start_year=1453;
	end_day = 1, end_month=12, end_year=2077;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);

	start_day = 1, start_month=12, start_year=2077;
	end_day = 1, end_month=12, end_year=2077;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);
	
	start_day = 30, start_month=9, start_year=1701;
	end_day = 1, end_month=12, end_year=2077;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);

	start_day = -6, start_month=9, start_year=1701;
	end_day = 1, end_month=19, end_year=2077;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);
	printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
	printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);
}


void test_find_angle() 
{
	double a = 1.2, b = 3.1, c = 2.0, alpha;
	printf("\n--------------------------------------------------------------------------\n");
	printf("| --> Testing test_find_angle...                                         |\n");
	printf("--------------------------------------------------------------------------\n");
	alpha = find_angle(a, b, c);
	printf("The angle for a=%f, b=%f and c=%f is %f.\n",a, b, c, alpha);
}


void test_print_tabulated() {
	printf("\n--------------------------------------------------------------------------\n");
	printf("| --> Testing test_print_tabulated...                                    |\n");
	printf("--------------------------------------------------------------------------\n\n");
	print_tabulated(1, 1.1, -1, 2, 2.0, 2, 3, 3.1, -3, '*');
	printf("\n\n");
	print_tabulated(12345, 12.30, +987654, 24680, 24.68, 864202, 13579, 13.58, 975313, '-');
	print_tabulated(12345, 12.35600, +987654, 24680, 24.6008, 864202, 13579, 13.999, 975313, '-');
}


/*
** main function for testing the functions...
**
*/
int main(void) {
	test_find_weekday_of_data();
	test_count_day_between_dates();
	test_find_angle();
	test_print_tabulated();
	return (0);
} /* end main */
