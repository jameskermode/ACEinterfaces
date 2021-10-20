import numpy as np
from ase.calculators.calculator import Calculator
from ase.constraints import voigt_6_to_full_3x3_stress, full_3x3_to_voigt_6_stress

from ctypes import c_void_p, c_double, c_int, cdll
from numpy.ctypeslib import ndpointer

class JulipCalculator(Calculator):
    """
    ASE-compatible Calculator that calls cace for forces and energy 
    """
    implemented_properties = ['forces', 'energy', 'free_energy', 'stress']
    default_parameters = {}
    name = 'JulipCalculator'

    def __init__(self, julip_calculator):
        Calculator.__init__(self)
        self.julip_calculator = 

    def calculate(self, atoms, properties, system_changes):
        Calculator.calculate(self, atoms, properties, system_changes)
        self.results = {}
        if 'energy' in properties:
            E = #energy(self.julip_calculator, julia_atoms)
            self.results['energy'] = E
            self.results['free_energy'] = E
        if 'forces' in properties:
            self.results['forces'] = #np.array(forces(self.julip_calculator, julia_atoms))
        if 'stress' in properties:
            voigt_stress = #full_3x3_to_voigt_6_stress(np.array(stress(self.julip_calculator, julia_atoms)))
            self.results['stress'] = voigt_stress

