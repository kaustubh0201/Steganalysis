#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "../image.h"
#include "reversible_DCT.h"
#include "reversible_dct_mat_consts.h"

#define mat_width 8
#define mat_height 8

static void round_mat(const long double f[mat_height * mat_width], long double o[mat_height * mat_width]){
    assert(f != NULL);
    assert(o != NULL);

    for(uint8_t i = 0; i < mat_height * mat_width; i++){
        o[i] = round(f[i]);
    }
}

static void mat_mult(const long double a[mat_height * mat_width], const long double b[mat_height * mat_width], 
                     long double o[mat_width * mat_height]){

    assert(a != NULL);
    assert(b != NULL);
    assert(o != NULL);
    assert(mat_height == mat_width);
    
    long double sum = 0.0f;
    for(uint8_t j = 0; j < mat_height; j++){
        for(uint8_t i = 0; i < mat_width; i++){

            for(uint8_t k = 0; k < mat_width; k++){
                sum += a[i_img(mat_width, k, i)] * b[i_img(mat_width, j, k)];
            }

            o[i_img(mat_width, j, i)] = sum;
            sum = 0.0;

        }
    }

}

static void transpose(const long double a[mat_height * mat_width], long double o[mat_height * mat_width]){
    for(uint8_t j = 0; j < mat_height; j++){
        for(uint8_t i = 0; i < mat_width; i++){
            o[i_img(mat_width, j, i)] = a[i_img(mat_width, i, j)];
        }
    }
}

static void copy_mat(const long double src[mat_height * mat_width], long double dest[mat_height * mat_width]){
    for(uint8_t i = 0; i < mat_height * mat_width; i++){
        dest[i] = src[i];
    }
}
/*
static void print_mat(const long double a[mat_width * mat_height]){
    for(uint8_t j = 0; j < mat_height; j++){
        for(uint8_t i = 0; i < mat_width; i++){
            printf("%.1f ", a[i_img(mat_width, i, j)]);
        }
        printf("\n");
    }
}*/

static void transform_subimage(const long double a[mat_height * mat_width], long double o[mat_height * mat_width]){    
    long double temp[mat_height * mat_width];
    long double temp2[mat_height * mat_width];
    const long double * const transforms[8] = {s1, s2, s3, s4, s5, s6, s7, s8};
    
    copy_mat(a, temp2);
    //print_mat(temp2);
    for(uint8_t i = 0; i < 2; i++){
        //print_mat(s0);
        mat_mult(s0, temp2, temp);
        //print_mat(temp);
        round_mat(temp, o);
        //printf("0\n");
        //print_mat(o); printf("\n");

        for(uint8_t j = 0; j < 8; j++){
            mat_mult(transforms[j], o, temp);
            round_mat(temp, o);
            //printf("%u\n", j+1); print_mat(o);
        }

        mat_mult(p, o, temp);
        //printf("P\n"); print_mat(temp);
        transpose(temp, temp2);
        //printf("T\n"); print_mat(temp2);
    }

    transpose(temp2, o);
}

static void invsere_transform_subimage(const long double a[mat_height * mat_width], long double o[mat_height * mat_width]){
    long double temp[mat_height * mat_width];
    long double temp2[mat_height * mat_width];
    const long double * const inverse_transforms[9] = {i_s0, i_s1, i_s2, i_s3, i_s4, i_s5, i_s6, i_s7, i_s8};
    
    copy_mat(a, temp2);
    for(uint8_t i = 0; i < 2; i++){
        mat_mult(i_p, temp2, o);
        for(uint8_t j = 8; j != (uint8_t)-1; j--){
            mat_mult(inverse_transforms[j], o, temp);
            round_mat(temp, o);
        }

        transpose(o, temp2);
    }

    transpose(temp2, o);
}

static void embed_data(long double dct[mat_height * mat_width], e_rDCT st_data)
{ 
    assert(dct != NULL);
    assert(st_data.stream != NULL);


    uint8_t bit;
    for(uint8_t j = 0; j < mat_height; j++){
        if(!get_rBit_stream_status(st_data.stream)) break;

        for(uint8_t i = 0; i < mat_width; i++){
            if(!get_rBit_stream_status(st_data.stream)) break;
            
            if((i + j) <= st_data.p) continue;

            if(st_data.q <= dct[i_img(mat_width, j, i)]) dct[i_img(mat_width, j, i)] += st_data.q;
            else if(-st_data.q >= dct[i_img(mat_width, j, i)]) dct[i_img(mat_width, j, i)] -= st_data.q;
            else {
                bit = get_bits(st_data.stream, 1);

                //printf("%u %f\n", bit, dct[i_img(mat_width, j, i)]);

                if(!bit) dct[i_img(mat_width, j, i)] *= 2;
                else dct[i_img(mat_width, j, i)] = 2*dct[i_img(mat_width, j, i)] + 1;

                //printf("%u %f\n", bit, dct[i_img(mat_width, j, i)]);

            }

        }
    }
}

static void recover_data(const long double dct[mat_height * mat_width], d_rDCT st_data)
{
    assert(dct != NULL);
    assert(st_data.stream != NULL);
    
    for(uint8_t j = 0; j < mat_height; j++){
        if(!get_wBit_stream_status(st_data.stream)) break;

        for(uint8_t i = 0; i < mat_width; i++){
            if(!get_wBit_stream_status(st_data.stream)) break;;
            
            if((i + j) <= st_data.p) continue;

            if(2*st_data.q <= dct[i_img(mat_width, j, i)]);
            else if(dct[i_img(mat_width, j, i)] <= -2*st_data.q);
            else{
                
                //printf("%f\n", dct[i_img(mat_width, j, i)]);


                if( ((int32_t)abs(round(dct[i_img(mat_width, j, i)]))) % 2 == 0) 
                    write_bits(st_data.stream, 0, 1);
                else
                    write_bits(st_data.stream, 1, 1);            } 

        }
    }
}


