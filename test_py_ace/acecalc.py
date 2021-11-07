
import numpy as np
from ase.calculators.calculator import Calculator
from ctypes import *
from numpy.ctypeslib import ndpointer


class ACECalculator(Calculator):
  """
  ASE compatible calculator of C-LJ potential
  """

  implemented_properties = ['forces', 'energy']
  
  default_parameters = {}
  
  name = 'ACECalculator'

  def __init__(self, 
               initcmd = None, 
               jsonpath = None, 
               calcid = "__ase_calc__"):
    Calculator.__init__(self)
    self.lib = cdll.LoadLibrary("./ace.so")

    self.ace_init = self.lib.ace_init 
    self.ace_init.restype = None
    self.ace_init.argtypes = [] 

    self.ace_cleanup = self.lib.ace_cleanup 
    self.ace_cleanup.restype = None
    self.ace_init.argtypes = [] 

    self.jl_eval_string = self.lib.jl_eval_string 
    self.jl_eval_string.restype = None
    self.jl_eval_string.argtypes = [c_char_p,]

    self.energy = self.lib.energy
    self.energy.restype = c_double
    self.energy.argtypes = [c_char_p,    # calculator id 
                      ndpointer(c_double, flags="C_CONTIGUOUS"),   # positions 
                      ndpointer(c_int32, flags="C_CONTIGUOUS"),    # Z
                      ndpointer(c_double, flags="C_CONTIGUOUS"),   # cell 
                      ndpointer(c_int32, flags="C_CONTIGUOUS"),    # pbc 
                      c_int ]

    self.forces = self.lib.forces
    self.forces.restype = None
    self.forces.argtypes = [c_char_p,    # calculator id 
                      ndpointer(c_double, flags="C_CONTIGUOUS"),   # forces 
                      ndpointer(c_double, flags="C_CONTIGUOUS"),   # positions 
                      ndpointer(c_int32, flags="C_CONTIGUOUS"),    # Z
                      ndpointer(c_double, flags="C_CONTIGUOUS"),   # cell 
                      ndpointer(c_int32, flags="C_CONTIGUOUS"),    # pbc 
                      c_int ]

    self.init_calc(calcid, initcmd, jsonpath)

  def _cstr(self, str):
    return create_string_buffer(str.encode('utf-8'))

  def julia_eval(self, str):
    self.jl_eval_string(self._cstr(str))

  def eval_energy(self, X, Z, cell, pbc):
    Nats = len(Z)
    E = self.energy(self.calcid_c, X.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
    return E 

  def eval_forces(self, calc_id, X, Z, cell, pbc):
    Nats = len(Z)
    Fs = np.zeros((Nats, 3)).flatten()
    self.forces(self.calcid_c, Fs, X.flatten(), Z.flatten(), cell.flatten(), pbc.flatten(), Nats)
    return Fs.reshape((Nats, 3))

  def init_calc(self, calcid, initcmd, jsonpath):
    self.calcid = calcid 
    self.calcid_c = self._cstr(calcid)
    if initcmd != None and jsonpath == None: 
      cmd = calcid + " = " + initcmd
    elif jsonpath != None and initcmd == None: 
      cmd = calcid + " = read_dict( load_json(\"" + jsonpath + "\"))"
    else:
      print("exactly one of jsonpath and initcmd must be provided")
      # TODO: throw an exception 
    print("Loading potential: " + cmd)
    self.julia_eval(cmd)

  
  def calculate(self, atoms, properties, system_changes):
    Calculator.calculate(self, atoms, properties, system_changes)
    X = atoms.get_positions()
    cell = atoms.get_cell()
    Z = atoms.get_atomic_numbers().astype("int32")
    pbc = atoms.get_pbc().astype("int32")
    Nats = len(atoms)
    self.results = {}
    if 'energy' in properties:
      self.results["energy"] = self.eval_energy(positions, Z, cell, pbc, Nats)
    if 'forces' in properties:
      self.results['forces'] = self.eval_forces(positions, Z, cell, pbc, Nats)
