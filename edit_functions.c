#include "edit_functions.h"

int get_tga_header_type(const TGAHeader *header)
{
    int type = 0;
    type = (int)header->image_type;
    return type;
}

int get_tga_x_origin(const TGAHeader *header)
{
    int x_origin = 0;
    memcpy(&x_origin, header->x_origin, 2);
    return x_origin;
}

int get_tga_y_origin(const TGAHeader *header)
{
    int y_origin = 0;
    memcpy(&y_origin, header->y_origin, 2);
    return y_origin;
}

int get_tga_header_width(const TGAHeader *header)
{
    int width = 0;
    memcpy(&width, header->width, 2);
    return width;
}

int get_tga_header_height(const TGAHeader *header)
{
    int height = 0;
    memcpy(&height, header->height, 2);
    return height;
}

int get_tga_depth(const TGAHeader *header)
{
    int depth = 0;
    depth = (int)header->depth;
    return depth;
}

void set_tga_header_width(TGAHeader *header, const int set_to)
{
    header->width[0] = set_to & 0xff;
    header->width[1] = (set_to >> 8) & 0xff;
}

void set_tga_header_height(TGAHeader *header, const int set_to)
{
    header->height[0] = set_to & 0xff;
    header->height[1] = (set_to >> 8) & 0xff;
}

Pixel *load_pixels(const TGAHeader header, FILE *file)
{
    int width = get_tga_header_width(&header);
    int height = get_tga_header_height(&header);

    Pixel *pixels = (Pixel *)malloc(sizeof(Pixel) * width * height);
    if (fread(pixels, sizeof(Pixel) * width * height, 1, file) != 1)
    {
        printf("Image pixels loading have failed!\n");
        exit(1);
    }

    return pixels;
}

int get_pixels_size(const int height, const int width)
{
    int pixels_size = 0;
    pixels_size = sizeof(Pixel) * height * width;
    return pixels_size;
}

void set_pixels(Pixel *pixels, const int set_to)
{
    pixels->red = (byte)set_to;
    pixels->green = (byte)set_to;
    pixels->blue = (byte)set_to;
}

void destroy_pixels(Pixel *pixels)
{
    free(pixels);
    pixels = NULL;
}

int resize_f(TGAHeader *header, Pixel *pixels, Pixel *result, const int new_width, const int new_height)
{
    int height = get_tga_header_height(header), width = get_tga_header_width(header);
    bool conditions = (new_height < (2 * height) && new_width < (2 * width));

    if (conditions)
    {
        Pixel *buffer = (Pixel *)malloc(get_pixels_size(height, width));
        memcpy(buffer, pixels, get_pixels_size(height, width));

        set_tga_header_height(header, new_height);
        set_tga_header_width(header, new_width);

        for (int y = 0; y < new_height; y++)
        {

            for (int x = 0; x < new_width; x++)
            {
                int result_offset = y * new_width + x;

                double x_coefficient = (double)x / (double)new_width;

                double y_coefficient = (double)y / (double)new_height;

                int buffer_offset = round(height * y_coefficient) * width + round(width * x_coefficient);

                result[result_offset] = buffer[buffer_offset];
            }
        }

        destroy_pixels(buffer);
        return 0;
    }
    else
    {
        printf("Invalid arguments of command have been inserted, please try again!(range of values should be 1 - %d for height and 1 - %d for width)\n", 2 * height - 1, 2 * width - 1);
        return 1;
    }
}

int crop_f(TGAHeader *header, Pixel *pixels, Pixel *result, const int x_source, const int y_source, const int new_width, const int new_height)
{
    int width = get_tga_header_width(header), height = get_tga_header_height(header), result_y = 0;
    bool conditions = (((new_height + y_source) <= height && (new_width + x_source) <= width));

    if (conditions)
    {
        Pixel *buffer = (Pixel *)malloc(get_pixels_size(height, width));
        memcpy(buffer, pixels, get_pixels_size(height, width));

        set_tga_header_height(header, new_height);
        set_tga_header_width(header, new_width);

        for (int y = y_source; y < (new_height + y_source); y++)
        {
            int result_x = 0;
            for (int x = x_source; x < (new_width + x_source); x++)
            {
                int buffer_offset = y * width + x;
                int result_offset = result_y * new_width + result_x;

                result[result_offset] = buffer[buffer_offset];
                result_x++;
            }
            result_y++;
        }

        destroy_pixels(buffer);
        return 0;
    }
    else
    {
        printf("Invalid arguments of command have been inserted, please try again!(most likely cause is wrong range of values)\n");
        return 1;
    }
}

