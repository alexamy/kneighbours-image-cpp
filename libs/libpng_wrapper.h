#ifndef LIBPNG_WRAPPER_H
#define LIBPNG_WRAPPER_H

#include <png.h>

void read_png_file(
  char *filename,
  png_bytep **row_pointers,
  png_uint_32 *width,
  png_uint_32 *height,
  png_byte *color_type,
  png_byte *bit_depth
);

void write_png_file(
  char *filename,
  png_bytep *row_pointers,
  png_uint_32 width,
  png_uint_32 height
);

void process_png_file(
  png_bytep *row_pointers,
  png_uint_32 width,
  png_uint_32 height,
  void (*transformer)(int, int, png_bytep, png_bytep, png_bytep*)
);

#endif
