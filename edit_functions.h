#pragma once
#pragma scalar_storage_order little - endian

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
 * Renaming unsigned char to byte for convenience.
 **/
typedef unsigned char byte;

/**
 * Structure for TGA file header.
 **/
typedef struct
{
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;

/**
 * Structure for pixel in RGB.
 **/
typedef struct
{
    byte blue;
    byte green;
    byte red;
} Pixel;

/**
 * Gets current type of TGA image from header (int).
 **/
int get_tga_header_type(const TGAHeader *header);

/**
 * Gets current x origin coordinate of TGA image from header (int).
 **/
int get_tga_x_origin(const TGAHeader *header);

/**
 * Gets current y origin coordinate of TGA image from header (int).
 **/
int get_tga_y_origin(const TGAHeader *header);

/**
 * Gets current width of TGA image from header (int).
 **/
int get_tga_header_width(const TGAHeader *header);

/**
 * Gets current height of TGA image from header (int).
 **/
int get_tga_header_height(const TGAHeader *header);

/**
 * Gets current depth of TGA image from header (int).
 **/
int get_tga_depth(const TGAHeader *header);

/**
 * Sets new width for TGA image header (void).
 * Little-endian dependent
 **/
void set_tga_header_width(TGAHeader *header, const int set_to);

/**
 * Sets new height for TGA image header (void).
 * Little-endian dependent
 **/
void set_tga_header_height(TGAHeader *header, const int set_to);

/**
 * Mallocates pointer of Pixel and copies all the pixels from the TGA image into it (Pixel *).
 **/
Pixel *load_pixels(const TGAHeader header, FILE *file);

/**
 * Gets current size of mallocated pointer of Pixel type (int).
 **/
int get_pixels_size(const int height, const int width);

/**
 * Sets values of mallocated pointer of Pixel type (void).
 **/
void set_pixels(Pixel *pixels, const int set_to);

/**
 * Frees and sets to NULL pointer of Pixel type (void).
 **/
void destroy_pixels(Pixel *pixels);

/**
 * resize w,h
 * Resizes the image to the desired dimensions of width w and height h (int).
 * Maximum of dimensions:
 * new width = (2*w)-1
 * new height = (2*h)-1
 * In this program, this function works with the internal image memory.
 * Little-endian dependent
 **/
int resize_f(TGAHeader *header, Pixel *pixels, Pixel *result, const int new_width, const int new_height);

/**
 * crop x,y,w,h
 * Crops the image to desired dimensions of width w and height h from coordinates (x,y) (int).
 * Only a rectangular cutout with width w and height h will be left from the picture.
 * The upper-left corner of the crop will lie at coordinates (x, y).
 * In this program, this function works with the internal image memory.
 * Little-endian dependent
 **/
int crop_f(TGAHeader *header, Pixel *pixels, Pixel *result, const int x_source, const int y_source, const int new_width, const int new_height);

/**
 * copy x1,y1,w,h x2,y2
 * Copies pixels with width w, height h, and upper-left corner to coordinate (x1, y1) to the to coordinates (x2, y2) (void).
 * In this program, this function works with the internal image memory.
 **/
void copy_f(TGAHeader *header, Pixel *pixels, const int x_source, const int y_source, const int new_width, const int new_height, const int x_dest, const int y_dest);

/**
 * move x,y
 * Moves all pixels in the image by x pixels down/up and y pixels left/right (void).
 * positive number -> down/right
 * negative number -> up/left
 * In this program, this function works with the internal image memory.
 **/
void move_f(const TGAHeader *header, Pixel *pixels, const int move_y, const int move_x);

/**
 * bw
 * Converts the image to grayscale (void).
 * The image is still represented by RGB pixels, but their value is adjusted so that they are only in grayscale.
 * In this program, this function works with the internal image memory.
 **/
void bw_f(const TGAHeader *header, Pixel *pixels);

/**
 * mirror -f
 * Mirrors the image depending on the parameter -f (void).
 * It can be (must be checked beforehand):
 * -v -> vertically (is_horizontal = false)
 * -h -> horizontally (is_horizontal = true)
 * In this program, this function works with the internal image memory.
 **/
void mirror_f(const TGAHeader *header, Pixel *pixels, bool is_horizontal);

/**
 * info
 * Prints the current size of the image (void).
 * In this program, this function works with the internal image memory.
 **/
void info_f(const TGAHeader header);

/**
 * save
 * Saves the current version of the internal image memory in the output file specified by the --output command-line argument (void).
 **/
void save_f(const TGAHeader header, const Pixel *pixels, const char *output_path);

/**
 * exit
 * Executes the save command, frees the pointer of Pixel type (void).
 **/
void exit_f(const TGAHeader header, Pixel *pixels, const char *output_path);