void copy_f(TGAHeader *header, Pixel *pixels, const int x_source, const int y_source, const int copy_width, const int copy_height, const int x_dest, const int y_dest)
{
    int width = get_tga_header_width(header), height = get_tga_header_height(header), buffer_y = 0;
    bool conditions = ((copy_height > 0 && copy_width > 0) && (x_source >= 0 && y_source >= 0) && (x_dest >= 0 && y_dest >= 0)) && (((copy_height + y_source) <= height && (copy_width + x_source) <= width)) && (((copy_height + y_dest) <= height && (copy_width + x_dest) <= width));

    if (conditions)
    {
        Pixel *buffer = (Pixel *)malloc(get_pixels_size(copy_height, copy_width));

        for (int y = y_source; y < (copy_height + y_source); y++)
        {
            int buffer_x = 0;
            for (int x = x_source; x < (copy_width + x_source); x++)
            {
                int pixel_offset = y * width + x;
                int buffer_offset = buffer_y * copy_width + buffer_x;

                buffer[buffer_offset] = pixels[pixel_offset];
                buffer_x++;
            }
            buffer_y++;
        }

        buffer_y = 0;

        for (int y = y_dest; y < (copy_height + y_dest); y++)
        {
            int buffer_x = 0;
            for (int x = x_dest; x < (copy_width + x_dest); x++)
            {
                int pixel_offset = y * width + x;
                int buffer_offset = buffer_y * copy_width + buffer_x;

                pixels[pixel_offset] = buffer[buffer_offset];
                buffer_x++;
            }
            buffer_y++;
        }

        destroy_pixels(buffer);
    }
    else
    {
        printf("Invalid arguments of command have been inserted, please try again!(most likely cause is wrong range of values)\n");
    }
}

void move_f(const TGAHeader *header, Pixel *pixels, const int move_x, const int move_y)
{
    int height = get_tga_header_height(header), width = get_tga_header_width(header);

    int move_y_coefficient = 0;
    move_y_coefficient = move_y % get_tga_header_height(header);
    int move_x_coefficient = 0;
    move_x_coefficient = move_x % get_tga_header_width(header);

    Pixel *buffer = (Pixel *)malloc(sizeof(Pixel) * width * height);
    memcpy(buffer, pixels, get_pixels_size(height, width));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            int move_x_for = x - (move_x_coefficient);

            if (move_x_for > width - 1)
            {
                move_x_for -= width;
            }
            if (move_x_for < 0)
            {
                move_x_for += width;
            }

            int move_y_for = y - (move_y_coefficient);

            if (move_y_for > height - 1)
            {
                move_y_for -= height;
            }
            if (move_y_for < 0)
            {
                move_y_for += height;
            }

            int pixel_offset = y * width + x;
            int buffer_offset = move_y_for * width + move_x_for;

            pixels[pixel_offset] = buffer[buffer_offset];
        }
    }

    destroy_pixels(buffer);
}

void bw_f(const TGAHeader *header, Pixel *pixels)
{
    int height = get_tga_header_height(header), width = get_tga_header_width(header);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int pixel_offset = y * width + x;

            int gray_factor = pixels[pixel_offset].red * 0.21 + pixels[pixel_offset].green * 0.72 + pixels[pixel_offset].blue * 0.07;

            set_pixels(&pixels[pixel_offset], gray_factor);
        }
    }
}

void mirror_f(const TGAHeader *header, Pixel *pixels, bool is_horizontal)
{
    int height = get_tga_header_height(header), width = get_tga_header_width(header), pixels_y = 0;

    Pixel *buffer = (Pixel *)malloc(sizeof(Pixel) * width * height);
    memcpy(buffer, pixels, get_pixels_size(height, width));

    if (is_horizontal)
    {
        for (int y = 0; y < height; y++)
        {
            int pixels_x = 0;
            for (int x = width - 1; x >= 0; x--)
            {
                int buffer_offset = y * width + x;
                int pixel_offset = pixels_y * width + pixels_x;

                pixels[pixel_offset] = buffer[buffer_offset];

                pixels_x++;
            }
            pixels_y++;
        }
    }
    else
    {
        for (int y = height - 1; y >= 0; y--)
        {
            int pixels_x = 0;
            for (int x = 0; x < width; x++)
            {
                int buffer_offset = y * width + x;
                int pixel_offset = pixels_y * width + pixels_x;

                pixels[pixel_offset] = buffer[buffer_offset];

                pixels_x++;
            }
            pixels_y++;
        }
    }

    destroy_pixels(buffer);
}

void info_f(TGAHeader header)
{
    printf("Current height: %d\n", get_tga_header_height(&header));
    printf("Current width: %d\n", get_tga_header_width(&header));
}

void save_f(const TGAHeader header, const Pixel *pixels, const char *output_path)
{
    int height = get_tga_header_height(&header), width = get_tga_header_width(&header);

    FILE *output_img = fopen(output_path, "wb");
    if (output_img == NULL)
    {
        printf("Invalid output!");
        exit(1);
    }

    fwrite(&header, sizeof(header), 1, output_img);
    fwrite(pixels, get_pixels_size(height, width), 1, output_img);

    fclose(output_img);
    output_img = NULL;
}

void exit_f(const TGAHeader header, Pixel *pixels, const char *output_path)
{
    save_f(header, pixels, output_path);
    destroy_pixels(pixels);
}
