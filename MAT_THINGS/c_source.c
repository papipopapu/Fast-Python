#include <stdio.h>
#include <stdlib.h>
#include "dct.c"
void print_ui8(u_int8_t *matrix, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", matrix[i * m + j]);
        }
        printf("\n");
    }
}
void print_i8(int8_t *matrix, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", matrix[i * m + j]);
        }
        printf("\n");
    }
}
void print_i16(int16_t *matrix, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%d ", matrix[i * m + j]);
        }
        printf("\n");
    }
}
void print_f(float *matrix, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%f ", matrix[i * m + j]);
        }
        printf("\n");
    }
}
int main() {
    size_t n = 8, m = 8;
    u_int8_t iso_uint8_block[64] = {
        52, 55, 61, 66, 70, 61, 64, 73,
        63, 59, 55, 90, 109, 85, 69, 72, 
        62, 59, 68, 113, 144, 104, 66, 73,
        63, 58, 71, 122, 154, 106, 70, 69,
        67, 61, 68, 104, 126, 88, 68, 70,
        79, 65, 60, 70, 77, 68, 58, 75, 
        85, 71, 64, 59, 55, 61, 65, 83,
        87, 79, 69, 68, 65, 76, 78, 94
    };
    float iso_float_block[n*m];
    int8_t iso_int8_block[n*m];
    int8_t int8_sequence[n*m];
    u_int8_t aprox_block[n*m];

    get_block(iso_uint8_block, aprox_block, 8, 8, 8, 5, 5);

    print_ui8(aprox_block, 8, 8);
    /*
    block_dct(iso_uint8_block, iso_float_block, 8);

    print_f(iso_float_block, 8, 8);

    block_quantize(QUANT_MATRIX_8_8, iso_int8_block, iso_float_block, 8);

    print_i8(iso_int8_block, 8, 8);

    block_zigzag(iso_int8_block, int8_sequence, 8);

    print_i8(int8_sequence, 8, 8);
    */


    
    return 0;
}