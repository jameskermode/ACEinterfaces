import ase
#from ase.build import molecule
from ase.io import read
import numpy as np 
import ace

#water = molecule("H2O")

# need to fix the cell. 
# We should do this automated, inside Julia.
#X = water.get_positions() + 1
#water.set_positions(X)
at = read("../assets/ch_test2.xyz")
#at.set_cell(2 * np.ones(3))

cace_calc = ace.ACECalculator(jsonpath="../assets/CH_ace_test.json")

at.set_calculator(cace_calc)

print("Positions of the atoms")
print(at.get_positions())

print("TEST energy call")
print(at.get_potential_energy())

print("TEST force call")
print(at.get_forces())

#water = water * [2, 2, 2]
at.pbc = [True, True, True]
#water.set_calculator(cace_calc)
print("TEST stress call")
print(at.get_stress())