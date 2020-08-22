#include <iostream>
#include <algorithm>
#include <conio.h>
#include "png++/png.hpp"

#define COLOR_COUNT 8
#define PRECISION 5

using namespace std;
using namespace png;

typedef vector<rgb_pixel> pixel_vector;
typedef image<rgb_pixel> rgb_image;

struct rgb
{
  uint_32 red;
  uint_32 green;
  uint_32 blue;
};

struct dimension
{
  uint_32 x;
  uint_32 y;
  uint_32 width;
  uint_32 height;
};

// get image pixels
pixel_vector get_pixels(rgb_image &image)
{
  uint_32 width = image.get_width();
  uint_32 height = image.get_height();
  pixel_vector pixels(width * height);
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
rgb_image convert_pixels_to_image(pixel_vector &pixels, int width, int height)
{
  size_t x, y;
  rgb_image image(width, height);
  for(int i = 0; i < pixels.size(); i++)
  {
    x = i % width;
    y = (i - x) / width;
    image.set_pixel(x, y, pixels[i]);
  }
  return image;
}

// find average of pixels categories
pixel_vector find_averages(pixel_vector &pixels, vector<uint_32> &categories)
{
  pixel_vector colors(COLOR_COUNT);
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
int assign_categories(pixel_vector &pixels, pixel_vector &colors, vector<uint_32> &categories)
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

// fill rectangle in image
void fill_rectangle(rgb_image &image, rgb_pixel &color, dimension d)
{
  for(int i = d.x; i < d.x + d.width; i++)
  {
    for(int j = d.y; j < d.y + d.height; j++)
    {
      image.set_pixel(i, j, color);
    }
  }
}

// copy part of image in another image
void copy_image_part(rgb_image &source, rgb_image &target, dimension ds, dimension dt)
{
  for(int i = ds.x; i < ds.x + ds.width; i++)
  {
    for(int j = ds.y; j < ds.y + ds.height; j++)
    {
      target.set_pixel(dt.x + i, dt.y + j, source.get_pixel(i, j));
    }
  }
}

// setup collage from images and colors
// original and processed must be equal size
rgb_image make_collage(pixel_vector &colors, rgb_image &original, rgb_image &processed)
{
  uint_32 width = original.get_width();
  uint_32 height = original.get_height();
  uint_32 stripe_width = width / COLOR_COUNT;

  rgb_image result(width * 2, height * 2);

  for(int i = 0; i < colors.size(); i++)
  {
    fill_rectangle(result, colors[i], { i * stripe_width, 0, stripe_width, height * 2 });
  }
  copy_image_part(original,  result, { 0, 0, width, height }, { width, 0 });
  copy_image_part(processed, result, { 0, 0, width, height }, { width, height });

  return result;
}

// main
int main(int argc, const char *argv[])
{
  // startup
  srand(time(NULL));
  string file_name;
  rgb_image image_original;

  // load image
  try
  {
    file_name = string(argv[1]);
    image_original = rgb_image(file_name);
  }
  catch(...)
  {
    printf("Provide PNG image as first argument\n");
    printf("Press any key...\n");
    getch();
    return -1;
  }

  uint_32 width = image_original.get_width();
  uint_32 height = image_original.get_height();
  uint_32 size = width * height;

  // simplify
  pixel_vector pixels = get_pixels(image_original);
  pixel_vector colors = get_random_elements<rgb_pixel>(pixels, COLOR_COUNT);
  vector<uint_32> categories(size);

  int prev_max_distance, max_distance = 0;
  int step = 0;

  do
  {
    printf("Run #%d\n", ++step);
    prev_max_distance = max_distance;
    max_distance = assign_categories(pixels, colors, categories);
    colors = find_averages(pixels, categories);
  }
  while(abs(prev_max_distance - max_distance) > PRECISION);

  // write simplified image
  printf("Writing result files\n");

  pixel_vector simplified(size);
  for(int i = 0; i < size; i++) {
    simplified[i] = colors[categories[i]];
  }

  rgb_image image_simplified = convert_pixels_to_image(simplified, width, height);
  string simplified_name = file_name.substr(0, file_name.find(".png")) + "_simplified.png";
  image_simplified.write(simplified_name);

  rgb_image image_collage = make_collage(colors, image_original, image_simplified);
  string collage_name = file_name.substr(0, file_name.find(".png")) + "_simplified_collage.png";
  image_collage.write(collage_name);

  return 0;
}
