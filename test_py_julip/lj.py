
from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np
from time import time

def _cstr(str):
  return create_string_buffer(str.encode('utf-8'))

lib = cdll.LoadLibrary("./lj.so")

julip_init = lib.julip_init 
julip_init.restype = None
julip_init.argtypes = [] 

julip_cleanup = lib.julip_cleanup 
julip_cleanup.restype = None
julip_init.argtypes = [] 

julip_init_lj = lib.init_lj 
julip_init_lj.restype = c_void_p
julip_init_lj.argtypes = [c_char_p, ] 

julip_init_calculator = lib.init_calculator
julip_init_calculator.restype = c_void_p
julip_init_calculator.argtypes = [c_char_p, c_char_p] 


energy = lib.energy
energy.restype = c_double
energy.argtypes = [c_char_p,    # calculator id 
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # positions 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # Z
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # cell 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # pbc 
                   c_int ]

forces = lib.forces
forces.restype = None
forces.argtypes = [c_char_p,    # calculator id 
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # forces 
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # positions 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # Z
                   ndpointer(c_double, flags="C_CONTIGUOUS"),   # cell 
                   ndpointer(c_int32, flags="C_CONTIGUOUS"),    # pbc 
                   c_int ]

julip_init()

# Julip calculator 
ljid = "cace_ljcalc"
ljid_c = _cstr(ljid)

# julip_init_lj(ljid_c)
julip_init_calculator(ljid_c, _cstr("LennardJones() * SplineCutoff(15.0, 20.0)"))

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

# WARMUP 

# warmup 
energy(ljid_c, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
F = np.empty((Nats, 3)).flatten()
forces(ljid_c, F, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)



# print(positions)
# print(positions.flatten())
t0 = time()
pyE = py_energy(positions, Nats)
t1 = time()
C_E = energy(ljid_c, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
t2 = time()

print("Energy Python: {0:.3f} in {1:.3f} seconds".format( pyE, t1 - t0))
print("Energy JuLIP: {0:.3f} in {1:.3f} seconds".format( C_E, t2 - t1))

### TEST FORCES
forces(ljid_c, F, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
print(F.reshape((Nats, 3)))


julip_cleanup()
