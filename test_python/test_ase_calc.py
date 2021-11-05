import ase
from ase.build import molecule
import ACE_ASE

water = molecule("H2O")

cace_calc = ACE_ASE.CACECalculator()

water.set_calculator(cace_calc)

print("Positions of the atoms")
print(water.get_positions())

print("TEST energy call")
print(water.get_potential_energy())

print("TEST force call")
print(water.get_forces())