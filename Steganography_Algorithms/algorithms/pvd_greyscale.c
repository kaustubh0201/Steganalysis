#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "../constants.h"
#include "../image.h"


#include "pvd_greyscale.h"

typedef struct partitions{
    uint8_t size;
    uint8_t partitions[];
} Partitions;

//Paritioning scheme 8, 8, 16, 32, 64, 128
static u8_Pair find_difference_range(uint8_t d, const Partitions* restrict par){
    assert(d <= 255);

    uint8_t p = 0;
    for(uint8_t i = 0; i < par->size; i++){
        if(d >= p && d <= p +(par->partitions[i] - 1)){
            return (u8_Pair) {p, p + (par->partitions[i] - 1)};
        }

        p += par->partitions[i]; 
    }

    assert(0);
}

static u8_Pair embedding_func(u8_Pair i_pixel, int16_t d_old, int16_t d_new, bool *out_bounds){
    assert(out_bounds != NULL);
    int16_t diff = d_new - d_old;
    if(abs(d_old) % 2 == 0){
        int16_t x = (int16_t)(i_pixel.x) - floor(diff/2.0f);
        int16_t y = (int16_t)(i_pixel.y) + ceil(diff/2.0f);
        if( x < 0 || x > 255 ||  y < 0 || y > 255)  *out_bounds = true;
        else *out_bounds = false;

        return (u8_Pair){x, y};
    }

    int16_t x = (int16_t)(i_pixel.x) - ceil(diff/2.0f);
    int16_t y = (int16_t)(i_pixel.y) + floor(diff/2.0f);
    if( x < 0 || x > 255 ||  y < 0 || y > 255)  *out_bounds = true;
    else *out_bounds = false;

    return (u8_Pair){x, y};
}

static u8_Pair embed_data(u8_Pair old_vals, e_PVD_GREY st_data, bool * restrict skip)
{
    uint8_t num_bits;
    int16_t d;
    int16_t d_new;
    u8_Pair range = {};
    bool out_bounds = false;

    d = old_vals.y - old_vals.x;
    range = find_difference_range(abs(d), st_data.partitions);
    //Checking if the pixel is eligible or not for embedding
    embedding_func(old_vals, abs(d), range.y, &out_bounds);
    if(out_bounds){
        *skip = true;
        return (u8_Pair){0, 0};
    }

    //Embedding data
    num_bits = log2(range.y - range.x + 1);
    if (num_bits == 0){
        *skip = true;
        return (u8_Pair){0, 0};
    }
    *skip = false;

    d_new = range.x + get_bits(st_data.stream, num_bits);
    //printf("Bits written: %u value written: %u ", num_bits, bits_to_val(&msg[*msg_index], num_bits, *bit_num));
    d_new *= d >= 0 ? 1 : -1;
    

    return embedding_func((u8_Pair){old_vals.x, old_vals.y}, d, d_new, &out_bounds);               
}

static void recover_data(u8_Pair old_vals, d_PVD_GREY st_data)
{
    uint8_t num_bits;
    uint8_t bits;
    int16_t d_new;
    u8_Pair range = {};
    bool out_bounds = false;

    d_new = old_vals.y - old_vals.x;

    range = find_difference_range(abs(d_new), st_data.partitions);
    embedding_func(old_vals, abs(d_new), range.y, &out_bounds);
    if(out_bounds) return;
 
    bits = d_new >= 0 ? d_new - range.x : -d_new - range.x;
    num_bits = log2(range.y - range.x + 1);

    write_bits(st_data.stream, bits, num_bits);

}

