from ase.io import read
import ace

at = read("../assets/ch_test2.xyz")

cace_calc = ace.ACECalculator(jsonpath="../assets/CH_ace_test.json", ACE_version=1)

at.set_calculator(cace_calc)

print("Positions of the atoms")
print(at.get_positions())

print("TEST energy call")
print(at.get_potential_energy())

print("TEST force call")
print(at.get_forces())

at.pbc = [True, True, True]
print("TEST stress call")
print(at.get_stress())