void reversible_DCT_encrypt(e_rDCT st_data){
    assert(st_data.st_img->img_p != NULL);
    assert(st_data.stream != NULL);
    Image* st_img = st_data.st_img;

    uint8_t channels = st_img->channels;
    uint64_t height = st_img->height - (st_img->height % mat_height);
    uint64_t width = (st_img->width - (st_img->width % mat_width)) * channels;


    long double input[mat_width * mat_height];
    long double output[mat_height * mat_width];

    for(uint64_t j = 0; j < height; j += 8){
        if(!get_rBit_stream_status(st_data.stream)) break;

        for(uint64_t i = 0; i < width; i += 8 * channels){
            if(!get_rBit_stream_status(st_data.stream)) break;

            for(uint8_t k = 0; k < mat_height; k++){
                for(uint8_t l = 0; l < mat_width; l++){
                    input[i_img(mat_width, k, l)] = 
                    st_img->img_p[i_img(st_img->width*channels, i + l*channels, j + k)];
                }
            }

            transform_subimage(input, output);
            embed_data(output, st_data);
            invsere_transform_subimage(output, input);

            for(uint8_t k = 0; k < mat_height; k++){
                for(uint8_t l = 0; l < mat_width; l++){
                    st_img->img_p[i_img(st_img->width*channels, i + l*channels, j + k)] = input[i_img(mat_width, k, l)];
                }
            }

        }
    }

    recovery_key_msg(st_data.stream);
}


void reversible_DCT_decrypt(d_rDCT st_data){
    assert(st_data.st_img->img_p != NULL);
    assert(st_data.stream != NULL);


    Image* st_img = st_data.st_img;
    uint8_t channels = st_img->channels;
    uint64_t height = st_img->height - (st_img->height % mat_height);
    uint64_t width = (st_img->width - (st_img->width % mat_width)) * channels;

    long double input[mat_width * mat_height];
    long double output[mat_height * mat_width];

    for(uint64_t j = 0; j < height; j += 8){
        if(!get_wBit_stream_status(st_data.stream)) break;

        for(uint64_t i = 0; i < width; i += 8 * channels){
            if(!get_wBit_stream_status(st_data.stream)) break;

            for(uint8_t k = 0; k < mat_height; k++){
                for(uint8_t l = 0; l < mat_width; l++){
                    input[i_img(mat_width, k, l)] = 
                    st_img->img_p[i_img(st_img->width*channels, i + l*channels, j + k)];
                }
            }

            transform_subimage(input, output);
            recover_data(output, st_data);
        }
    }
}

void destroy_e_rdct_struct(e_rDCT * restrict st){
    assert(st != NULL);

    free_image(st->st_img);
    free(st->st_img);
    delete_read_bitstream(st->stream);
}

void destroy_d_rdct_struct(d_rDCT * restrict st){
    assert(st != NULL); 

    free_image(st->st_img);
    free(st->st_img);
    delete_write_bitstream(st->stream);
}

e_rDCT construct_e_rdct_struct(const char * restrict img_path, uint32_t msg_len, 
                                const char * restrict msg, uint8_t p, int32_t q){
    
    assert(img_path != NULL);
    assert(msg_len != 0);
    assert(msg != NULL);

    e_rDCT er;
    Image * img = malloc(sizeof(Image));
    if (img == NULL){
        fprintf(stderr, "Unable to allocate memory for the image.\n");
        return (e_rDCT){NULL, NULL, 0, 0};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for the image.\n");
        return (e_rDCT){NULL, NULL, 0, 0};
    }

    //Convert to greyscale
    if(img->channels > 2){
        Image grey = convert_to_greyscale(img);

        if(grey.img_p == NULL){
            fprintf(stderr, "Error in greycale conversion.\n");
            return (e_rDCT){NULL, NULL, 0, 0};
        }

        free_image(img);
        *(img) = grey;
    }  

    er.st_img = img;
    er.stream = create_read_bitstream(msg, msg_len);
    er.p = p;
    er.q = q;

    return er;
}

d_rDCT construct_d_rdct_struct(const char * restrict img_path, uint32_t msg_len, uint8_t p, int32_t q){
    assert(img_path != NULL);
    assert(msg_len != 0);
    
    d_rDCT dr;
    Image * img = malloc(sizeof(Image));
    
    if (img == NULL){
        fprintf(stderr, "Unable to allocate memory for the image.\n");
        return (d_rDCT){NULL, NULL, 0, 0};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for the image.\n");
        return (d_rDCT){NULL, NULL, 0, 0};
    }

    //check greyscale
    if(img->channels > 2){
        fprintf(stderr, "Not a greyscale image.\n");
        return (d_rDCT){NULL, NULL, 0, 0};
    }

    if(img->height < mat_height || img->width < mat_width){
        fprintf(stderr, "Small image");
        return (d_rDCT){NULL, NULL, 0, 0};
    }

    dr.st_img = img;
    dr.stream = create_write_bitstream(msg_len);
    dr.p = p;
    dr.q = q;
    
    return dr;
}

#undef mat_width
#undef mat_height