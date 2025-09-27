#include "input_functions.h"
#include "string.h"
#include "time.h"

void flush()
{
	while ((getchar()) != '\n')
		;
}

void read_string(const char *prompt, char *destination)
{
	printf("%s", prompt);
	//scanf("%s", destination); //read until white space
	//white space as in space key, tab key, enter key
	scanf("%[^\n]", destination); //using [^/n] will read until the enter key (new line) is pressed
	flush();
}

float read_float(const char *prompt)
{
	float input;
	printf("%s", prompt);
	scanf("%f", &input);
	flush();
	return input;
}

int read_integer(const char *prompt)
{
	int input;
	printf("%s", prompt);
	scanf("%d", &input);
	flush();
	return input;
}

int read_integer_in_range(const char *prompt, int min, int max)
{
	int input;
	bool repeat = false;

	printf("%s", prompt);

	do
	{
		scanf("%d", &input);
		flush();

		if (input < min && input > max)
		{
			printf("Please enter a value between %d and %d: \n",min,max);
			repeat = true;
		}
		else
		{
			repeat = false;
		}
	} while (repeat);

	return input;
}

bool read_boolean(const char *prompt)
{
	char input[5];

	printf("%s", prompt);
	scanf("%s", input);

	if (strcmp(input, "y") == 0 ||
		strcmp(input, "Y") == 0 ||
		strcmp(input, "yes") == 0 ||
		strcmp(input, "Yes") == 0 ||
		strcmp(input, "YES") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int get_age_from_year_born(int year_born){
    time_t t = time(NULL); // From "time.h"
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900 - year_born;
}
