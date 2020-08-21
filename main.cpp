#include <iostream>
#include "png++/png.hpp"

using namespace std;
using namespace png;

int main(int argc, const char *argv[])
{
  // load image
  image<rgb_pixel> image_original("input.png");
  uint_32 width = image_original.get_width();
  uint_32 height = image_original.get_height();
  uint_32 size = width * height;

  // get pixels of input image
  vector<rgb_pixel> original(size);
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      original[i + j * width] = image_original.get_pixel(i, j);
    }
  }

  // simplify pixels
  vector<rgb_pixel> simplified(original);

  // write simplified image
  image<rgb_pixel> image_simplified(width, height);
  size_t x, y;
  for(int i = 0; i < simplified.size(); i++)
  {
    x = i % width;
    y = (i - x) / width;
    image_simplified.set_pixel(x, y, simplified[i]);
  }

  image_simplified.write("output.png");

  // exit
  return 0;
}
