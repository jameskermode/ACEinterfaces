from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np
from time import time

lib = cdll.LoadLibrary("./morse.so")
energy = lib.energy
energy.restype = c_double
energy.argtypes = [ndpointer(c_double, flags="C_CONTIGUOUS"), c_int]

forces = lib.forces
forces.restype = None
forces.argtypes = [ndpointer(c_double, flags="C_CONTIGUOUS"), c_int,
                    ndpointer(c_double, flags="C_CONTIGUOUS")]

def pyLJ(r, eps, sigm):
  r_sc = (sigm / r)**6
  return 4 * eps * (r_sc**2 - r_sc)

def py_energy(positions, Nats):
  Energy = 0
  for i in range(Nats - 1):
    for j in range(i+1, Nats):
      D = np.sqrt(np.sum(np.square(positions[i] - positions[j])))
      #print("Python distance: ", D)
      Energy += pyLJ(D, 1, 1)
      #print("Python energy: ", Energy)
  return Energy

### TEST ENERGIES
np.random.seed(124)
Nats = 20
positions = np.random.uniform(-100, 100, (Nats, 3))
#print(positions)
#print(positions.flatten())
t0 = time()
pyE = py_energy(positions, Nats)
t1 = time()
C_E = energy(positions.flatten(), Nats)
t2 = time()
print("Energy Python: {0:.1f} in {1:.1f} seconds".format( pyE, t1 - t0))
print("Energy C: {0:.3f} in {1:.3f} seconds".format( C_E, t2 - t1))

### TEST FORCES
np.random.seed(124)
Nats = 2
positions = np.random.uniform(-100, 100, (Nats, 3))
forces_result = np.empty((Nats, 3)).flatten()
forces(positions, Nats, forces_result)
print(forces_result.reshape((Nats, 3)))


