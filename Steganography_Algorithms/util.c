#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <tgmath.h>
#include "constants.h"

bool str_case_cmp(const char * p1, const char * p2){
    for(unsigned int i = 0; p1[i] != '\0'; i++){
        if(isalpha(p1[i])){
            if(tolower(p1[i]) != tolower(p2[i])) return false;
            else                                 continue;
        }

        if(p1[i] != p2[i]) return false;
    }

    return true;
}

uint8_t get_bit_from_char(uint8_t n, uint8_t c){
    assert(n < 8);

    uint8_t mask = 1;
    mask <<= n;

    return ((c & mask) >> n);
}

//Not counting null character in msg_len
void append_en_to_image_name(char* restrict arr, uint32_t msg_len, char c){
    uint32_t i = msg_len - 1;
    for(; i != (uint32_t)(-1); i--){
        if(arr[i] == '/')
            break;
    }

    if(i == (uint32_t)(-1))
        i = 0;
    
    for(uint32_t j = msg_len; j != i; j--){
        arr[j + 3] = arr[j];
    }
    arr[i + 1] = 'e';
    arr[i + 2] = c;
    arr[i + 3] = '_';
}

uint64_t i_img(uint32_t width, uint64_t x, uint64_t y){
    return x + y*width;
}

uint8_t min(uint8_t a, uint8_t b){
    return a > b ? b : a;
}

uint8_t min_4(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    return min(min(a, b), min(c, d));
}


uint8_t max(uint8_t a, uint8_t b){
    return a > b ? a : b;
}
uint8_t max_4(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    return max(max(a, b), max(c, d));
}

uint8_t power_2(uint8_t k){
    assert(k < 8);
    return 1 << k;
}

uint8_t u8_fclamp(float f){
    return fmin(fmax(f, 0.0f), 255.0f);
}

uint8_t k_bit_lsb(uint8_t pixel, uint8_t value, uint8_t k){
    assert(k < 8);
    return pixel - (pixel % power_2(k)) + value;
}

uint8_t recover_k_bit_lsb(uint8_t pixel, uint8_t k){
    assert(k < 8);
    return pixel % power_2(k);
}

bool is_power_2(uint8_t p){
    return (p != 0) && !(p & (p - 1));
}

typedef struct read_bit_stream {
    const char * buffer;
    uint32_t buf_len;
    uint32_t cur_index;
    uint8_t cur_bit;        //Index of next bit to read (0, ...,  7)
    bool status;            //If the bitstream has read all the bits, status will be false 
} rBit_stream;

static const uint8_t bit_masks[9] = {
    0x00, //No bits
    0x01, //First bit
    0x03, //First 2 bits
    0x07, //First 3 bits
    0x0F, //First 4 bits
    0x1F, //First 5 bits
    0x3F, //First 6 bits
    0x7F, //First 7 bits
    0xFF  //First 8 bits 
};

/*
static void set_kth_bit(char * restrict buf, uint32_t k){
    assert(buf != NULL);
    assert(k >= 0 && k <= 7);

    (*buf) = (*buf) | ((uint8_t)1 << k);
}

static void clear_kth_bit(char * restrict buf, uint32_t k){
    assert(buf != NULL);
    assert(k >= 0 && k <= 7);

    (*buf) = (*buf) & (~((uint8_t)1 << k));
}

static uint8_t get_kth_bit(char const * restrict buf, uint32_t k){
    assert(buf != NULL);
    assert(k >= 0 && k <= 7);

    return ((*buf) >> k) & ((uint8_t)1);
} 

static uint8_t get_k_lsb_bits(char const * restrict buf, uint32_t k){
    assert(buf != NULL);
    assert(k >= 1 && k <= 8);

    return (*buf) & bit_masks[k];
}*/

rBit_stream* create_read_bitstream(const char * restrict buffer, uint32_t buf_len){
    assert(buffer != NULL);

    rBit_stream* s = malloc(sizeof(rBit_stream));
    if(s == NULL){
        fprintf(stderr, "Unable to allocate memory for read stream.\n");
        return NULL;
    }

    (*s) = (rBit_stream) {buffer, buf_len, 0, 0, true};
    return s;
}

void delete_read_bitstream(rBit_stream * restrict stream){
    assert(stream != NULL);
    free(stream);
}


bool get_rBit_stream_status(const rBit_stream * restrict s){
    assert(s != NULL);
    return s->status;
}

