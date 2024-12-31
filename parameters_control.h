#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

/**
 * Prints an error message into the terminal and terminates the program with mode 1 (void).
 **/
void error_exit(const char *error_message);

/**
 * Checks if --input and --output arguments are correctly established (void).
 * Records input and output paths indexes into the *parameters.
 **/
void parameters_checker(int *parameters, const int argc, char **argv);

/**
 * Handles too long or too short values from terminal (void).
 **/
int too_long_short_str(char *str);

/**
 * Counts the order number of num parameter (int).
 **/
int order_counter(int num);

/**
 * Checks if the *str is a digit or not (int).
 **/
int str_digit(char *str);

/**
 * Checks if the argument is valid (int).
 * Can be used only on flag commands.
 **/
int one_argument_validity(char *buffer, char *token, bool *first_p);

/**
 * Checks if both of the arguments are valid (int).
 * Can be used only on digit commands.
 **/
int two_arguments_validity(char *buffer, char *token, int *first_p, int *second_p);

/**
 * Checks if all four arguments are valid (int).
 * Works with nested two_arguments_validity.
 * Can be used only on digit commands.
 **/
int four_arguments_validity(char *buffer, char *token, const char *delim, int *first_p, int *second_p, int *third_p, int *fourth_p);

/**
 * Checks if all 6 arguments are valid (int).
 * Works with nested two_arguments_validity.
 * Can be used only on digit commands while after the 4th number there is ' ', the rest is with ','.
 **/
int six_arguments_validity(char *buffer, char *token, int *first_p, int *second_p, int *third_p, int *fourth_p, int *fifth_p, int *sixth_p);