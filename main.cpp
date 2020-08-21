#include <iostream>
#include "png++/png.hpp"

using namespace std;
using namespace png;

int main(int argc, const char *argv[])
{
  image<rgb_pixel> image_original("input.png");
  uint_32 width = image_original.get_width();
  uint_32 height = image_original.get_height();

  vector<rgb_pixel> original(width * height);
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      original[i + j * width] = image_original.get_pixel(i, j);
    }
  }

  image<rgb_pixel> image_simplified(width, height);
  size_t x, y;
  for(int i = 0; i < original.size(); i++)
  {
    x = i % width;
    y = (i - x) / width;
    image_simplified.set_pixel(x, y, original[i]);
  }

  image_simplified.write("output.png");
  return 0;
}
