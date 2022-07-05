from numpy import ndarray, ctypeslib, float64
from ctypes import CDLL, c_int, c_double
import os

NPTR2 = ctypeslib.ndpointer(dtype = float64, ndim = 2, flags = "C")
NPTR1 = ctypeslib.ndpointer(dtype = float64, ndim = 1, flags = "C")
PYSOURCE = CDLL(f"{os.path.dirname(__file__)}/shared.so")



def mult(L, R):
    """
    Multiplies two matrices very quickly.

        Parameters:
            L(numpy.ndarray): left matrix to mulitply
            R(numpy.ndarray): right matrix to mulitply

        Returns:
            ANS(numpy.ndarray): result matrix

    """
    PYSOURCE.MAT_MULT.argtypes = [NPTR2, NPTR2, NPTR2, c_int, c_int, c_int]
    PYSOURCE.MAT_MULT.restype = None

    N1 = L.shape[0] 
    N2 = R.shape[0]
    N3 = R.shape[1]
    ANS = ndarray((N1, N3), float64)
    PYSOURCE.MAT_MULT(L, R, ANS, N1, N2, N3)
    return ANS
def dot(L, R):
    """
    Get the dot product between two vectors very quickly.

        Parameters:
            L(numpy.ndarray): vector 
            R(numpy.ndarray): vector 
        Returns:
            ANS(float): result scalar

    """
    PYSOURCE.DOT.argtypes = [NPTR1, NPTR1, c_int]
    PYSOURCE.DOT.restype = c_double

    N = L.size
    ANS = PYSOURCE.DOT(L, R, N)
    return ANS



"gcc -c c_source.c"
"g++ -shared -o shared.so c_source.o -lblas"
