#include <iostream>
#include <algorithm>
#include "png++/png.hpp"

using namespace std;
using namespace png;

struct rgb {
  int red;
  int green;
  int blue;
};

int main(int argc, const char *argv[])
{
  // constants
  int color_count = 8;
  int epsilon = 5;
  string file_name = "input.png";

  // load image
  image<rgb_pixel> image_original(file_name);
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

  // fill colors with random values
  vector<rgb_pixel> colors(color_count);

  for(int i = 0; i < color_count; i++)
  {
    colors[i] = original[rand() % size];
  }

  // categorize pixels
  vector<int> categories(size);
  vector<int> distances(color_count);
  vector<int>::iterator curr_distance;
  int max_distance, prev_max_distance;
  int r, g, b;

  do
  {
    prev_max_distance = max_distance;
    max_distance = 0;

    // every pixel
    for(int i = 0; i < size; i++)
    {
      // find distances to each category
      for(int c = 0; c < color_count; c++)
      {
        distances[c] = abs(original[i].red - colors[c].red) + abs(original[i].green - colors[c].green) + abs(original[i].blue - colors[c].blue);
      }
      // set max distance
      curr_distance = min_element(distances.begin(), distances.end());
      if(*curr_distance > max_distance) max_distance = *curr_distance;
      // assign new category
      categories[i] = distance(distances.begin(), curr_distance);
    }

    // gather new colors from average of pixels
    vector<rgb> color_sum(color_count);
    vector<int> colors_count(color_count);

    for(int i = 0; i < size; i++)
    {
      color_sum[categories[i]].red += original[i].red;
      color_sum[categories[i]].green += original[i].green;
      color_sum[categories[i]].blue += original[i].blue;
      colors_count[categories[i]]++;
    }
    for(int i = 0; i < color_count; i++) {
      colors[i].red = (png::byte)(color_sum[i].red / colors_count[i]);
      colors[i].green = (png::byte)(color_sum[i].green / colors_count[i]);
      colors[i].blue = (png::byte)(color_sum[i].blue / colors_count[i]);
    }
  }
  while((prev_max_distance - max_distance) > epsilon);

  // write simplified image
  image<rgb_pixel> image_simplified(width, height);
  size_t x, y;
  for(int i = 0; i < size; i++)
  {
    x = i % width;
    y = (i - x) / width;
    image_simplified.set_pixel(x, y, colors[categories[i]]);
  }

  image_simplified.write("output.png");

  // exit
  return 0;
}
