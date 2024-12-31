#include "parameters_control.h"

void error_exit(const char *error_message)
{
    printf("%s\n", error_message);
    exit(1);
}

void parameters_checker(int *parameters, const int argc, char **argv)
{
    if (argc == 5)
    {
        int checker[2];
        memset(checker, 0, sizeof(checker));

        for (int i = 1; i < argc - 1; i++)
        {
            if (strcmp(argv[i], "--input") == 0)
            {
                parameters[0] = i + 1;
                checker[0]++;
            }
            if (strcmp(argv[i], "--output") == 0)
            {
                parameters[1] = i + 1;
                checker[1]++;
            }
        }

        if ((checker[0] != 1 || checker[1] != 1) || (strcmp(argv[parameters[0]], "--output") == 0 || strcmp(argv[parameters[1]], "--input") == 0))
        {
            error_exit("Invalid parameters have been established!");
        }
    }
    else
    {
        error_exit("Invalid parameters have been established!");
    }
}

int too_long_short_str(char *str)
{
    if (strlen(str) > 50 || str[0] == '\n')
    {
        if (strstr(str, "\n") == NULL)
        {
            do
            {
                fgets(str, sizeof(str), stdin);
            } while (strstr(str, "\n") == NULL);
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

int order_counter(int num)
{
    int counter = 1;

    if (num < 0)
    {
        counter++;
        num *= -1;
    }

    num /= 10;

    while (num > 0)
    {
        num /= 10;
        counter++;
    }

    return counter;
}

int str_digit(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '-' && strlen(str) == 1)
        {
            return 1;
        }
        else if (!isdigit(str[i]) && str[i] != '-')
        {
            return 1;
        }
        else
        {
            continue;
        }
    }
    return 0;
}

int one_argument_validity(char *buffer, char *token, bool *first_p)
{
    strtok(buffer, " ");
    token = strtok(NULL, " -");
    if (token != NULL)
    {
        if (strcmp(token, "h") == 0)
        {
            *first_p = true;
            return 0;
        }
        else if (strcmp(token, "v") == 0)
        {
            *first_p = false;
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

int two_arguments_validity(char *buffer, char *token, int *first_p, int *second_p)
{
    strtok(buffer, " ");
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        if (str_digit(token) == 0)
        {
            *first_p = atoi(token);
            token = strtok(NULL, ",");
            if (token != NULL)
            {
                if (str_digit(token) == 0)
                {
                    *second_p = atoi(token);

                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

int four_arguments_validity(char *buffer, char *token, const char *delim, int *first_p, int *second_p, int *third_p, int *fourth_p)
{
    if (two_arguments_validity(buffer, token, first_p, second_p) == 0)
    {
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            if (str_digit(token) == 0)
            {
                *third_p = atoi(token);
                token = strtok(NULL, delim);
                if (token != NULL)
                {
                    if (str_digit(token) == 0)
                    {
                        *fourth_p = atoi(token);

                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

int six_arguments_validity(char *buffer, char *token, int *first_p, int *second_p, int *third_p, int *fourth_p, int *fifth_p, int *sixth_p)
{
    if (four_arguments_validity(buffer, token, " ", first_p, second_p, third_p, fourth_p) == 0)
    {
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            if (str_digit(token) == 0)
            {
                *fifth_p = atoi(token);
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    if (str_digit(token) == 0)
                    {
                        *sixth_p = atoi(token);

                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}