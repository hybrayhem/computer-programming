#include <stdio.h>

/* customer types */
typedef enum
{
	employee,
	retired,
	student,
	unemployed,
	other
} label;

/* bank customer */
typedef struct
{
	char name[20];
	int age;
	label label;
} customer;


/* prints current sequence */
void current_seq(customer customers[], int len)
{
	int i;

	if (len > 0)
	{
		for (i = 0; i < (len - 1); i++)
		{
			printf("%d-", customers[i].label);
		}
		printf("%d", customers[len - 1].label);
	}
}

/* processes and removes the first customer in sequence */
void process_customer(customer customers[], int *len)
{
	int i;

	if (*len > 0)
	{
		/* shift array to left */
		for (i = 0; i < *len; i++)
		{
			customers[i] = customers[i + 1];
		}
		/* discharge the last copy customer */
		customers[i - 1].name[0] = '\0';
		customers[i - 1].age = 0;
		customers[i - 1].label = 0;

		(*len)--;
	}
	else
	{
		printf("There is not any customer in bank system sequence.\n");
	}
}

/* returns the index that customer should be go */
int get_customer_index(label label, customer customers[], int len)
{
	int i = 0, j = 0;
	int maxseqlen[] = {5, 3, 3, 2, 2}; /* max number of labels in a group provided by problem */

	/* go until find a group */
	while (customers[i].label != label && i < len)
	{
		i++;
	}

	/* if group found check is it full */
	for (; i < len; i++)
	{
		/* if group is full search another */
		if (j >= maxseqlen[label])
		{
			j = 0;
			while (customers[i].label != label && i < len)
			{
				i++;
			}
		}

		if (customers[i].label == label)
		{
			j++;
		}
		else
		{
			return i; /* if group not full take the customer here */
		}
	}

	return len; /* take the customer to end as a last option */
}

/* gets information of customer and places it in sequence */
void add_customer(customer customers[], int *len)
{
	int label, index, i;
	customer temp;

	if (*len < 50)
	{
		printf("\nPlease enter the name of the customer : ");
		scanf("%s", temp.name);

		printf("Please enter the age of the customer : ");
		scanf("%d", &temp.age);

		if(temp.age < 18){
			printf("Too young for being a bank customer.\n");
			return;
		}

		printf("Please enter the label of the customer : ");
		scanf("%d", &label);

		if(label < 0 || label > 4){
			printf("Invalid label.\n");
			return;
		}

		temp.label = label;

		index = get_customer_index(label, &customers[0], *len); /* where the customer should be placed */
		for (i = *len; i >= index; i--) /* shift sequence right from starting new customers place */
		{
			customers[i + 1] = customers[i];
		}
		customers[index] = temp; /* take the new customer into reserved place */

		(*len)++;
	}
	else
	{
		printf("Bank is full!\n");
	}
}


int main()
{
	customer customers[50] = {{0, 0, 0}}; /* customer array filled with 0's */
	int seqlen = 0; /* index of sequence end */
	int slct = 5, stus;

	printf("***********Banking System***********\n\n");
	do
	{
		if (slct == 1)
		{
			add_customer(&customers[0], &seqlen);
		}
		else if (slct == 2)
		{
			process_customer(&customers[0], &seqlen);
		}
		else if (slct == -1)
		{
			printf("exit\n");
			break;
		}
		else
		{
			printf("Invalid input.\n");
		}
		printf("\n");
		
		printf("Current Sequence: ");
		current_seq(customers, seqlen);
		printf("\n1 - Add customer\n");
		printf("2 - Process customer\n");
		printf("Enter -1 for exit\n");
		printf("\n> ");

		stus = scanf("%d", &slct);
		if(stus <= 0) printf("Invalid input.\n");

	} while (stus > 0); /* run until scanf fails */

	return 0;
}
