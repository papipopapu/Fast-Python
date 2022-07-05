#include <stdio.h>
#include <stdlib.h>
// gcc -shared -o libthing.so thing.c

// or...

// gcc -c thing.c 
// gcc -shared -o libthing.so thing.o

void df(double *y, double *dy, double t)
{
  
    dy[0] = y[1];
    dy[1] = -10 * y[0];
    // yNext[i] = f( yPrev[i], t, dt) ... write df function here
    
}




void stepRK4(double *y0, double *yf, double t, double dt, int nVar)
{
    int j;
    double K1[nVar], K2[nVar], K3[nVar], K4[nVar], temp[nVar];// same minus the "* tau"
 
                                                                   df(y0,   K1, t           );
    for (j=0; j<nVar; j++) { temp[j] = y0[j] + dt * 0.5 * K1[j]; } df(temp, K2, t + dt * 0.5); 
    for (j=0; j<nVar; j++) { temp[j] = y0[j] + dt * 0.5 * K2[j]; } df(temp, K3, t + dt * 0.5); 
    for (j=0; j<nVar; j++) { temp[j] = y0[j] + dt *       K3[j]; } df(temp, K4, t + dt      ); 


    for (j=0; j<nVar; j++) { yf[j] = y0[j] + dt/6 * (K1[j] + 2 * K2[j] + 2 * K3[j] + K4[j]); } 

}
void RK4(double *Y, double t0, double dt, const int nVar, const int N)
{
  double t = t0;
  for(int i=nVar; i<N*nVar; i += nVar)
  {
    stepRK4(&Y[i-nVar], &Y[i], t, dt, nVar);
    t += dt;
  }
}


int main()
{
}