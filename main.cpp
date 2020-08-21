#include <iostream>
#include <algorithm>
#include "png++/png.hpp"

#define COLOR_COUNT 8
#define EPSILON 5

using namespace std;
using namespace png;

struct rgb {
  int red;
  int green;
  int blue;
};

// get image pixels
vector<rgb_pixel> get_pixels(image<rgb_pixel> &image) {
  uint_32 width = image.get_width();
  uint_32 height = image.get_height();
  vector<rgb_pixel> pixels(width * height);
  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      pixels[i + j * width] = image.get_pixel(i, j);
    }
  }
  return pixels;
}

int main(int argc, const char *argv[])
{
  // startup
  srand(time(NULL));
  string file_name = argv[1];

  // load image
  image<rgb_pixel> image_original(file_name);
  uint_32 width = image_original.get_width();
  uint_32 height = image_original.get_height();
  uint_32 size = width * height;

  // get pixels of input image
  vector<rgb_pixel> original = get_pixels(image_original);

  // fill colors with random values
  vector<rgb_pixel> colors(COLOR_COUNT);
  for(int i = 0; i < COLOR_COUNT; i++)
  {
    colors[i] = original[rand() % size];
  }

  // categorize pixels
  vector<uint_32> categories(size);
  vector<uint_32> distances(COLOR_COUNT);
  vector<uint_32>::iterator curr_distance;
  int max_distance, prev_max_distance;
  int step = 0;

  do
  {
    printf("Run #%d\n", ++step);

    prev_max_distance = max_distance;
    max_distance = 0;

    // find category for every pixel
    for(int i = 0; i < size; i++)
    {
      // find distances to each category
      for(int c = 0; c < COLOR_COUNT; c++)
      {
        distances[c] = abs(original[i].red - colors[c].red) + abs(original[i].green - colors[c].green) + abs(original[i].blue - colors[c].blue);
      }
      // assign new category
      curr_distance = min_element(distances.begin(), distances.end());
      categories[i] = distance(distances.begin(), curr_distance);
      if(*curr_distance > max_distance) max_distance = *curr_distance;
    }

    // gather new colors from average of pixels
    vector<rgb> color_sum(COLOR_COUNT);
    vector<uint_32> colors_count(COLOR_COUNT, 1);

    for(int i = 0; i < size; i++)
    {
      color_sum[categories[i]].red += original[i].red;
      color_sum[categories[i]].green += original[i].green;
      color_sum[categories[i]].blue += original[i].blue;
      colors_count[categories[i]]++;
    }
    for(int i = 0; i < COLOR_COUNT; i++) {
      colors[i].red = (png::byte)(color_sum[i].red / colors_count[i]);
      colors[i].green = (png::byte)(color_sum[i].green / colors_count[i]);
      colors[i].blue = (png::byte)(color_sum[i].blue / colors_count[i]);
    }
  }
  while(abs(prev_max_distance - max_distance) > EPSILON);

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

  return 0;
}
