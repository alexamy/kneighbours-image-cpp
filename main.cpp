#include <iostream>
#include "png++/png.hpp"

using namespace std;
using namespace png;

int main(int argc, const char *argv[])
{
  image<rgb_pixel> image("input.png");

  for(int i = 0; i < image.get_width(); i++)
  {
    for(int j = 0; j < image.get_height(); j++)
    {
      rgb_pixel pixel = image.get_pixel(i, j);
    }
  }

  image.write("output.png");
  return 0;
}
