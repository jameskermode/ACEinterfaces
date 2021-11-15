
using ACE

##

# create random OH potential
B = ACE.Utils.rpi_basis(; species = [:H, :O], N = 3)
V = ACE.Random.randcombine(B; diff=2)
D = write_dict(V)
save_json("randpotHO.json", D)

D1 = load_json("randpotHO.json")
V1 = read_dict(D1)
@show V == V1
