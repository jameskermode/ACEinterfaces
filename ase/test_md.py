from pathlib import Path

from ase.io import read
import ace
from ase.md.verlet import VelocityVerlet
from ase.units import fs
from ase.md.velocitydistribution import MaxwellBoltzmannDistribution

at = read(Path(__file__).parent / "../assets/ch_test.xyz")

cace_calc = ace.ACECalculator(jsonpath=Path(__file__).parent / "../assets/CH_ACE1_large.json", ACE_version=1)

at.set_calculator(cace_calc)
MaxwellBoltzmannDistribution(at, temperature_K=100)

print("running 200,000 step MD")
dyn = VelocityVerlet(at, timestep=1 * fs, logfile='-', loginterval=100)
dyn.run(200000)
print("Done 200,000 step MD without problem")
