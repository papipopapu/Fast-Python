#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

const float QUANT_MATRIX_8_8[64] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
};
void get_block(u_int8_t *IMAGE, u_int8_t *ISO_UINT8_BLOCK, size_t BLOCK_SIZE, size_t IMG_WIDTH, size_t IMG_HEIGHT, size_t I0, size_t J0) {
    /*
    Extract a block of size BLOCK_SIZE from IMAGE at position (I0, J0). If the block overflows the image, it is filled
    with an approximation based on near values to reach BLOCK_SIZE * BLOCK_SIZE pixels.
        * IMAGE: the image to extract the block from.
        * ISO_UINT8_BLOCK: the block to fill outside the image.
        * BLOCK_SIZE: the size of the block.
        * IMG_WIDTH: the width of the image.
        * IMG_HEIGHT: the height of the image.
        * I0: the x coordinate of upper left corner of the block.
        * J0: the y coordinate of upper left corner of the block.
    */  

    int i, j, disti, distj;
    for (i = 0; i < BLOCK_SIZE; i++) {
        for (j = 0; j < BLOCK_SIZE; j++) {
            
            if (j + J0 >= IMG_WIDTH && i + I0 >= IMG_HEIGHT) {
                distj = 2+ J0 + j - IMG_WIDTH; 
                disti = 2+ I0 + i - IMG_HEIGHT;
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = (ISO_UINT8_BLOCK[(i-1) * BLOCK_SIZE + j] + ISO_UINT8_BLOCK[i * BLOCK_SIZE + (j-1)]) / (disti + distj);
            }
            else if (i + I0 > IMG_HEIGHT) {
                disti = 2+ I0 + i - IMG_HEIGHT;
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = ISO_UINT8_BLOCK[(i-1) * BLOCK_SIZE + j] / disti;
            }
            else if (j + J0 > IMG_WIDTH) {
                distj = 2+ J0 + j - IMG_WIDTH;
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = ISO_UINT8_BLOCK[i * BLOCK_SIZE + (j-1)] / distj;
            }
            else if (j + J0 == IMG_WIDTH) {
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = IMAGE[(I0 + i) * IMG_WIDTH + (J0 + j-1)] / 2.;
            }       
            else if (i + I0 == IMG_HEIGHT) {
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = IMAGE[(I0 + i-1) * IMG_WIDTH + (J0 + j)] / 2.;
            }
            else {
                ISO_UINT8_BLOCK[i * BLOCK_SIZE + j] = IMAGE[(I0 + i) * IMG_WIDTH + (J0 + j)];
            }
        }
    }
}

void block_dct(u_int8_t *ISO_UINT8_BLOCK, float *ISO_FLOAT_BLOCK, size_t BLOCK_DIM) {
    /*
    Obtains the discrete cosine transform of the given ISO_BLOCK of pixeks, into the ISO_FLOAT_BLOCK, both of size BLOCK_DIM * BLOCK_DIM.

        * ISO_UINT8_BLOCK: input block, its a pointer to an isolated chunk of memory outside the original image.
        * ISO_FLOAT_BLOCK: output block, its a pointer to an isolated chunk of memory outside the original image. 
        * BLOCK_DIM: block dimension, 8 for 8x8 blocks, 16 for 16x16 blocks, etc.     
          
    */
    float *shifted = (float *)malloc(BLOCK_DIM * BLOCK_DIM * sizeof(float));
    int i, j, k, l;
    float ai, aj, temp, cte = 2./BLOCK_DIM;
    for (i = 0; i < BLOCK_DIM * BLOCK_DIM; i++) {
        shifted[i] = ISO_UINT8_BLOCK[i] - 128;
    }
    for (i = 0; i < BLOCK_DIM; i++) {
        for (j = 0; j < BLOCK_DIM; j++) {
            ai = i == 0 ? M_SQRT1_2 : 1;
            aj = j == 0 ? M_SQRT1_2 : 1;  
            temp = 0;      
            for (k = 0; k < BLOCK_DIM; k++) {
                for (l = 0; l < BLOCK_DIM; l++) {
                    temp +=  shifted[k * BLOCK_DIM + l] * cos(M_PI*i*0.5*(2.*k+1.)/BLOCK_DIM) * cos(M_PI*j*0.5*(2.*l+1.)/BLOCK_DIM);
                }
            } 
            ISO_FLOAT_BLOCK[i * BLOCK_DIM + j] = cte * ai * aj * temp; 
        }
    }
    free(shifted);
}

