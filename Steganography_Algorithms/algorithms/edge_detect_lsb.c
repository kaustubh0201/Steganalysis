#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "../image.h"
#include "../util.h"
#include "edge_detect_lsb.h"


void edge_detect_encrypt(e_Edge_Detect st_data){
    Image * st_img = st_data.st_img;
    
    Image edge = hybrid_edge_detector(st_img);
    const uint8_t channels = st_img->channels;

    const uint64_t size = edge.image_size - (edge.image_size % st_data.block_size); 

    uint8_t block_id = 0;
    uint8_t bits = 0;
    for(uint64_t i = 0; i < size; i += st_data.block_size * channels){
        if(!get_rBit_stream_status(st_data.stream)) break;

        for(uint8_t j = 1; j < st_data.block_size; j++){
            if(!get_rBit_stream_status(st_data.stream)) break;

            if(edge.img_p[i + j * channels] == 255){
                block_id += power_2(j - 1);
                bits = get_bits(st_data.stream, st_data.edge_bits);
                st_img->img_p[i + j * channels] = k_bit_lsb(st_img->img_p[i + j * channels], bits, st_data.edge_bits);
            }else{
                bits = get_bits(st_data.stream, st_data.non_edge_bits);
                st_img->img_p[i + j * channels] = k_bit_lsb(st_img->img_p[i + j * channels], bits, st_data.non_edge_bits);
            }
        }
        st_img->img_p[i] = k_bit_lsb(st_img->img_p[i], block_id, st_data.block_size - 1);

        block_id = 0;
    }

    recovery_key_msg(st_data.stream);

    free_image(&edge);
}

void edge_detect_decrypt(d_Edge_Detect st_data){
    Image * st_img = st_data.st_img;
    const uint8_t channels = st_img->channels;
    const uint64_t size = st_img->image_size - st_img->image_size % st_data.block_size; 

    uint8_t block_id = 0;
    uint8_t bits = 0;

    for(uint64_t i = 0; i < size; i += st_data.block_size * channels){
        if(!get_wBit_stream_status(st_data.stream)) break;

        block_id = recover_k_bit_lsb(st_img->img_p[i], st_data.block_size - 1);

        for(uint8_t j = 1; j < st_data.block_size; j++, block_id >>= 1){
            if(!get_wBit_stream_status(st_data.stream)) break;
            if((block_id & 1) == 1){
                bits = recover_k_bit_lsb(st_img->img_p[i + j * channels], st_data.edge_bits);
                write_bits(st_data.stream, bits, st_data.edge_bits);
            }else{
                bits = recover_k_bit_lsb(st_img->img_p[i + j * channels], st_data.non_edge_bits);
                write_bits(st_data.stream, bits, st_data.non_edge_bits);
            }

        }

    }
}

void destroy_e_edge_detect_struct(e_Edge_Detect * restrict st){
    assert(st != NULL);

    free_image(st->st_img);
    free(st->st_img);
    delete_read_bitstream(st->stream);        
}

e_Edge_Detect construct_e_edge_detect_struct(const char * restrict img_path, uint32_t msg_len,
                                             const char * restrict msg, uint8_t block_size, 
                                             uint8_t non_edge_bits,
                                             uint8_t edge_bits){

    assert(img_path != NULL);
    assert(msg != NULL);

    if(block_size == 0){
        fprintf(stderr, "Invalid block size, must be greater than 0.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    if(edge_bits > 8){
        fprintf(stderr, "Invalid value for number of bits to embed in edge pixels.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    if(non_edge_bits > 8){
        fprintf(stderr, "Invalid value for number of bits to embed in non-edge pixels.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    e_Edge_Detect st;

    Image* img = malloc(sizeof(Image));
    if(img == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    //Convert to greyscale
    if(img->channels > 2){
        Image grey = convert_to_greyscale(img);

        if(grey.img_p == NULL){
            fprintf(stderr, "Error in greycale conversion.\n");
            return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
        }

        free_image(img);
        *(img) = grey;
    }    

    rBit_stream* stream = create_read_bitstream(msg, msg_len);
    if(stream == NULL){
        fprintf(stderr, "Unable to allocate memory for message bit stream.\n");
        return (e_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    st.st_img = img;
    st.stream = stream;
    st.block_size = block_size;
    st.non_edge_bits = non_edge_bits;
    st.edge_bits = edge_bits;

    return st;
}

void destroy_d_edge_detect_struct(d_Edge_Detect * restrict st){
    assert(st != NULL);

    free_image(st->st_img);
    free(st->st_img);
    delete_write_bitstream(st->stream);        
}

d_Edge_Detect construct_d_edge_detect_struct(const char * restrict img_path, uint32_t msg_len,
                                             uint8_t block_size, uint8_t non_edge_bits, 
                                             uint8_t edge_bits){
    assert(img_path != NULL);

    if(block_size == 0){
        fprintf(stderr, "Invalid block size, must be greater than 0.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    if(edge_bits > 8){
        fprintf(stderr, "Invalid value for number of bits to embed in edge pixels.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    if(non_edge_bits > 8){
        fprintf(stderr, "Invalid value for number of bits to embed in non-edge pixels.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    d_Edge_Detect st;

    Image* img = malloc(sizeof(Image));
    if(img == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    *(img) = load_image(img_path);
    if(img->img_p == NULL){
        fprintf(stderr, "Unable to allocate memory for image.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    //check greyscale
    if(img->channels > 2){
        fprintf(stderr, "Not a greyscale image.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }    

    wBit_stream* stream = create_write_bitstream(msg_len);
    if(stream == NULL){
        fprintf(stderr, "Unable to allocate memory for message bit stream.\n");
        return (d_Edge_Detect){NULL, NULL, 0, 0, 0};
    }

    st.st_img = img;
    st.stream = stream;
    st.block_size = block_size;
    st.non_edge_bits = non_edge_bits;
    st.edge_bits = edge_bits;

    return st;  
}
