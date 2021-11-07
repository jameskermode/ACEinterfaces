
from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np
from time import time
import random 

def _cstr(str):
  return create_string_buffer(str.encode('utf-8'))

lib = cdll.LoadLibrary("./ace.so")

ace_init = lib.ace_init 
ace_init.restype = None
ace_init.argtypes = [] 

ace_cleanup = lib.ace_cleanup 
ace_cleanup.restype = None
ace_init.argtypes = [] 

jl_eval_string = lib.jl_eval_string 
jl_eval_string.restype = None
jl_eval_string.argtypes = [c_char_p,]

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

def _cstr(str):
  return create_string_buffer(str.encode('utf-8'))

def julia_eval(str):
  jl_eval_string(_cstr(str))
  

def eval_energy(calc_id, X, Z, cell, pbc):
  Nats = len(Z)
  E = energy(_cstr(calc_id), X.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
  return E 

def eval_forces(calc_id, X, Z, cell, pbc):
  Nats = len(Z)
  Fs = np.zeros((Nats, 3)).flatten()
  forces(_cstr(calc_id), Fs, X.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
  return Fs.reshape((Nats, 3))

def init_calc(calcid, initcmd = None, jsonpath = None):
  if initcmd != None and jsonpath == None: 
    cmd = calcid + " = " + initcmd
  elif jsonpath != None and initcmd == None: 
    cmd = calcid + " = read_dict( load_json(\"" + jsonpath + "\"))"
  else:
    print("exactly one of jsonpath and initcmd must be provided")
    # TODO: throw an exception 
  print("Loading potential: " + cmd)
  julia_eval(cmd)


ace_init()

# Julip calculator : Lennard Jones 
init_calc("cace_lj", initcmd = "LennardJones() * SplineCutoff(15.0, 20.0)")


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
eval_energy("cace_lj", positions, Z, cell, pbc)
F = eval_forces("cace_lj", positions, Z, cell, pbc)

t0 = time()
pyE = py_energy(positions, Nats)
t1 = time()
C_E = eval_energy("cace_lj", positions, Z, cell, pbc)
t2 = time()

print("Energy Python: {0:.3f} in {1:.3f} seconds".format( pyE, t1 - t0))
print("Energy JuLIP: {0:.3f} in {1:.3f} seconds".format( C_E, t2 - t1))

### TEST FORCES
F = eval_forces("cace_lj", positions, Z, cell, pbc)
print(F)

##### TESTING LOADING AND EVALUATING OF AN ACE POTENTIAL 
# Julip calculator : ACE 
aceid = "cace_ace"
init_calc(aceid, jsonpath = "randpotHO.json")

# convert the species to H and O 
for i in range(0, Nats):
  if random.random() < 0.5:
    Z[i] = 1
  else:
    Z[i] = 8

Eace = eval_energy(aceid, positions, Z, cell, pbc)
print("ACE Energy: ", Eace)

Face = eval_forces(aceid, positions, Z, cell, pbc)
print("ACE Forces: ", Face)

ace_cleanup()
