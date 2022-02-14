using ACE1, JuLIP

IP = read_dict( load_dict("/home/cdt1906/Documents/phd/ACE_dev/interfaces/ACEinterfaces/assets/CH_ACE1_test.json")["IP"])

X = [-1.62840152, -0.37075213,-0.00382227, 
-0.64824885, 0.40373766, 0.01092238,
0.74097139, 0.26398960, -0.21126373]
Z = [6, 6, 6]
cell = [50.0,  0.0,  0.0,  0.0, 50.0,  0.0,  0.0,  0.0, 50.0]
bc = [0, 0, 0]

GC.@preserve X Z cell bc at = Atoms(X = X, Z = Z, cell=cell, pbc = Bool.(bc))

println("#### STARTING ENERGY AND FORCE CALCULATIONS ####"); 
for i in 1:100000 
    GC.@preserve IP E = energy(IP, at)
    GC.@preserve IP F = forces(IP, at)
end
println("#### 100,000,000 energy and force evaluations without a problem ####");