void general_dct(u_int8_t *ISO_UINT8_BLOCK, float *ISO_FLOAT_BLOCK, size_t BLOCK_WIDTH, size_t BLOCK_HEIGHT) {
    /*
    Obtains the discrete cosine transform of the given chunk of pixeks, into the ISO_FLOAT_BLOCK.
    Used when a whole BLOCK does not fit, here only part of the BLOCK varaible's memory will be used.

        * BLOCK: input block, its a pointer to an isolated chunk of memory outside the original image. 
                 The block is assumed to be of size BLOCK_WIDTH * BLOCK_HEIGHT.
        * ISO_FLOAT_BLOCK: output block, its a pointer to an isolated chunk of memory outside the original image.
        * BLOCK_DIM: block dimension, 8 for 8x8 blocks, 16 for 16x16 blocks, etc.     
    */
    float *shifted = (float *)malloc(BLOCK_WIDTH * BLOCK_HEIGHT* sizeof(float));
    int i, j, k, l;
    float ai, aj, temp, cte = 2./sqrt(BLOCK_WIDTH * BLOCK_HEIGHT);
    for (i = 0; i < BLOCK_WIDTH * BLOCK_HEIGHT; i++) {
        shifted[i] = ISO_UINT8_BLOCK[i] - 128;
    }
    for (i = 0; i < BLOCK_HEIGHT; i++) {
        for (j = 0; j < BLOCK_WIDTH; j++) {
            ai = i == 0 ? M_SQRT1_2 : 1;
            aj = j == 0 ? M_SQRT1_2 : 1;  
            temp = 0;      
            for (k = 0; k < BLOCK_HEIGHT; k++) {
                for (l = 0; l < BLOCK_WIDTH; l++) {
                    temp +=  shifted[k * BLOCK_WIDTH + l] * cos(M_PI*i*0.5*(2.*k+1.)/BLOCK_HEIGHT) * cos(M_PI*j*0.5*(2.*l+1.)/BLOCK_WIDTH);
                }
            } 
            ISO_FLOAT_BLOCK[i * BLOCK_WIDTH + j] = cte * ai * aj * temp; 
        }
    }
    free(shifted);
}

void block_quantize(const float *QUANT_MAT, int8_t *ISO_INT8_BLOCK, float *ISO_FLOAT_BLOCK, size_t BLOCK_DIM) {
    /*
    Quantizies the ISO_FLOAT_BLOCK after a discrete cosine transform using the QUANT_MAT.

        * QUANT_MAT: quantization matrix
        * ISO_INT8_BLOCK: output block, its a pointer to an isolated chunk of memory outside image of int8 type
        * ISO_FLOAT_BLOCK: input block, its a pointer to an isolated chunk of memory outside image of float type
        * BLOCK_DIM: block dimension, 8 for 8x8 blocks, 16 for 16x16 blocks, etc.
    */
    int i;
    for (i = 0; i < BLOCK_DIM * BLOCK_DIM; i++) {  
        ISO_INT8_BLOCK[i] = round(ISO_FLOAT_BLOCK[i] / QUANT_MAT[i]);  
    }
}

void block_zigzag(int8_t *ISO_INT8_BLOCK, int8_t *INT8_SEQUENCE, size_t BLOCK_DIM) {
    /*
    Reorders the ISO_INT8_BLOCK  into INT8_SEQUENCE after a quantization.

        * INT8_SEQUENCE: output block, its a pointer to an isolated chunk of memory outside image of int8 type
        * ISO_INT8_BLOCK: output block, its a pointer to an isolated chunk of memory outside image of int8 type
        * BLOCK_DIM: block dimension, 8 for 8x8 blocks, 16 for 16x16 blocks, etc.
    */
    int i = 0, j = 0, t;
    bool up = true; // or false, doesnt matter
    for (t = 0; t < BLOCK_DIM * BLOCK_DIM ; t++) {
        INT8_SEQUENCE[t] = ISO_INT8_BLOCK[i * BLOCK_DIM + j];
        if (i == 0) {
            t++;
            if (j < BLOCK_DIM - 1) {
                j++;
            } else {
                i++;
            }
            INT8_SEQUENCE[t] = ISO_INT8_BLOCK[i * BLOCK_DIM + j];
            up = false;
        }
        else if (j == 0) {
            t++;
            if (i < BLOCK_DIM - 1) {
                i++;
            } else {
                j++;
            }
            INT8_SEQUENCE[t] = ISO_INT8_BLOCK[i * BLOCK_DIM + j];
            up = true;
        }
        else if (j == BLOCK_DIM-1)
        {
            t++; i++;
            INT8_SEQUENCE[t] = ISO_INT8_BLOCK[i * BLOCK_DIM + j];
            up = false;
        }
        else if (i == BLOCK_DIM-1)
        {
            t++; j++; 
            INT8_SEQUENCE[t] = ISO_INT8_BLOCK[i * BLOCK_DIM + j];
            up = true;
        }
        if (up) {
            i--; j++;
        } else {
            i++; j--;
        }
    }
}
// {iso_uint8_block} -> dct -> {iso_float_block} -> quantize -> {iso_int8_block} -> zigzag_reorder -> {int8_sequence} -> huffman -> {huffman}

