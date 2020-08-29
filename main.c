#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <libpng_wrapper.h>

int main(int argc, char *argv[]) {
  if(argc != 3) abort();

  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_bytep *row_pointers = NULL;

  read_png_file(argv[1], &row_pointers, &width, &height, &color_type, &bit_depth);
  process_png_file(row_pointers, width, height);
  write_png_file(argv[2], row_pointers, width, height);

  return 0;
}
