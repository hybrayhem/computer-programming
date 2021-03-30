/*
** hw2_lib.c:
**
** The source file implementing library functions.
**
*/

#include <stdio.h>
#include <math.h>
#include "hw2_lib.h"

int is_leap_year(int year){
    if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0 )){
        return 1;
    }
    return 0;
}

int is_date_valid(int day, int month, int year){
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_leap_year(year)) months[1] = 29;

    if(day < 1 || month < 1 || month > 12 || day > months[month - 1] || year < 1){
        return 0;
    }
    return 1;
}

int find_weekday_of_data(int day, int month, int year)
{
    int weekday, k, j;

    if(!is_date_valid(day, month, year)){
        printf("WARNING! Date is not valid: %d/%d/%d\n", day, month, year);
        return -1;
    }

    if(month<3){
        month += 12;    /* in this algorithm year starts from march as 3rd month and ends with february as 14th month */
        year--;
    }


    weekday = (day + (13*(month +1)/5) + year + year/4 - year/100 + year/400) % 7;
    return ((weekday + 5) % 7) + 1;
}

/* 14/05/2018   23/03/2021  */
int count_day_between_dates(int start_day, int start_month, int start_year, int end_day, int end_month, int end_year)
{
    int i, first_year_count = 365, mid_years_count = 0, last_year_count = 0;
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


    if(!is_date_valid(start_day, start_month, start_year)){
        printf("WARNING! Start date is not valid: %d/%d/%d\n", start_day, start_month, start_year);
        return -1;
    }
    if(!is_date_valid(end_day, end_month, end_year)){
        printf("WARNING! End date is not valid: %d/%d/%d\n", end_day, end_month, end_year);
        return -1;
    }

    if(is_leap_year(start_year)){
        first_year_count = 366;
        months[1] = 29;
        /*printf("start_leap\n");*/
    }
    for(i = 0; i < start_month - 1; i++){
        first_year_count -= months[i];
        /*printf("substract %d\n", months[i]);*/
    }
    months[1] = 28;
    first_year_count -= start_day;
    /*printf("first_year_count %d\n", first_year_count);*/


    for(i = start_year + 1; i < end_year; i++){
        if(is_leap_year(i)){
            mid_years_count += 366;
        }else{
            mid_years_count += 365;
        }
    }
    /*printf("mid_years_count %d\n", mid_years_count);*/


    if(is_leap_year(end_year)){
        months[1] = 29;
        /*printf("end_leap\n");*/
    }
    for(i = 0; i < end_month - 1; i++){
        last_year_count += months[i];
        /*printf("adding %d\n", months[i]);*/
    }
    last_year_count += end_day - 1;
    /*printf("last_year_count %d\n", last_year_count);*/

    if(start_year == end_year){
        if (months[1] == 29)
        {
            return first_year_count + last_year_count - 366 + 1;
        }
        return first_year_count + last_year_count - 365 + 1;
    }
    return first_year_count + mid_years_count + last_year_count + 1;
}

double find_angle(double a, double b, double c)
{ 
    double x, y, cosa, alpha;
	
	x = sqrt((a*a + c*c - (b*b/2))/18);
	/*printf("x: %f\n", x);*/
	y = sqrt((a*a + b*b - (c*c/2))/18);
	/*printf("y: %f\n", y);*/
	
	cosa = (4*x*x + 4*y*y - a*a) / (8*x*y);
	/*printf("cosa: %f\n", cosa);*/
	
	alpha = acos(cosa);
	/*printf("alpha: %f\n", alpha);*/
	return alpha;
}

void print_padded(char item[], int length, int alignment, char border){ /* alignment =   -1 : left   0 : center    1 : right */
    int i, padleft, padright;

    int size = 0;
    int dot = -1;


    while (item[size] != '\0'){
        if(dot < 0 && item[size] == '.'){
            dot = size;
        }
        size++;
    }

    if(dot > 0){
	size = dot + 3;
	if(item[dot+1] == '0' && item[dot+2] == '0'){
		size = dot;
	}
	else if(item[dot+2] == '0'){
		size = dot+2;
	}
	item[size] = '\0';
    }

    length -= size;
    padleft = length/2;
    padright = length - padleft;

    printf("%c ", border);

    if(alignment == -1) printf("%s", item);
    for(i = 0; i < padleft; i++){
        printf(" ");
    }
    if(alignment == 0) printf("%s", item);
    for(i = 0; i < padright; i++){
        printf(" ");
    }
    if(alignment == 1) printf("%s", item);

    printf(" %c", border);
}

void print_tabulated(unsigned int r11, double r12, int r13, 
                     unsigned int r21, double r22, int r23, 
                     unsigned int r31, double r32, int r33, char border)
{

    char item[20];
    char line[56];
    /* column widths */
    int cw1 = 13;    
    int cw2 = 15;
    int cw3 = 16;

    if(border == '*'){
        sprintf(line, "******************************************************");
    }else if(border == '-'){
        sprintf(line, "------------------------------------------------------");
    }


    printf("┌%s┐\n", line);
    printf("|    Row 101    | ");
    printf("   Row ABCDEFG    | ");
    printf("   Row XYZ123    |");

    printf("\n|%s|\n", line);

    sprintf(item, "%u", r11);
    print_padded(item, cw1, 0, '|');

    sprintf(item, "%f", r12);
    print_padded(item, cw2, -1, ' ');

    sprintf(item, "%+d", r13);
    print_padded(item, cw3, -1, '|');

    printf("\n|%s|\n", line);
    sprintf(item, "%u", r21);
    print_padded(item, cw1, 0, '|');

    sprintf(item, "%f", r22);
    print_padded(item, cw2, -1, ' ');

    sprintf(item, "%+d", r23);
    print_padded(item, cw3, -1, '|');

    printf("\n|%s|\n", line);
    sprintf(item, "%u", r31);
    print_padded(item, cw1, 0, '|');

    sprintf(item, "%f", r32);
    print_padded(item, cw2, -1, ' ');

    sprintf(item, "%+d", r33);
    print_padded(item, cw3, -1, '|');

    printf("\n└%s┘\n", line);
}



/************************************\ 
| Row 101 | Row ABCDEFG | Row XYZ123 |
 ************************************ 
|    1    | 1.1         | -1         | 
 ************************************ 
|    2    | 2           | 2          | 
 ************************************ 
|    3    | 3.1         | -3         | 
\************************************/ 