void pvd_grayscale_encrypt(e_PVD_GREY st_data){
    bool flip = false;
    bool skip_first_pixel = false;
    bool skip = false;

    Image * st_img = st_data.st_img;

    uint8_t channels = st_img->channels;
    uint8_t *g1 = NULL, *g2 = NULL;
    u8_Pair new_val;

    uint64_t i = 0;
    uint64_t width = st_img->width * st_img->channels;
    for(uint64_t j = 0; j < st_img->height; j++){
        if(!get_rBit_stream_status(st_data.stream)) break;

        if(!flip){
            if(skip_first_pixel) {
                i = channels;
                skip_first_pixel = false;
            }
            else i = 0;

            for(; i < width; i += 2*st_img->channels){
                //Checking if full message is embedded or not
                if(!get_rBit_stream_status(st_data.stream)) break;

                //Getting pixel values
                g1 = &(st_img->img_p[i_img(width, i, j)]);
                if(i == width - (channels)){
                    if(j + 1 == st_img->height) break;

                    g2 = &(st_img->img_p[i_img(width, (width - (channels)), j + 1)]);
                    skip_first_pixel = true;
                    
                }else{
                    g2 = &(st_img->img_p[i_img(width, i + channels, j)]);
                }

                new_val = embed_data((u8_Pair){*g1, *g2}, st_data, &skip);
                if(skip == true)
                    continue;

                //printf("hPos: (%lu, %lu) OG: (%u, %u)", i, j, *g1, *g2);
                *g1 = new_val.x;
                *g2 = new_val.y;
                //printf(" NEW: (%u, %u)\n", *g1, *g2);
            }
        }else{
            if(skip_first_pixel){
                i = (width - (channels)) -  (channels);
                skip_first_pixel = false;
            }
            else i = width - (channels);

            for(;; i -= 2*st_img->channels){
                //Checking if full message is embedded or not
                if(!get_rBit_stream_status(st_data.stream)) break;

                //Getting pixel values
                g1 = &(st_img->img_p[i_img(width, i, j)]);
                if(i == 0){
                    if(j + 1 == st_img->height) break;

                    g2 = &(st_img->img_p[i_img(width, 0, j + 1)]);
                    skip_first_pixel = true;
                }else{
                    g2 = &(st_img->img_p[i_img(width, i - (channels), j)]);
                }

                new_val = embed_data((u8_Pair){*g1, *g2}, st_data, &skip);
                if(skip == true){
                    if(i < 2*st_img->channels) break;
                    else continue;
                }
                //printf("Pos: (%lu, %lu) OG: (%u, %u)", i, j, *g1, *g2);

                *g1 = new_val.x;
                *g2 = new_val.y;

                //printf(" NEW: (%u, %u)\n", *g1, *g2);
                if (i < 2*st_img->channels) break;
            }
        }

        flip = !flip; 
    }

    recovery_key_msg(st_data.stream);
}

void pvd_grayscale_decrypt(d_PVD_GREY st_data){
    bool flip = false;
    bool skip_first_pixel = false;

    Image * st_img = st_data.st_img;
    uint8_t channels = st_img->channels;
    uint8_t g1, g2;

    uint64_t i = 0;
    uint64_t width = st_img->width * st_img->channels;
    for(uint64_t j = 0; j < st_img->height; j++){
       if(!get_wBit_stream_status(st_data.stream)) break;

        if(!flip){
            if(skip_first_pixel) {
                i = channels;
                skip_first_pixel = false;
            }
            else i = 0;

            for(; i < width; i += 2*st_img->channels){
                //Checking if full message is embedded or not
                if(!get_wBit_stream_status(st_data.stream)) break;

                //Getting pixel values
                g1 = (st_img->img_p[i_img(width, i, j)]);
                if(i == width - (channels)){
                    if(j + 1 == st_img->height) break;

                    g2 = (st_img->img_p[i_img(width, (width - (channels)), j + 1)]);
                    skip_first_pixel = true;
                }else{
                    g2 = (st_img->img_p[i_img(width, i + channels, j)]);
                }

                recover_data((u8_Pair){g1, g2}, st_data);
                

            }
        }else{
            if(skip_first_pixel) {
                i = (width - (channels)) -  (channels);
                skip_first_pixel = false;
            }else i = width - (channels);

            for(;; i -= 2*st_img->channels){
                //Checking if full message is embedded or not
                if(!get_wBit_stream_status(st_data.stream)) break;
                //Getting pixel values
                g1 = (st_img->img_p[i_img(width, i, j)]);
                if(i == 0){
                    if(j + 1 == st_img->height) break;

                    g2 = (st_img->img_p[i_img(width, 0, j + 1)]);
                    skip_first_pixel = true;
                }else{
                    g2 = (st_img->img_p[i_img(width, i - (channels), j)]);
                }

                recover_data((u8_Pair){g1, g2}, st_data);

                if (i < 2*st_img->channels) break;
            }
        }

        flip = !flip; 
    }
}

