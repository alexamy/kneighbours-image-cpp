#include <iostream>
#include "png++/png.hpp"

using namespace std;
using namespace png;

int main(int argc, const char *argv[])
{
  image<rgb_pixel> image_original("input.png");
  uint_32 width = image_original.get_width();
  uint_32 height = image_original.get_height();

  vector<vector<rgb_pixel>> original(width);
  for(int i = 0; i < width; i++)
  {
    original[i] = vector<rgb_pixel>(height);
    for(int j = 0; j < height; j++)
    {
      original[i][j] = image_original.get_pixel(i, j);
    }
  }

  image<rgb_pixel> image_simplified(width, height);
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      image_simplified.set_pixel(i, j, original[i][j]);
    }
  }

  image_simplified.write("output.png");
  return 0;
}
