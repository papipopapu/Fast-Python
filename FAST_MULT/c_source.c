#include "cblas.h"
void MAT_MULT(double* L, double* R, double* ANS, int N1, int N2, int N3)
{
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N1, N3, N2, 1.0, L, N2, R, N3, 0.0, ANS, N3); 
}
double DOT(double* L, double* R, int N)
{
    return cblas_ddot(N, L, 1, R, 1);
}
int main(void)
{
    return 0;
}