uint8_t get_bits(rBit_stream * restrict stream, uint8_t k){
    assert(stream != NULL);
    assert(stream->buffer != NULL);
    assert(k >= 1 && k <= 8);

    if(stream->status == false){
        return 0;
    }

    uint8_t bits;
    if(stream->cur_bit + k <= NUM_BITS_IN_CHAR){
        bits = (stream->buffer[stream->cur_index] >> stream->cur_bit) & bit_masks[k];
        stream->cur_index += (stream->cur_bit + k)/NUM_BITS_IN_CHAR;
        stream->cur_bit = (stream->cur_bit + k) % NUM_BITS_IN_CHAR;
        
        if(stream->cur_index == stream->buf_len)
            stream->status = false;

        return bits;
    }

    uint8_t num_bits_from_byte1 = NUM_BITS_IN_CHAR - stream->cur_bit;
    bits = (stream->buffer[stream->cur_index] >> stream->cur_bit) & bit_masks[num_bits_from_byte1];
    stream->cur_index += 1;
    stream->cur_bit = 0;

    if(stream->cur_index == stream->buf_len){
        stream->status = false;
        return bits;
    }

    uint8_t num_bits_from_byte2 = k - num_bits_from_byte1;
    bits = bits +
          ((stream->buffer[stream->cur_index] & bit_masks[num_bits_from_byte2]) << num_bits_from_byte1);

    stream->cur_bit += num_bits_from_byte2;
    return bits;
}

typedef struct writed_bit_stream {
    uint32_t buf_len;
    uint32_t cur_index;
    uint8_t cur_bit;
    bool status;
    char buffer[];      
} wBit_stream;

wBit_stream* create_write_bitstream(uint32_t buf_len){
    wBit_stream* stream = calloc(1, sizeof(wBit_stream) + sizeof(char [buf_len]));
    stream->status = true;
    stream->buf_len = buf_len;
    if(stream == NULL){
        fprintf(stderr, "Unable to allocate memory for read stream.\n");
        return NULL;
    }
    return stream;
}

void delete_write_bitstream(wBit_stream * stream){
    assert(stream != NULL);
    free(stream);
}

bool get_wBit_stream_status(const wBit_stream * restrict s){
    assert(s != NULL);
    return s->status;
}

void write_bits(wBit_stream * restrict stream, uint8_t bits, uint8_t k){
    assert(stream != NULL);
    assert(stream->buffer != NULL);
    assert(k >= 1 && k <= 8);
    
    if(stream->status == false) return;

    if(stream->cur_bit + k <= NUM_BITS_IN_CHAR){
        bits = bits & bit_masks[k];
        bits <<= stream->cur_bit;
        stream->buffer[stream->cur_index] += bits;
        stream->cur_index += (stream->cur_bit + k)/NUM_BITS_IN_CHAR;
        stream->cur_bit = (stream->cur_bit + k) % NUM_BITS_IN_CHAR;

        if(stream->cur_index == stream->buf_len)
            stream->status = false;
        
        return;
    }

    uint8_t num_bits_to_byte1 = NUM_BITS_IN_CHAR - stream->cur_bit;
    uint8_t bits_to_byte1 = bits & bit_masks[num_bits_to_byte1];
    bits_to_byte1 <<= stream->cur_bit;
    stream->buffer[stream->cur_index] += bits_to_byte1;

    stream->cur_bit = 0;
    stream->cur_index += 1;

    if(stream->cur_index == stream->buf_len){
        stream->status = false;
        return;
    }

    uint8_t num_bits_to_byte2 = k - num_bits_to_byte1;
    uint8_t bits_to_byte2 = (bits >> num_bits_to_byte1) & bit_masks[num_bits_to_byte2];
    stream->buffer[stream->cur_index] += bits_to_byte2;
    stream->cur_bit += num_bits_to_byte2;
}

void recovery_key_msg(const rBit_stream * restrict stream){
    if(stream->status){
        printf("Full message can't be embedded in the image, embedded first %u characters (bytes) and %u bits.\n", 
               stream->cur_index, stream->cur_bit);
        if(stream->cur_bit != 0) printf("Recovery key is: %u.\n", stream->cur_index + 1);
        else printf("Recovery key is: %u.\n", stream->cur_index);

    }else printf("Recovery key is: %u.\n", stream->cur_index);
}

void print_buffer(const wBit_stream * restrict s){
    fprintf(stdout, "Recovered message: %s\n", s->buffer);
}