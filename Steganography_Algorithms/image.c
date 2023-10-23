#include <stdint.h>
#include <stdlib.h>
#include <tgmath.h>
#include <stdbool.h>
#include <assert.h>

#include "util.h"
#include "image.h"
#include "constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"


Image load_image(const char * img_path){
    Image i = {};
    i.img_p = stbi_load(img_path, &i.width, &i.height, &i.channels, 0);
    i.image_size = i.width * i.height * i.channels;
    i.ic = LIBRARY;

    i.ic = LIBRARY;
    return i;
}

void write_png(const char* img_path, Image img){
    stbi_write_png(img_path, img.width, img.height, img.channels, img.img_p, img.width*img.channels);
}

void free_image(Image* img){
    if(img->ic == LIBRARY)
        stbi_image_free(img->img_p);
    else if(img->ic == MALLOC)
        free(img->img_p);

    img->width = img->height = img->channels = img->image_size = 0;
    img->img_p = NULL;
}


Image convert_to_greyscale(const Image* img){
    Image grey = {img->width, img->height, img->channels == 4 ? 2 : 1, NULL, 0, MALLOC}; 
    grey.image_size = grey.width * grey.height * grey.channels;
    grey.img_p = malloc(grey.image_size * sizeof(uint8_t));
    
    if(grey.img_p == NULL){
        fprintf(stderr, "Failed to allocate memory for the greyscale conversion.\n");
        return (Image){0};
    }

    for(uint64_t i = 0, j = 0; i != img->image_size && j != grey.image_size; i += img->channels, j += grey.channels){
        grey.img_p[j] = (img->img_p[i] + img->img_p[i + 1] + img->img_p[i + 2])/3;
        if(grey.channels == 2)
            grey.img_p[j + 1] = img->img_p[i + 3]; 
    }

    return grey;

}

Image fuzzy_edge_detector(const Image* img){
    if(img->channels > 2) return (Image){0};

    const uint8_t W = 3;
    const uint8_t TAU = 4;
    const float P = 1.0f;

    //Finding the max grey value
    uint8_t max_value = 0;
    for(uint64_t i = 0; i < img->image_size; i += img->channels){
        if(img->img_p[i] > max_value){
            max_value = img->img_p[i];
            if (max_value == 255) break;
        } 
    }

    //Normalisation with WxW neighbour
    float * const fnorm_img = malloc(img->image_size/img->channels * sizeof(float));
    
    if(fnorm_img == NULL){
        fprintf(stderr, "Failed to allocate memory for the normalisation.\n");
        return (Image){0};
    } 

    const uint64_t width = img->width * img->channels;
    uint8_t min_g = 255;
    uint8_t max_g = 0;
    const uint8_t gi = img->channels - 1;
    uint8_t g = 0; 
    
    const uint8_t w_half = W/2;
    for(uint64_t j = 0; j < img->height; j++){
        for(uint64_t i = 0; i < width; i += img->channels){
            
            for(int8_t k = -w_half; k <= w_half; k++){
                for(int8_t l = -w_half; l <= w_half; l++){
                    if((i + l*(gi + 1) != -1) && (j + k != -1) && (i + l*(gi + 1) < width) && (j + k < img->height)){
                        g = img->img_p[i_img(width, i + l*(gi + 1), j + k)];
                        if(g < min_g){
                            min_g = g;
                        }

                        if(g > max_g){
                            max_g = g;
                        }
                    }
                }
            }

            fnorm_img[i_img(img->width, i/img->channels, j)] = (max_g - min_g)/(float)max_value;
            min_g = 255;
            max_g = 0;
        }
    }


    //Edginess
    float * const fedge_img = malloc(img->image_size/img->channels * sizeof(float));
    
    if(fedge_img == NULL){
        fprintf(stderr, "Failed to allocate memory for the edginess.\n");
        return (Image){0};
    }

    float sum = 0.0f;
    float f = 0.0f;
    for(uint64_t j = 0; j < img->height; j++){
        for(uint64_t i = 0; i < img->width; i++){
            
            for(int8_t k = -w_half; k <= w_half; k++){
                for(int8_t l = -w_half; l <= w_half; l++){
                    if((i + l != -1) && (j + k != -1) && (i + l < width) && (j + k < img->height)){
                        f = fnorm_img[i_img(img->width, i + l, j + k)];
                        sum += fmin(f, pow(1 - pow(f, P), 1.0/P));
                    }
                }
            }

            fedge_img[i_img(img->width, i, j)] = fmin(1.0f, (float)TAU/W * sum);
            sum = 0.0f;
        }
    }
    
    free(fnorm_img);

    //Hystersis
    Image edge = {img->width, img->height, img->channels, NULL, img->image_size, MALLOC};
    edge.img_p = malloc(edge.image_size);
    
    if(edge.img_p == NULL){
        fprintf(stderr, "Failed to allocate memory for the greyscale conversion.\n");
        return (Image){0};
    }

    
    for(uint64_t i = 0; i < edge.image_size; i += edge.channels){
        edge.img_p[i] = fedge_img[i/edge.channels] == 1 ? 255 : 0 ;
        if(edge.channels == 2)
             edge.img_p[i + 1] = img->img_p[i + 1]; 
    }

    free(fedge_img);
    return edge;
}

