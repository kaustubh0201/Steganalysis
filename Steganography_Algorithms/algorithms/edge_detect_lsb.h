//Description: High payload steganography mechanism using hybrid edge detector
//Authors: Wen-Jan Chen, Chin-Chen Chang, T. Hoang Ngan Le

#ifndef EDGE_DETECT_LSB
#define EDGE_DETECT_LSB

#include <stdint.h>
#include "../image.h"
#include "../util.h"

typedef struct e_edge_detect{
    Image * st_img;
    rBit_stream* stream;
    uint8_t block_size;
    uint8_t non_edge_bits;
    uint8_t edge_bits; 
}e_Edge_Detect;

typedef struct d_edge_detect{
    Image * st_img;
    wBit_stream* stream;
    uint8_t block_size;
    uint8_t non_edge_bits;
    uint8_t edge_bits; 
}d_Edge_Detect;


// Return values:
// -1 - 0 image size
// -2 - 0 message
// -3 - Error in greyscale conversion
e_Edge_Detect construct_e_edge_detect_struct(const char * restrict img_path, uint32_t msg_len,
                                             const char * restrict msg, uint8_t block_size, 
                                             uint8_t non_edge_bits,
                                             uint8_t edge_bits);
void destroy_e_edge_detect_struct(e_Edge_Detect * restrict st);
void edge_detect_encrypt(e_Edge_Detect st_data);


// Return values:
// -1 - 0 image size
// -2 - image not greyscale
// NULL character not counted in msg_len
//msg should be zeroed 
d_Edge_Detect construct_d_edge_detect_struct(const char * restrict img_path, uint32_t msg_len,
                                             uint8_t block_size, uint8_t non_edge_bits, 
                                             uint8_t edge_bits);
void destroy_d_edge_detect_struct(d_Edge_Detect * restrict st);                                             
void edge_detect_decrypt(d_Edge_Detect st_data);


#endif