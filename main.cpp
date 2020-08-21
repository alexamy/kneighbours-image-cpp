#include <iostream>
#include <algorithm>
#include "png++/png.hpp"

#define COLOR_COUNT 8
#define EPSILON 5

using namespace std;
using namespace png;

struct rgb
{
  int red;
  int green;
  int blue;
};

// get image pixels
vector<rgb_pixel> get_pixels(image<rgb_pixel> &image)
{
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

// take random pixels from image
template<typename T>
vector<T> get_random_elements(vector<T> &vec, int count)
{
  vector<T> colors(count);
  for(int i = 0; i < count; i++)
  {
    colors[i] = vec[rand() % vec.size()];
  }
  return colors;
}

// convert pixel array to image
image<rgb_pixel> convert_pixels_to_image(vector<rgb_pixel> &pixels, int width, int height)
{
  size_t x, y;
  image<rgb_pixel> image(width, height);
  for(int i = 0; i < pixels.size(); i++)
  {
    x = i % width;
    y = (i - x) / width;
    image.set_pixel(x, y, pixels[i]);
  }
  return image;
}

// find average of pixels categories
vector<rgb_pixel> find_averages(vector<rgb_pixel> &pixels, vector<uint_32> &categories)
{
  vector<rgb_pixel> colors(COLOR_COUNT);
  vector<rgb> color_sum(COLOR_COUNT);
  vector<uint_32> colors_count(COLOR_COUNT, 1);

  for(int i = 0; i < pixels.size(); i++)
  {
    color_sum[categories[i]].red   += pixels[i].red;
    color_sum[categories[i]].green += pixels[i].green;
    color_sum[categories[i]].blue  += pixels[i].blue;
    colors_count[categories[i]]++;
  }

  for(int i = 0; i < COLOR_COUNT; i++) {
    colors[i].red   = (png::byte)(color_sum[i].red   / colors_count[i]);
    colors[i].green = (png::byte)(color_sum[i].green / colors_count[i]);
    colors[i].blue  = (png::byte)(color_sum[i].blue  / colors_count[i]);
  }

  return colors;
}

// find distance between pixels
inline uint_32 distance_pixels(rgb_pixel &p1, rgb_pixel &p2)
{
  return abs(p1.red - p2.red) + abs(p1.green - p2.green) + abs(p1.blue - p2.blue);
}

// assign categories for pixels
int assign_categories(vector<rgb_pixel> &pixels, vector<rgb_pixel> &colors, vector<uint_32> &categories)
{
  vector<uint_32> distances(COLOR_COUNT);
  vector<uint_32>::iterator curr_distance;
  int max_distance = 0;

  for(int i = 0; i < pixels.size(); i++)
  {
    for(int c = 0; c < COLOR_COUNT; c++)
    {
      distances[c] = distance_pixels(pixels[i], colors[c]);
    }

    curr_distance = min_element(distances.begin(), distances.end());
    categories[i] = distance(distances.begin(), curr_distance);
    if(*curr_distance > max_distance) max_distance = *curr_distance;
  }

  return max_distance;
}

// main
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

  // simplify
  vector<rgb_pixel> pixels = get_pixels(image_original);
  vector<rgb_pixel> colors = get_random_elements<rgb_pixel>(pixels, COLOR_COUNT);
  vector<uint_32> categories(size);

  int max_distance, prev_max_distance;
  int step = 0;

  do
  {
    printf("Run #%d\n", ++step);
    prev_max_distance = max_distance;
    max_distance = assign_categories(pixels, colors, categories);
    colors = find_averages(pixels, categories);
  }
  while(abs(prev_max_distance - max_distance) > EPSILON);

  // write simplified image
  vector<rgb_pixel> simplified(size);
  for(int i = 0; i < size; i++) {
    simplified[i] = colors[categories[i]];
  }

  image<rgb_pixel> image_simplified = convert_pixels_to_image(simplified, width, height);
  image_simplified.write("output.png");

  return 0;
}