static uint8_t sort_angle(float angle){
    if(angle < 0)
        angle += M_PI;


    //angle 0
    if ((0 <= angle && angle < M_PI/8.0) || (7.0*M_PI/8.0 <= angle && M_PI))
        return 0;
    //angle 45
    else if ((M_PI/8.0 <= angle && angle < 3.0*M_PI/8.0))
        return 1;
    //angle 90
    else if ((3.0*M_PI/8.0 <= angle && angle < 5.0*M_PI/8.0))
        return 2;
    //angle 135
    else if ((5.0*M_PI/8.0 <= angle && angle < 7.0*M_PI/8.0))
        return 3;

    assert(0);
}

Image canny_edge_detector(const Image* img){
    if(img->channels > 2) return (Image){0};

    const float gaussian_5x5[25] = {0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 
                                    0.015019, 0.059912,	0.094907, 0.059912, 0.015019,
                                    0.023792, 0.094907,	0.150342, 0.094907,	0.023792,
                                    0.015019, 0.059912,	0.094907, 0.059912,	0.015019,
                                    0.003765, 0.015019, 0.023792, 0.015019,	0.003765};
    
    const int8_t sobel_x[9] = {-1, 0, 1,
                              -2, 0, 2,
                              -1, 0, 1};
    
    const int8_t sobel_y[9] = {1, 2, 1,
                               0, 0, 0,
                               -1, -2, -1};
     
    //Applying gaussian blur
    const uint8_t blur_w = 2;
    const uint8_t gi = img->channels - 1;
    const uint64_t width = img->width * img->channels;

    Image blur = {img->width, img->height, img->channels, NULL, img->image_size, MALLOC};
    blur.img_p = malloc(blur.image_size);

    float sum = 0.0f;
    for(uint64_t j = 0; j < img->height; j++){
        for(uint64_t i = 0; i < width; i += img->channels){
            if(blur.channels == 2)
                blur.img_p[i + 1] = img->img_p[i + 1]; 

            for(int8_t k = -blur_w; k <= blur_w; k++){
                for(int8_t l = -blur_w; l <= blur_w; l++){
                    if((i + l*(gi + 1) != -1) && (j + k != -1) && (i + l*(gi + 1) < width) && (j + k < img->height)){
                        sum += img->img_p[i_img(width, i + l*(gi + 1), j + k)] * gaussian_5x5[i_img(5, blur_w + l, blur_w + k)];
                    }
                }
            }

            blur.img_p[i_img(width, i, j)] = u8_fclamp(sum);
            sum = 0.0f;
        }
    }

    //Sobel operator
    float * const grad_mag = malloc(blur.image_size/blur.channels * sizeof(float));
    if(grad_mag == NULL){
        fprintf(stderr, "Failed to allocate memory for the gradient intensity.\n");
        return (Image){0};
    }

    uint8_t * const angle = malloc(blur.image_size/blur.channels * sizeof(uint8_t));
    if(angle == NULL){
        fprintf(stderr, "Failed to allocate memory for the gradient angle.\n");
        return (Image){0};
    }

    float i_x = 0.0f;
    float i_y = 0.0f;
    const uint8_t sobel_w = 1;
    float g_max = 0.0f;
    for(uint64_t j = 0; j < blur.height; j++){
        for(uint64_t i = 0; i < width; i += blur.channels){

            for(int8_t k = -sobel_w; k <= sobel_w; k++){
                for(int8_t l = -sobel_w; l <= sobel_w; l++){
                    if((i + l*(gi + 1) != -1) && (j + k != -1) && (i + l*(gi + 1) < width) && (j + k < blur.height)){
                        i_x += blur.img_p[i_img(width, i + l*(gi + 1), j + k)] * sobel_x[i_img(3, sobel_w + l, sobel_w + k)];
                        i_y += blur.img_p[i_img(width, i + l*(gi + 1), j + k)] * sobel_y[i_img(3, sobel_w + l, sobel_w + k)];
                    }
                }
            }

            grad_mag[i_img(blur.width, i, j)] = hypot(i_x, i_y);
            if(hypot(i_x, i_y) > g_max){
                g_max = hypot(i_x, i_y);
            }
            angle[i_img(blur.width, i, j)] = sort_angle(atan2(i_y, i_x));
            i_y = i_x = 0.0f;
        }
    }

    for(uint64_t i = 0; i < blur.image_size; i += blur.channels){
        blur.img_p[i] = u8_fclamp(grad_mag[i/blur.channels]/g_max * 255.0);
    }

    free(grad_mag);
    
    //Non-maximum suppression
    uint8_t p = 0;
    uint8_t n = 0;
    uint8_t max_pixel = 0;
    for(uint64_t j = 1; j < blur.height - 1; j++){
        for(uint64_t i = 1; i < width - 1; i += blur.channels){

            switch(angle[i_img(blur.width, i, j)]){
                case 0:
                    {
                        p = blur.img_p[i_img(width, i - 1*(gi + 1), j)];
                        n = blur.img_p[i_img(width, i + 1*(gi + 1), j)];
                    }
                    break;
                case 1:
                    {
                        p = blur.img_p[i_img(width, i + 1*(gi + 1), j - 1)];
                        n = blur.img_p[i_img(width, i - 1*(gi + 1), j + 1)];
                    }
                    break;
                case 2:
                    {
                        p = blur.img_p[i_img(width, i, j - 1)];
                        n = blur.img_p[i_img(width, i, j + 1)];
                    }   
                    break;
                case 3:
                    {
                        p = blur.img_p[i_img(width, i - 1*(gi + 1), j - 1)];
                        n = blur.img_p[i_img(width, i + 1*(gi + 1), j + 1)];
                    } 
                    break;
                default:
                    assert(0);
            }

            if(blur.img_p[i_img(width, i, j)] < p || blur.img_p[i_img(width, i, j)] < n){
                blur.img_p[i_img(width, i, j)] = 0;
                continue;
            }

            if(blur.img_p[i_img(width, i, j)] > max_pixel) max_pixel = blur.img_p[i_img(width, i, j)];
        }
    }

    //Setting edge pixels to 0
    for(uint64_t i = 0; i < width; i += blur.channels){
        blur.img_p[i_img(width, i, 0)] = blur.img_p[i_img(width, i, blur.height - 1)] = 0;
    }

    for(uint64_t i = 0; i < blur.height; i += 1){
        blur.img_p[i_img(width, 0, i)] = blur.img_p[i_img(width, width - 1 - gi, i)] = 0;
    }


    free(angle);

    //Double thresholding
    const float high_threshold = 0.09f * max_pixel;
    const float low_threshold = 0.05f * high_threshold;

    const uint8_t strong_pixel = 255;
    const uint8_t weak_pixel = 25;
    uint8_t cur_pixel = 0.0f;

    for(uint64_t j = 0; j < blur.height; j++){
        for(uint64_t i = 0; i < width; i += blur.channels){
            cur_pixel = blur.img_p[i_img(width, i, j)];

            if(cur_pixel < low_threshold)
                blur.img_p[i_img(width, i, j)] = 0;
            else if(cur_pixel >= low_threshold && cur_pixel < high_threshold){
                blur.img_p[i_img(width, i, j)] = weak_pixel;
            }else if(cur_pixel >= high_threshold){
                blur.img_p[i_img(width, i, j)] = strong_pixel;
            }else{
                assert(0);
            }

        }
    }    


    Image output = {img->width, img->height, img->channels, NULL, img->image_size, MALLOC};
    output.img_p = malloc(blur.image_size);

    //Edge tracking with hytersis
    bool has_strong_pixel = false;
    for(uint64_t j = 0; j < blur.height; j++){
        for(uint64_t i = 0; i < width; i += blur.channels){
            if(blur.img_p[i_img(width, i, j)] == strong_pixel || blur.img_p[i_img(width, i, j)] == 0){
                output.img_p[i_img(width, i, j)] = blur.img_p[i_img(width, i, j)];
                continue;
            }
                

            for(int8_t k = -sobel_w; k <= sobel_w; k++){
                for(int8_t l = -sobel_w; l <= sobel_w; l++){
                    if((i + l*(gi + 1) != -1) && (j + k != -1) && (i + l*(gi + 1) < width) && (j + k < blur.height)){
                        if(blur.img_p[i_img(width, i + l*(gi + 1), j + k)] == strong_pixel){
                            has_strong_pixel = true;
                            break;
                        }
                    }
                }

                if(has_strong_pixel) break;
            }

            if(has_strong_pixel) output.img_p[i_img(width, i, j)] = strong_pixel;
            else output.img_p[i_img(width, i, j)] = 0;

            has_strong_pixel = false;
        }
    }

    free_image(&blur);

    return output;
}

Image hybrid_edge_detector(const Image* img){
    if(img->channels > 2)
        return (Image){0};

    Image fuzzy_img = fuzzy_edge_detector(img);
    Image canny_img = canny_edge_detector(img);



    for(uint64_t i = 0; i < img->image_size; i += img->channels){
        if(fuzzy_img.img_p[i] == 255){
            canny_img.img_p[i] = 255;
        }
    }

    free_image(&fuzzy_img);
    return canny_img;
}
