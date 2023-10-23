#ifndef IMAGE
#define IMAGE

#include <stdint.h>
#include <stdbool.h>

#include "util.h"

enum image_creator{
    MALLOC, 
    LIBRARY
};

typedef struct{
    int32_t width;
    int32_t height;
    int32_t channels;
    uint8_t* img_p;
    uint64_t image_size;
    enum image_creator ic;
} Image;

extern Image load_image(const char* img_path);
extern void write_png(const char* img_path, Image img);
extern void free_image(Image* img);
extern Image convert_to_greyscale(const Image* img);
extern Image fuzzy_edge_detector(const Image* img);
extern Image canny_edge_detector(const Image* img);
extern Image hybrid_edge_detector(const Image* img);

#endif