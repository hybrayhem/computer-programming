#include <stdio.h>

void calculate_fibonacci_sequence();
void decide_perfect_harmonic_number();
void difference_max_min();
void bmi_calculation();

int main()
{
    calculate_fibonacci_sequence();
    decide_perfect_harmonic_number();
    difference_max_min();
    bmi_calculation();
    return 0;
}

void calculate_fibonacci_sequence()
{
    int term = 0;
    int valid = 0;
    int status = 0;
    int n1, n2, n3;
    char temp;

    printf("\nPlease enter term(s) number : ");
    while (1)
    {
        do
        {
            status = scanf("%d", &term);
            temp = getchar();
            if(temp != '\n') while(getchar() != '\n'); /* clear unnecessary string input */

            /* printf("scanf value = '%d', term = '%d', temp = '%c'\n", status, term, temp); */

            if (temp == '*')
            {
                /*printf("END\n");*/
                break;
            }

            if (status == 0)
            {
                printf("Please enter “numeric” term(s) number: ");
            }
            else if (term < 0)
            {
                printf("Please enter “positive” term(s) number: ");
            }
            else
            {
                valid = 1;
                printf("Fibonacci Sequence:\n");
                n1 = 0;
                n2 = 0;
                n3 = 1;
                while (term > 0)
                {
                    n1 = n2;
                    n2 = n3;
                    printf("%d\n", n3);
                    n3 = n1 + n2;
                    term--;
                }

                printf("\nPlease enter term(s) number : ");
            }
        } while (!valid);

        if (temp == '*')
        {
            /*printf("END\n");*/
            break;
        }
    }
    /*printf("Fibonacci Sequence:\n");
    n1 = 0;
    n2 = 0;
    n3 = 1;
    while (term >= 0)
    {
        n1 = n2;
        n2 = n3;
        printf("%d\n", n3);
        n3 = n1 + n2;
        term--;
    }*/
}

void decide_perfect_harmonic_number()
{
    int i, num;
    int d_sum;
    float d_count;
    float harmonic_divisor_sum;
    float harmonic_mean;
    char temp;

    while(1){
        d_sum = 0;
        d_count = 0.0;
        harmonic_divisor_sum = 0.0;

        printf("\nPlease enter input number : ");
        scanf("%d", &num);
        temp = getchar();

        if (temp == '*')
        {
            /*printf("END\n");*/
            break;
        }

        printf("Natural Number Divisors: ");

        for (i = 1; i <= num; i++)
        {
            if (num % i == 0)
            {
                printf("%d", i);
                if (i != num)
                {
                    d_sum += i;
                    /*printf("\ni: %d, d_sum: %d", i, d_sum);*/
                    printf(", \n");
                }
                

                /*d_sum += i;*/
                d_count++;
                printf("\nbefore sum --> h_d_sum: %f", harmonic_divisor_sum);
                harmonic_divisor_sum += (float) 1 / i;
                printf("\n1/i: %f, h_d_sum: %f\n", (float) 1 / i, harmonic_divisor_sum);
            }
        }
        /*d_sum -= num;*/

        /*printf("    d_sum: %d, num: %d", d_sum, num);*/
        printf("\nIs Perfect Number? : ");
        if (d_sum == num)
        {
            printf("Yes");
        }
        else
        {
            printf("No");
        }

        harmonic_mean = d_count / harmonic_divisor_sum;
        printf("\n d_count: %f, h_mean: %f, int h_mean: %d", d_count, harmonic_mean, (int)harmonic_mean);
        printf("\nIs Harmonic Divisor Number? : ");

        /* round fraction after 5 decimals */
        harmonic_mean = (int) (harmonic_mean * 100000 + .5);
        harmonic_mean = (float)harmonic_mean / 100000;

        if (harmonic_mean == (int)harmonic_mean)
        {
            printf("Yes\n");
        }
        else
        {
            printf("No\n");
        }
    }
}

void difference_max_min()
{
    float n1, n2, n3, n4, n5;
    float min, max;

    printf("\nPlease enter 5 numbers: ");
    scanf("%f %f %f %f %f", &n1, &n2, &n3, &n4, &n5);

    min = n1;
    max = n1;

    if(n2 > max) max = n2;
    if(n2 < min) min = n2;

    if(n3 > max) max = n3;
    if(n3 < min) min = n3;

    if(n4 > max) max = n4;
    if(n4 < min) min = n4;
    
    if(n5 > max) max = n5;
    if(n5 < min) min = n5;

    printf("Maximum number is: %.5g\n", max);
    printf("Minimum number is: %.5g\n", min);
    printf("Difference between maximum and minimum is %.5g\n", max - min);
}

void bmi_calculation()
{
    float h, w, bmi;

    printf("\nPlease enter weight(kg) : ");
    scanf("%f", &w);

    printf("Please enter height(m) : ");
    scanf("%f", &h);

    bmi = w / (h * h);

    printf("Your category: ");
    if (bmi < 16.0)
    {
        printf("Severely Underweight\n");
    }
    else if (bmi <= 18.4)
    {
        printf("Underweight\n");
    }
    else if (bmi <= 24.9)
    {
        printf("Normal\n");
    }
    else if (bmi <= 29.9)
    {
        printf("Owerweight\n");
    }
    else
    {
        printf("Obese\n");
    }
}
