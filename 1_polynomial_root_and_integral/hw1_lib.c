/*
** hw4_lib.c:
**
** The source file implementing library functions.
**
** Author: Halil Ibrahim Ilhan. (c) 2021
**
** Revision: 2021.03.14.18.30
** 
*/

#include <stdio.h>
#include <math.h>   /* used math.h for fabs() function in root finding*/
#include "hw1_lib.h"

/* calculation the integral value of third degree polynomial between xs and xe with left riemann sum method */
double integral3(double a0, double a1, double a2, double a3, double xs, double xe, double delta)
{
    double x, y, r;

    /* start from xs */
    x = xs;
    while (x < xe)
    {
        /* calculate height */
        y = ((a0 * x * x * x) + (a1 * x * x) + (a2 * x) + a3);
        /* and add area to result */
        r += y * delta;
        /* go to next rectangle */
        x += delta;
    }

    return r;
}

/* calculation the integral value of fourth degree polynomial between xs and xe with left riemann sum method */
double integral4(double a0, double a1, double a2, double a3, double a4, double xs, double xe, double delta)
{
    double x, y, r;

    /* start from xs */
    x = xs;
    while (x < xe)
    {
        /* calculate height */
        y = (a0 * x * x * x * x) + (a1 * x * x * x) + (a2 * x * x) + (a3 * x) + a4;
        /* and add area to result */
        r += y * delta;
        /* go to next rectangle */
        x += delta;
    }

    return r;
}

/* finds the root of third degree polynomial between xs and xe with regula falsi method */
double root3(double a0, double a1, double a2, double a3, double xs, double xe)
{
    const double tolerance = 0.0000001;
    double r;
    do
    {
        /* root = xe - (f(xe) * (xe - xs)) / (f(xe) - f(xs)) */
        r = xe - ((((a0 * xe * xe * xe) + (a1 * xe * xe) + (a2 * xe) + a3) * (xe - xs)) / (((a0 * xe * xe * xe) + (a1 * xe * xe) + (a2 * xe) + a3) - ((a0 * xs * xs * xs) + (a1 * xs * xs) + (a2 * xs) + a3)));

        if (((a0 * xs * xs * xs) + (a1 * xs * xs) + (a2 * xs) + a3) * ((a0 * xe * xe * xe) + (a1 * xe * xe) + (a2 * xe) + a3) < 0)
        {
            xe = r;
        }
        else
        {
            xs = r;
        }

    /*continue if |f(r)| > tolerance*/
    } while (fabs((((a0 * r * r * r) + (a1 * r * r) + (a2 * r) + a3)) > tolerance));

    return r;
}

/* finds the root of fourth degree polynomial between xs and xe with regula falsi method */
double root4(double a0, double a1, double a2, double a3, double a4, double xs, double xe)
{
    const double tolerance = 0.0000001;
    double r;
    do
    {
        /* root = xe - (f(xe) * (xe - xs)) / (f(xe) - f(xs)) */
        r = xe - ((((a0 * xe * xe * xe * xe) + (a1 * xe * xe * xe) + (a2 * xe * xe) + (a3 * xe) + a4) * (xe - xs)) / (((a0 * xe * xe * xe * xe) + (a1 * xe * xe * xe) + (a2 * xe * xe) + (a3 * xe) + a4) - ((a0 * xs * xs * xs * xs) + (a1 * xs * xs * xs) + (a2 * xs * xs) + (a3 * xs) + a4)));

        if (((a0 * xs * xs * xs * xs) + (a1 * xs * xs * xs) + (a2 * xs * xs) + (a3 * xs) + a4) * ((a0 * xe * xe * xe * xe) + (a1 * xe * xe * xe) + (a2 * xe * xe) + (a3 * xe) + a4) < 0)
        {
            xe = r;
        }
        else
        {
            xs = r;
        }
    /*continue if |f(r)| > tolerance*/
    } while (fabs((((a0 * r * r * r * r) + (a1 * r * r * r) + (a2 * r * r) + (a3 * r) + a4)) > tolerance));

    return r;
}