void destroy_e_pvd_grey_struct(e_PVD_GREY * restrict st){
    assert(st != NULL);

    free_image(st->st_img);
    free(st->st_img);
    delete_read_bitstream(st->stream);    
}

e_PVD_GREY construct_e_pvd_grey_struct(const char * restrict img_path, uint32_t msg_len,
                                       const char * restrict msg, const Partitions* restrict p){

    assert(img_path != NULL);
    assert(msg != NULL);
    assert(p != NULL);

    e_PVD_GREY st;

    Image* img = malloc(sizeof(Image));
    if(img == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (e_PVD_GREY){NULL, NULL, NULL};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (e_PVD_GREY){NULL, NULL, NULL};
    }

    //Convert to greyscale
    if(img->channels > 2){
        Image grey = convert_to_greyscale(img);

        if(grey.img_p == NULL){
            fprintf(stderr, "Error in greycale conversion.\n");
            return (e_PVD_GREY){NULL, NULL, NULL};
        }

        free_image(img);
        *(img) = grey;
    }    

    rBit_stream* stream = create_read_bitstream(msg, msg_len);
    if(stream == NULL){
        fprintf(stderr, "Unable to allocate memory for message bit stream.\n");
        return (e_PVD_GREY){NULL, NULL, NULL};
    }

    st.st_img = img;
    st.stream = stream;
    st.partitions = p;

    return st;
}

void destroy_d_pvd_grey_struct(d_PVD_GREY * restrict st){
    assert(st != NULL);

    free_image(st->st_img);
    free(st->st_img);
    delete_write_bitstream(st->stream);    
}

d_PVD_GREY construct_d_pvd_grey_struct(const char * restrict img_path, 
                                       uint32_t msg_len, const Partitions* restrict p){

    assert(img_path != NULL);
    assert(p != NULL);
    d_PVD_GREY st;


    Image* img = malloc(sizeof(Image));
    if(img == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (d_PVD_GREY){NULL, NULL, NULL};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (d_PVD_GREY){NULL, NULL, NULL};
    }
    
    //check greyscale
    if(img->channels > 2){
        fprintf(stderr, "Not a greyscale image.\n");
        return (d_PVD_GREY){NULL, NULL, NULL};
    }

    wBit_stream* stream = create_write_bitstream(msg_len);
    if(stream == NULL){
        fprintf(stderr, "Unable to allocate memory for message bit stream.\n");
        return (d_PVD_GREY){NULL, NULL, NULL};
    }

    st.st_img = img;
    st.stream = stream;
    st.partitions = p;

    return st;
}

Partitions* create_partitions(uint8_t num, ...){
    Partitions* par = malloc(sizeof(Partitions) + sizeof(uint8_t[num]));

    va_list list;
    va_start(list, num);
    uint16_t partition_sum = 0;
    uint8_t v;
    for(uint8_t i = 0; i < num; i++){
        v = va_arg(list, int);

        if(!is_power_2(v)){
            fprintf(stderr, "Bad paritioning, partition length should be power of two.\n");
            free(par);
            return NULL;            
        }
        partition_sum += v;

        if(partition_sum > 256){
            fprintf(stderr, "Bad paritioning, sum adds to more than 256.\n");
            free(par);
            return NULL;
        }

        par->partitions[i] = v;
    }

    if(partition_sum < 256){
        fprintf(stderr, "Bad paritioning, sum doesn't add up to 256.\n");
        free(par);
        return NULL;
    }

    va_end(list);
    par->size = num;
    return par;
}

void destroy_partitions(Partitions * p){
    free(p);
}