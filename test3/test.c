  
#include <julia.h>
#include <stdio.h>

// JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

void println() { printf("\n"); }

jl_function_t *_atoms_from_c; 

void julip_init() {
   jl_eval_string("using Pkg; Pkg.activate(\".\"); Pkg.instantiate(; verbose=true);");
   jl_eval_string("using JuLIP");
   _atoms_from_c = jl_eval_string("(X, Z, cell, bc) -> Atoms(X = X, Z = Z, cell=cell, pbc = Bool.(bc))");
}

void ace_init() {
   jl_eval_string("using ACE, JuLIP");
}

jl_value_t* atoms_from_c(jl_value_t* X, 
                  jl_value_t* Z, 
                  jl_value_t* cell, 
                  jl_value_t* bc) {

   jl_value_t* at_args[] = {X, Z, cell, bc};   
   return jl_call(_atoms_from_c, at_args, 4);
}

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();
    julip_init();
    ace_init();
    
   /* load a potential */ 
    jl_eval_string("D = load_json(\"randpot.json\"); V = read_dict(D)");

   /* create random data defining structure */ 
   double cell[] = {5.0, 2.0, 0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 3.0};
   short bc[] = {1, 0, 1};
   double positions[] = { 0.0, 0.0, 0.0, 
                        3.0, 0.2, 0.5, 
                        0.1, 2.5, 2.8 };
   long Z[] = {28, 28, 8};

   /* create the JuLIP structure */ 
   int Nat = 3;
   jl_value_t* jl_float64 = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
   jl_value_t* jl_int32 = jl_apply_array_type((jl_value_t*)jl_int32_type, 1);

   jl_array_t* _X = jl_alloc_array_1d(jl_float64, 3 * Nat);
   jl_array_t* _cell = jl_alloc_array_1d(jl_float64, 9);
   jl_array_t* _bc = jl_alloc_array_1d(jl_int32, 3);
   jl_array_t* _Z = jl_alloc_array_1d(jl_int32, Nat);

   double *XData = (double*)jl_array_data(_X);
   for (int i = 0; i < 3*Nat; i++) XData[i] = positions[i];    // memcpy

   double *cellData = (double*)jl_array_data(_cell);
   for (int i = 0; i < 9; i++) cellData[i] = cell[i]; 

   int32_t *bcData = (int32_t*)jl_array_data(_bc);
   for (int i = 0; i < 3; i++) bcData[i] = bc[i]; 

   int32_t *ZData = (int32_t*)jl_array_data(_Z);
   for (int i = 0; i < Nat; i++) ZData[i] = Z[i]; 
   
   jl_value_t **args;
   JL_GC_PUSHARGS(args, 5); // args can now hold 5 `jl_value_t*` objects
   args[0] = (jl_value_t*)_X;
   args[1] = (jl_value_t*)_cell;
   args[2] = (jl_value_t*)_bc;
   args[3] = (jl_value_t*)_Z;

   jl_function_t *display = jl_get_function(jl_base_module, "display");

   jl_value_t *_at  = atoms_from_c((jl_value_t*)_X, (jl_value_t*)_Z, (jl_value_t*)_cell, (jl_value_t*)_bc);
   args[4] = _at; 
   jl_call1(display, _at); println();

   JL_GC_POP();

   jl_atexit_hook(0);
   return 0;
}
