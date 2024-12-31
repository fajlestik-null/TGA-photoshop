#include "edit_functions.h"
#include "parameters_control.h"

int main(int argc, char **argv)
{
    // command-line argument checking
    int parameters_i_o[2];
    memset(parameters_i_o, 0, sizeof(parameters_i_o));
    parameters_checker(parameters_i_o, argc, argv);

    // TGA loading
    FILE *input_img = fopen(argv[parameters_i_o[0]], "rb");
    if (input_img == NULL)
    {
        error_exit("Image loading have failed!");
    }

    TGAHeader header = {};
    if (fread(&header, sizeof(TGAHeader), 1, input_img) != 1)
    {
        error_exit("Image header loading have failed!");
    }

    if (get_tga_header_type(&header) != 2 || get_tga_x_origin(&header) != 0 || get_tga_y_origin(&header) != 0 || get_tga_depth(&header) != 24)
    {
        error_exit("Unsupported have been inserted format!");
    }

    Pixel *pixels = load_pixels(header, input_img);
    fclose(input_img);

    // command loop
    bool exit = false;
    do
    {
        char buffer[100] = {};
        char validated_str[100] = {};
        char error_output[100] = {};

        fgets(buffer, sizeof(buffer), stdin);

        // handling issues with length
        if (too_long_short_str(buffer) == 0)
        {
            printf("Invalid input, please try again!\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(validated_str, buffer);
        strcpy(error_output, buffer);

        char *token;
        token = strtok(buffer, " ");

        // start of command matching
        if (strcmp(token, "resize") == 0)
        {
            int width = 0, height = 0;
            int *width_p = &width;
            int *height_p = &height;
            int cmd_len = (int)strlen(validated_str);

            if (two_arguments_validity(validated_str, token, width_p, height_p) == 0 && (*height_p > 0 && *width_p > 0) && cmd_len == (8 + order_counter(*width_p) + order_counter(*height_p)))
            {
                int prev_height = get_tga_header_height(&header), prev_width = get_tga_header_width(&header);

                // external buffer
                Pixel *buffer = (Pixel *)malloc(get_pixels_size(prev_height, prev_width));
                memcpy(buffer, pixels, get_pixels_size(prev_height, prev_width));

                free(pixels);

                pixels = malloc(get_pixels_size(*height_p, *width_p));

                if (resize_f(&header, buffer, pixels, width, height) == 1)
                {
                    // invalid return handling
                    free(pixels);
                    pixels = malloc(get_pixels_size(prev_height, prev_width));
                    memcpy(pixels, buffer, get_pixels_size(prev_height, prev_width));
                }

                destroy_pixels(buffer);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "crop") == 0)
        {
            int x = 0, y = 0, width = 0, height = 0;
            int *x_p = &x;
            int *y_p = &y;
            int *width_p = &width;
            int *height_p = &height;
            int cmd_len = (int)strlen(validated_str);

            if (four_arguments_validity(validated_str, token, ",", x_p, y_p, width_p, height_p) == 0 && ((*height_p > 0 && *width_p > 0) && (*x_p >= 0 && *y_p >= 0)) && cmd_len == (8 + order_counter(*width_p) + order_counter(*height_p) + order_counter(*y_p) + order_counter(*x_p)))
            {
                int prev_height = get_tga_header_height(&header), prev_width = get_tga_header_width(&header);

                Pixel *buffer = (Pixel *)malloc(get_pixels_size(prev_height, prev_width));
                memcpy(buffer, pixels, get_pixels_size(prev_height, prev_width));

                free(pixels);

                pixels = malloc(get_pixels_size(*height_p, *width_p));

                if (crop_f(&header, buffer, pixels, *x_p, *y_p, *width_p, *height_p) == 1)
                {
                    free(pixels);
                    pixels = malloc(get_pixels_size(prev_height, prev_width));
                    memcpy(pixels, buffer, get_pixels_size(prev_height, prev_width));
                }

                destroy_pixels(buffer);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "copy") == 0)
        {
            int x = 0, y = 0, width = 0, height = 0, x_dest = 0, y_dest = 0;
            int *x_p = &x;
            int *y_p = &y;
            int *width_p = &width;
            int *height_p = &height;
            int *x_dest_p = &x_dest;
            int *y_dest_p = &y_dest;
            int cmd_len = (int)strlen(validated_str);

            if (six_arguments_validity(validated_str, token, x_p, y_p, width_p, height_p, x_dest_p, y_dest_p) == 0 && cmd_len == (10 + order_counter(*width_p) + order_counter(*height_p) + order_counter(*y_p) + order_counter(*x_p) + order_counter(*y_dest_p) + order_counter(*x_dest_p)))
            {
                copy_f(&header, pixels, *x_p, *y_p, *width_p, *height_p, *x_dest_p, *y_dest_p);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "move") == 0)
        {
            int x = 0, y = 0;
            int *x_p = &x;
            int *y_p = &y;
            int cmd_len = (int)strlen(validated_str);

            if (two_arguments_validity(validated_str, token, y_p, x_p) == 0 && cmd_len == (6 + order_counter(*x_p) + order_counter(*y_p)))
            {
                move_f(&header, pixels, *x_p, *y_p);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "bw") == 0)
        {
            int cmd_len = (int)strlen(validated_str);
            if (cmd_len == 2)
            {
                bw_f(&header, pixels);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "mirror") == 0)
        {
            bool is_horizontal = true;
            bool *is_horizontal_p = &is_horizontal;
            int cmd_len = (int)strlen(validated_str);

            if (one_argument_validity(validated_str, token, is_horizontal_p) == 0 && cmd_len == 9)
            {

                mirror_f(&header, pixels, *is_horizontal_p);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "info") == 0)
        {
            int cmd_len = (int)strlen(validated_str);
            if (cmd_len == 4)
            {
                info_f(header);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "save") == 0)
        {
            int cmd_len = (int)strlen(validated_str);
            if (cmd_len == 4)
            {
                save_f(header, pixels, argv[parameters_i_o[1]]);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else if (strcmp(token, "exit") == 0)
        {
            int cmd_len = (int)strlen(validated_str);
            if (cmd_len == 4)
            {
                exit = true;
                exit_f(header, pixels, argv[parameters_i_o[1]]);
            }
            else
            {
                printf("Command '%s' not found or invalid arguments of command have been inserted, please try again!\n", error_output);
                continue;
            }
        }
        else
        {
            printf("Command '%s' not found, please try again!\n", error_output);
        }
    } while (!exit);

    return 0;
}