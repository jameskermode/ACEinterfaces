
from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np
from time import time
import random 

def _cstr(str):
  return create_string_buffer(str.encode('utf-8'))

lib = cdll.LoadLibrary("./lj.so")

julip_init = lib.julip_init 
julip_init.restype = None
julip_init.argtypes = [] 

julip_cleanup = lib.julip_cleanup 
julip_cleanup.restype = None
julip_init.argtypes = [] 

jl_eval_string = lib.jl_eval_string 
jl_eval_string.restype = None
jl_eval_string.argtypes = [c_char_p,]

julip_init_lj = lib.init_lj 
julip_init_lj.restype = c_void_p
julip_init_lj.argtypes = [c_char_p, ] 

julip_init_calculator = lib.init_calculator
julip_init_calculator.restype = c_void_p
julip_init_calculator.argtypes = [c_char_p, c_char_p] 

julip_json_calculator = lib.json_calculator
julip_json_calculator.restype = c_void_p
julip_json_calculator.argtypes = [c_char_p, c_char_p]


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

# Julip calculator : Lennard Jones 
ljid = "cace_ljcalc"
ljid_c = _cstr(ljid)
julip_init_calculator(ljid_c, _cstr("LennardJones() * SplineCutoff(15.0, 20.0)"))


# Python calculator 
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
cell[0] = 10.0
cell[4] = 10.0
cell[8] = 10.0

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

##### TESTING LOADING AND EVALUATING OF AN ACE POTENTIAL 
# Julip calculator : ACE 
jl_eval_string(_cstr("using ACE"))
aceid = "cace_ace"
aceid_c = _cstr(aceid)
julip_json_calculator(aceid_c, _cstr("randpotHO.json"))

# convert the species to H and O 
for i in range(0, Nats):
  if random.random() < 0.5:
    Z[i] = 1
  else:
    Z[i] = 8

Eace = energy(aceid_c, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
print("ACE Energy: ", Eace)

Face = forces(aceid_c, F, positions.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
print("ACE Forces: ", F)

julip_cleanup()
