#include <stdio.h>

void write_polynomial();
void number_check();
void prime_sum();

/* main calls functions */
int main()
{
    write_polynomial();
    printf("\n");
    number_check();
    printf("\n");
    prime_sum();
    printf("\n");
    return 0;
}


void write_polynomial()
{
    int i = 0;
    float c; /* coefficient */
    int n;
    int first = 1; /* dont print leading sign until first item be printed */

    printf("Enter your polynomial [n a_n a_n-1 a_n-2 ... a_0]: ");
    scanf("%d", &n);

    printf("p(x) = ");
    while (getchar() != '\n')
    {
        scanf("%f", &c);
        /* add sign and coefficient */
        if (c == 0.0)
        {
            /* if coefficient is zero, i. item stays empty and lop continues to building next item*/
            i++;
            continue;
        }
        else if (c == 1.0)
        {
            /* if coefficient is one, adds just plus as leading */
            if (!first)
            {
                printf(" + ");
            }
            else
            {
                first = 0;
            }
        }
        else if (c == -1.0)
        {
            /* if coefficient minus one, adds just minus as leading */
            printf(" - ");
            first = 0;
        }
        else
        {

            if (!first)
            {
                if (c > 0)
                {
                    printf(" + ");
                }
                else
                {
                    printf(" ");
                }
            }

            first = 0;
            /* if coefficient is another number, adds number up to one fraction with sign */
            printf("%.1f", c);
        }

        /* add x and degree if exists */
        if (n - i > 1)
        {
            /* adds x^degree to item */
            printf("x^%d", n - i);
        }
        else if (n - i == 1)
        {
            /* adds 'x' to item */
            printf("x");
        }
        else if (n - i == 0 && (c == 1.0 || c == -1.0))
        {
            /* adds '1' to item if at end */
            printf("1");
        }

        i++;
    }
    printf("\n");
}


/* if input is armstrong returns true */
int is_armstrong(int input)
{
    int digit;
    long int num, sum = 0;

    num = input;
    /* get digits of num until all digits handled */
    while (num != 0)
    {
        digit = num % 10;
        sum += (digit * digit * digit); /* add cubes to armstrong check */
        num /= 10;                      /* delete last digit */
    }

    if (sum == input)
    {
        return 1;
    }

    return 0;
}

/* if input is armstrong returns true */
int is_palindrome(int input)
{
    int r_num = 0; /* reverse of num */
    int num, digit;
    int multiplier = 1; /* to rearrange digits reversely */

    /* store input in num */
    num = input;
    /* get largest decimal value of digits as multiplier */
    while (num != 0)
    {
        num /= 10;
        multiplier *= 10;
    }

    num = input; /* reset num */
    while (multiplier != 0)
    {
        multiplier /= 10; /* move right for digits */

        digit = num % 10; /* get most right digit from num */
        num /= 10;        /* move left in digits */

        r_num += digit * multiplier; /* place digit in most left position */
    }

    /* reversed number == original number */
    if (r_num == input)
    {
        return 1;
    }
    return 0;
}

/* get inputs and handle boolean control functions */
void number_check()
{
    int input, is_a, is_p;


    printf("Please enter an integer number : ");
    scanf("%d", &input);

    /* validation */
    while(input < 0){
        printf("Must be positive: %d\n", input);
        printf("Please enter an integer number : ");
        scanf("%d", &input);
    }

    is_a = is_armstrong(input);
    is_p = is_palindrome(input);

    if (is_a && is_p)
    {
        printf("This number is both Palindrome and Armstrong number.\n");
    }
    else if (is_a)
    {
        printf("This number is only Armstrong number.\n");
    }
    else if (is_p)
    {
        printf("This number is only Palindrome number.\n");
    }
    else
    {
        printf("This number does not satisfy any special cases\n");
    }
}


/* returns num is prime or not */
int is_prime(int num)
{
    int i;

    /* num cant divide by 0 and all nums divisible with 1, so start from 2 */
    for (i = 2; i < num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

/* gets sum of prime numbers between a and b, without including that */
void prime_sum()
{
    int a, b;
    long int sum = 0;

    printf("Please enter first integer number : ");
    scanf("%d", &a);
    printf("Please enter second integer number : ");
    scanf("%d", &b);

    /* validation */
    while ((a < 0 || b < 0) || (a > b))
    {
        printf("Invalid interval! And all values must be integer.\n\n");
        printf("Please enter first integer number : ");
        scanf("%d", &a);
        printf("Please enter second integer number : ");
        scanf("%d", &b);
    };

    printf("Sum of prime numbers between %d and %d : ", a, b);

    a++; /* not includes start and end point */
    while (a < b)
    {

        if (is_prime(a))
        {
            sum += a;
        }
        a++;
    }

    printf("%ld\n", sum);
}
