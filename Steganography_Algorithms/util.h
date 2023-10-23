#ifndef UTIL
#define UTIL

#include <stdint.h>
#include <stdbool.h>

typedef struct uint8_pair {
    uint8_t x;
    uint8_t y;
} u8_Pair;

typedef struct uint8_quad {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t w;
} u8_Quad;

typedef struct read_bit_stream rBit_stream;
typedef struct writed_bit_stream wBit_stream;

extern bool str_case_cmp(const char * p1, const char * p2);
extern uint8_t get_bit_from_char(uint8_t n, uint8_t c);

extern void append_en_to_image_name(char* restrict arr, uint32_t msg_len, char c);
extern uint64_t i_img(uint32_t width, uint64_t x, uint64_t y);
extern uint8_t min(uint8_t a, uint8_t b);
extern uint8_t min_4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
extern uint8_t max(uint8_t a, uint8_t b);
extern uint8_t max_4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
extern uint8_t power_2(uint8_t k);
extern uint8_t u8_fclamp(float f);
extern uint8_t k_bit_lsb(uint8_t pixel, uint8_t value, uint8_t k);
extern uint8_t recover_k_bit_lsb(uint8_t pixel, uint8_t k);

extern bool is_power_2(uint8_t p);

void recovery_key_msg(const rBit_stream * restrict stream);

extern rBit_stream* create_read_bitstream(const char * restrict buffer, uint32_t buf_len);
extern void delete_read_bitstream(rBit_stream * restrict stream);
extern bool get_rBit_stream_status(const rBit_stream * restrict s);
extern uint8_t get_bits(rBit_stream * restrict stream, uint8_t k);


extern wBit_stream* create_write_bitstream(uint32_t buf_len);
extern void delete_write_bitstream(wBit_stream * restrict stream);
extern bool get_wBit_stream_status(const wBit_stream * restrict s);
extern void print_buffer(const wBit_stream * restrict s);
extern void write_bits(wBit_stream * restrict stream, uint8_t bits, uint8_t k);

#endif