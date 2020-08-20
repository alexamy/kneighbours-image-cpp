#include <iostream>
#include "png++/png.hpp"

using namespace std;

int main(int argc, const char *argv[]) {
  png::image< png::rgb_pixel > image("input.png");
  image.write("output.png");
  return 0;
}
