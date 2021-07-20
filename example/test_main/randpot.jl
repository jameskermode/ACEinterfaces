
using ACE 

# create random potential
B = ACE.Utils.rpi_basis(; species = :Si, N = 3)
V = ACE.Random.randcombine(B; diff=2)
D = write_dict(V)
save_json("randpot.json", D)

D1 = load_json("randpot.json")
V1 = read_dict(D1)
@show V == V1
