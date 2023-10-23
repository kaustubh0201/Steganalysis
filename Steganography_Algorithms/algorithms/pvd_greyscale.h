#ifndef PVD_GREYSCALE
#define PVD_GREYSCALE

#include <stdint.h>
#include "../image.h"
#include "../util.h"

typedef struct partitions Partitions;
typedef struct e_pvd_grey {
    Image* st_img;
    rBit_stream* stream;
    const Partitions* partitions;
} e_PVD_GREY;

typedef struct d_pvd_grey {
    Image* st_img;
    wBit_stream* stream;
    const Partitions* partitions;
} d_PVD_GREY;


Partitions* create_partitions(uint8_t num, ...);
void destroy_partitions(Partitions * p);

e_PVD_GREY construct_e_pvd_grey_struct(const char * restrict img_path, uint32_t msg_len,
                                       const char * restrict msg, const Partitions* restrict p);
void destroy_e_pvd_grey_struct(e_PVD_GREY * restrict st);
void pvd_grayscale_encrypt(e_PVD_GREY st_data);


d_PVD_GREY construct_d_pvd_grey_struct(const char * restrict img_path, 
                                       uint32_t msg_len, const Partitions* restrict p);
void destroy_d_pvd_grey_struct(d_PVD_GREY * restrict st);
void pvd_grayscale_decrypt(d_PVD_GREY st_data);


#endif 
