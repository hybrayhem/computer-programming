/*
** hw1_io.c:
**
** The source file implementing output functions.
**
** Author: Halil Ibrahim Ilhan. (c) 2021
**
** Revision: 2021.03.14.18.30
** 
*/

#include <stdio.h>
#include "hw1_io.h"


void write_polynomial3(double a0, double a1, double a2, double a3)
{
    int i, j;
    double coefficients[4];
    char items[4][9] = {0}; /* items building in this with "signcoefficientx^degree" format*/
    char result[31] = {0};  /* the result that will be printed

    /*couldn't use the short declaration due to "expression must be a modifiable lvalue" error: coefficients = {a0, a1, a2, a3} */
    coefficients[0] = a0;
    coefficients[1] = a1;
    coefficients[2] = a2;
    coefficients[3] = a3;

    for (i = 0; i < 4; i++)
    {
        /* add sign and coefficient */
        if (coefficients[i] == 0.0)
        {
            /* if coefficient is zero, i. item stays empty and lop continues to building next item*/
            continue;
        }
        else if (coefficients[i] == 1.0)
        {
            /* if coefficient is one, adds just plus as leading */
            sprintf(items[i], "+");
        }
        else if (coefficients[i] == -1.0)
        {
            /* if coefficient minus one, adds just minus as leading */
            sprintf(items[i], "-");
        }
        else
        {
            /* if coefficient is another number, adds number up to three fraction with sign, trims if there is trailing zeros as fraction*/
            sprintf(items[i], "%+.3g", coefficients[i]);
        }

        /* add x and degree if exists */
        if (3 - i > 1)
        {
            /* adds x^degree to item */
            sprintf(items[i], "%sx^%d", items[i], 3 - i);
        }
        else if (3 - i == 1)
        {
            /* adds 'x' to item */
            sprintf(items[i], "%sx", items[i]);
        }
        else if (3 - i == 0 && (coefficients[i] == 1.0 || coefficients[i] == -1.0))
        {
            /* adds '1' to item if at end */
            sprintf(items[i], "%s1", items[i]);
        }

        /* add built item to result */ 
        sprintf(result, "%s%s", result, items[i]);
    }

    /* remove leading plus sign, if exists */
    if (result[0] == '+')
    {
        j = 0;
        /* shift every character to left once */ 
        while (result[j + 1] != '\0')
        {
            result[j] = result[j + 1];
            j++;
        }
        result[j] = '\0';
    }
    printf("%s", result);
}

void write_polynomial4(double a0, double a1, double a2, double a3, double a4)
{
    int i, j;
    double coefficients[5];
    char items[5][9] = {0}; /* items building in this with "signcoefficientx^degree" format*/
    char result[40] = {0};  /* the result that will be printed

    /*couldn't use this short declaration due to "expression must be a modifiable lvalue" error: coefficients = {a0, a1, a2, a3, a4} */
    coefficients[0] = a0;
    coefficients[1] = a1;
    coefficients[2] = a2;
    coefficients[3] = a3;
    coefficients[4] = a4;
    
    for (i = 0; i < 5; i++)
    {
        /* add sign and coefficient */
        if (coefficients[i] == 0.0)
        {
            /* if coefficient is zero, i. item stays empty and lop continues to building next item*/
            continue;
        }
        else if (coefficients[i] == 1.0)
        {
            /* if coefficient is one, adds just plus as leading */
            sprintf(items[i], "+");
        }
        else if (coefficients[i] == -1.0)
        {
            /* if coefficient minus one, adds just minus as leading */
            sprintf(items[i], "-");
        }
        else
        {
            /* if coefficient is another number, adds number up to three fraction with sign, trims if there is trailing zeros as fraction*/
            sprintf(items[i], "%+.3g", coefficients[i]);
        }

        /* add x and degree if exists */
        if (4 - i > 1)
        {
            /* adds x^degree to item */
            sprintf(items[i], "%sx^%d", items[i],4 - i);
        }
        else if (4 - i == 1)
        {
            /* adds 'x' to item */
            sprintf(items[i], "%sx", items[i]);
        }
        else if (4 - i == 0 && (coefficients[i] == 1.0 || coefficients[i] == -1.0))
        {
            /* adds '1' to item if at end */
            sprintf(items[i], "%s1", items[i]);
        }

        /* add built item to result */ 
        sprintf(result, "%s%s", result, items[i]);
    }

    /* remove leading plus sign, if exists */
    if (result[0] == '+')
    {
        j = 0;
        /* shift every character to left once */ 
        while (result[j + 1] != '\0')
        {
            result[j] = result[j + 1];
            j++;
        }
        result[j] = '\0';
    }
    printf("%s", result);
}
