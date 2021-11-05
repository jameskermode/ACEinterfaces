import numpy as np
from ase.calculators.calculator import Calculator
from ctypes import *
from numpy.ctypeslib import ndpointer


class CACECalculator(Calculator):
  """
  ASE compatible calculator of C-LJ potential
  """
  implemented_properties = ['forces', 'energy']
  default_parameters = {}
  name = 'CACECalculator'
  def __init__(self):
    Calculator.__init__(self)
    lib = cdll.LoadLibrary("./lj.so")
    self.energy = lib.energy
    self.energy.restype = c_double
    self.energy.argtypes = [ndpointer(c_double, flags="C_CONTIGUOUS"), c_int]
    self.forces = lib.forces
    self.forces.restype = None
    self.forces.argtypes = [ndpointer(c_double, flags="C_CONTIGUOUS"), c_int,
                    ndpointer(c_double, flags="C_CONTIGUOUS")]
  
  def calculate(self, atoms, properties, system_changes):
    Calculator.calculate(self, atoms, properties, system_changes)
    positions = atoms.get_positions().flatten()
    Nats = len(atoms.get_chemical_symbols())
    self.results = {}
    if 'energy' in properties:
      E = self.energy(positions, Nats)
      self.results["energy"] = E
    if 'forces' in properties:
      Fs = np.empty((Nats, 3)).flatten()
      self.forces(positions, Nats, Fs)
      self.results['forces'] = Fs.reshape((Nats, 3))