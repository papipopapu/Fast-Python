#ifndef DMATRIX_H
#define DMATRIX_H

// dmatrix.h contains:
    // includes of libraries like math.h
    // protoypes of dmatrix.c (which contains the basic functionality)
// things to note:
    // in theory, maybe dmatrix.h should only include the prototypes of dmatrix.c, and leave the common includes to some includes.h
    // to use dmatrix things, you need to:
        // include path/dmatrix.h
        // compile with the adecuate .c object files (e.j. path/dmatrix.c)
        // add the necessary flags for external things (e.j. -lm)

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct dmatrix {
    size_t n;
    size_t m;
    double *data;
} dmatrix;

void print_dmatrix(dmatrix *A);
dmatrix *create_dmatrix(size_t n, size_t m);
dmatrix *from_arr(double *arr, size_t n, size_t m);
dmatrix *from_copy(dmatrix *A);
dmatrix *from_move(dmatrix *A);
void move_dmatrix(dmatrix *to, dmatrix *from);
dmatrix *read_dmatrix(char *filename);
void write_dmatrix(char *filename, dmatrix *A);
void destroy_dmatrix(dmatrix *A);
double *ix(dmatrix *A, int i, int j);

#endif