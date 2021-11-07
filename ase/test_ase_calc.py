import ase
from ase.build import molecule
import numpy as np 
import ace

water = molecule("H2O")

# need to fix the cell. 
# We should do this automated, inside Julia.
X = water.get_positions() + 1
water.set_positions(X)
water.set_cell(2 * np.ones(3))

cace_calc = ace.ACECalculator(jsonpath="../assets/randpotHO.json")

water.set_calculator(cace_calc)

print("Positions of the atoms")
print(water.get_positions())

print("TEST energy call")
print(water.get_potential_energy())

print("TEST force call")
print(water.get_forces())

