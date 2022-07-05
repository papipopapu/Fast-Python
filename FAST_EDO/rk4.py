from ctypes import CDLL, POINTER, c_int, c_double
from numpy import ndarray, float64, ctypeslib
import os
import matplotlib.pyplot as plt


EDO = CDLL(f"{os.path.dirname(__file__)}/shared.so")



def rk4(Y0, t0, dt, N):
    """
    Solves the EDO using the Runge-Kutta 4th order method.
    
        Parameters:
            Y(numpy.ndarray): initial conditions
            t0(float): initial time
            dt(float): time step
            N(int): number of time steps
        
        Returns:
            Y(numpy.ndarray): solution
    """
    nvar = Y0.size

    NPTR = ctypeslib.ndpointer(dtype = float64, ndim = nvar, flags = "C")
    EDO.RK4.argtypes = [NPTR, c_double, c_double, c_int, c_int]
    EDO.RK4.restype = None


    Y = ndarray(shape = (N, nvar), dtype = float64)
    Y[0, :] = Y0
    EDO.RK4(Y, t0, dt, nvar, N)
    return Y




