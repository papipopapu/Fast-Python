
#include "dmatrix.h"

void rotate_dmatrix(dmatrix *A, int im, int jm, double cosv, double sinv){ // must be square dmatrix, (im, jm) is the position of -sin
    double temp;
    int k;
    for (k=0; k<A->m; k++) {
        temp          = *ix(A, im, k) * cosv - *ix(A, jm, k) * sinv;
        *ix(A, jm, k) = *ix(A, im, k) * sinv + *ix(A, jm, k) * cosv;

        *ix(A, im, k) = temp;
    }
}


void block_transpose_dmatrix(dmatrix *A, int block_size) {
    int i, j, k, l, n = A->n, m = A->m;
    dmatrix *temp = create_dmatrix(m, n);
    for (i = 0; i < n; i += block_size) {
        for (j = 0; j < m; j += block_size) {
            // transpose the block beginning at [i,j]
            for (k = i; k < i + block_size && k < n; k++) {
                for (l = j; l < j + block_size && l < m; l++) {
                    *ix(temp, l, k) = *ix(A, k, l);
                }
            }
        }
    }
    free(A->data);
    move_dmatrix(A, temp);
}

void rotate_dmatrix_transpose(dmatrix *A, int im, int jm, double cosv, double sinv) // this does R*E^t
{
    double temp;
    int k;
    for (k=0; k<A->m; k++) {
        temp          = *ix(A, k, im) * cosv - *ix(A, k, jm) * sinv;
        *ix(A, jm, k) = *ix(A, k, im) * sinv + *ix(A, k, jm) * cosv;

        *ix(A, im, k) = temp;
    }
}
    

void eigen_step(dmatrix *E, int im, int jm, double cosv, double sinv) {
    rotate_dmatrix(E, im, jm, cosv, sinv);
    rotate_dmatrix_transpose(E, im, jm, cosv, sinv);
    block_transpose_dmatrix(E, 16);
}

void max_nondiag(dmatrix *A, int *im, int *jm) {
    int i, j, max = 0;
    for (i=0; i<A->n; i++) {
        for (j=0; j<A->m; j++) {
            if (i != j) {
                if (fabs(*ix(A, i, j)) > max) {
                    max = fabs(*ix(A, i, j));
                    *im = i;
                    *jm = j;
                }
            }
        }
    }
}
