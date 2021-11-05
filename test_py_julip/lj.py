from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np
from time import time

lib = cdll.LoadLibrary("./lj.so")

julip_init = lib.julip_init 
julip_init.restype = None
julip_init.argtypes = [] 

julip_cleanup = lib.julip_cleanup 
julip_cleanup.restype = None
julip_init.argtypes = [] 

julip_init_lj = lib.init_lj 
julip_cleanup.restype = c_void_p
julip_init.argtypes = [] 

energy = lib.energy
energy.restype = c_double
energy.argtypes = [c_void_p, 
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # positions 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # Z
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # cell 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # pbc 
                   c_int ]

# forces = lib.forces
# forces.restype = None
# forces.argtypes = [ndpointer(c_double, flags="C_CONTIGUOUS"), c_int,
#                     ndpointer(c_double, flags="C_CONTIGUOUS")]

julip_init()

# Julip calculator 
calc = julip_init_lj()


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
Nats = 5
positions = np.random.uniform(-5, 5, (Nats, 3))
Z = np.zeros(Nats, dtype = "int32")
pbc = np.zeros(3, dtype = "int32")
cell = np.zeros(9)
cell[0] = 5.0
cell[4] = 5.0
cell[8] = 5.0

C_E_warmup = energy(calc, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)

# print(positions)
# print(positions.flatten())
t0 = time()
pyE = py_energy(positions, Nats)
t1 = time()
C_E = energy(calc, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
t2 = time()

print("Energy Python: {0:.3f} in {1:.3f} seconds".format( pyE, t1 - t0))
print("Energy JuLIP: {0:.3f} in {1:.3f} seconds".format( C_E, t2 - t1))

# ### TEST FORCES
# np.random.seed(124)
# Nats = 2
# positions = np.random.uniform(-100, 100, (Nats, 3))
# forces_result = np.empty((Nats, 3)).flatten()
# forces(positions, Nats, forces_result)
# print(forces_result.reshape((Nats, 3)))



julip_cleanup()
