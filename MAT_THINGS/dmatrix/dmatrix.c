#include "dmatrix.h"


void print_dmatrix(dmatrix *A) {
    int i, j;
    for (i = 0; i < A->n; i++) {
        for (j = 0; j < A->m; j++) {
            printf("%lf ", A->data[i * A->m + j]);
        }
        printf("\n");
    }
}
dmatrix *create_dmatrix(size_t n, size_t m) {
    dmatrix *A = (dmatrix *)malloc(sizeof(dmatrix));
    A->n = n;
    A->m = m;
    A->data = (double *)malloc(n * m * sizeof(double));
    return A;
}
dmatrix *from_arr(double *arr, size_t n, size_t m) {
    dmatrix *A = create_dmatrix(n, m);
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            A->data[i * m + j] = arr[i * m + j];
        }
    }
    return A;
}
dmatrix *from_copy(dmatrix *A) {
    dmatrix *B = create_dmatrix(A->n, A->m);
    memcpy(B->data, A->data, A->n * A->m * sizeof(double));
}

dmatrix *from_move(dmatrix *A) {
    dmatrix *B = create_dmatrix(A->n, A->m);
    memmove(B->data, A->data, A->n * A->m * sizeof(double));
}

void move_dmatrix(dmatrix *to, dmatrix *from) {
    to->data = from->data;
    to->n = from->n;
    to->m = from->m;
}
dmatrix *read_dmatrix(char *filename) {
    FILE *f = fopen(filename, "r");
    size_t n, m;
    fscanf(f, "%zu %zu", &n, &m);
    dmatrix *A = create_dmatrix(n, m);
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf(f, "%lf", &A->data[i * m + j]);
        }
    }
    fclose(f);
    return A;
}
void write_dmatrix(char *filename, dmatrix *A) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "%zu %zu\n", A->n, A->m);
    int i, j;
    for (i = 0; i < A->n; i++) {
        for (j = 0; j < A->m; j++) {
            fprintf(f, "%lf ", A->data[i * A->m + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void destroy_dmatrix(dmatrix *A) {
    free(A->data);
    free(A);
}
double *ix(dmatrix *A, int i, int j) {
    return A->data + i * A->m + j;
}
