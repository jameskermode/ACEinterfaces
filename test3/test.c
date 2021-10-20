  
#include <julia.h>
#include <stdio.h>

JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();
    jl_eval_string("using Pkg; Pkg.activate(\".\"); Pkg.instantiate(; verbose=true);");

    /* run Julia commands */
    jl_eval_string("using ACE, JuLIP");

   /* load a potential */ 
    jl_eval_string("D = load_json(\"randpot.json\"); V = read_dict(D)");

   /* create random data defining structure */ 
   double cell[] = {5.0, 2.0, 0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 3.0};
   short bc[] = {1, 0, 1};
   double positions[] = { 0.0, 0.0, 0.0, 
                        3.0, 0.2, 0.5, 
                        0.1, 2.5, 2.8 };
   int Z[] = {28, 28, 8};

   /* create the JuLIP structure */ 
   int Nat = 3;
   jl_value_t* jl_float64 = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
   jl_value_t* jl_int = jl_apply_array_type((jl_value_t*)jl_int64_type, 1);

   jl_array_t* _X = jl_alloc_array_1d(jl_float64, 3 * Nat);
   jl_array_t* _cell = jl_alloc_array_1d(jl_float64, 9);
   jl_array_t* _Z = jl_alloc_array_1d(jl_int, Nat);
   jl_array_t* _bc = jl_alloc_array_1d(jl_int, 3);

   double *XData = (double*)jl_array_data(_X);
   for (int i = 0; i < 3*Nat; i++) XData[i] = positions[i];    // memcpy

   double *ZData = (double*)jl_array_data(_Z);
   for (int i = 0; i < Nat; i++) ZData[i] = Z[i]; 

   double *cellData = (double*)jl_array_data(_cell);
   for (int i = 0; i < 9; i++) cellData[i] = cell[i]; 

   double *bcData = (double*)jl_array_data(_bc);
   for (int i = 0; i < 3; i++) bcData[i] = bc[i]; 

   jl_function_t *_atoms_from_c = jl_eval_string("(X, Z, bc) -> Atoms(X = X, Z = Z, pbc = bc)");


   jl_function_t *display = jl_get_function(jl_base_module, "display");
   jl_call1(display, (jl_value_t*)_X);   
   jl_value_t *_at = jl_call3(_atoms_from_c, (jl_value_t*)_X, (jl_value_t*)_Z, (jl_value_t*)_bc);
   jl_call1(display, (jl_value_t*)_at);

   jl_function_t *_set_cell = jl_eval_string("(at, cell) -> at.set_cell(cell)");
   jl_value_t *_at_wcell = jl_call2(_set_cell, (jl_value_t*)_at, (jl_value_t*)_cell);
   jl_call1(display, (jl_value_t*)_at_wcell);

  
   // jl_eval_string("D = load_json(\"randpot.json\"); V = read_dict(D)");


   // jl_eval_string("_Z = ");
   // jl_eval_string("@show Z");
   // // etc
   // jl_eval_string("at = Atoms(X = _X, Z = _Z, pbc = _bc, cell = _cell)")

   // /* evaluate the energy on the structure */ 
   // jl_value_t *jl_E = jl_eval_string("energy(V, at)");
   // if jl_typeis(jl_E, jl_float64_type) {
   //    double E = jl_unbox_float64(jl_E);
   //    printf("E = %f \n", E);
   // } else {
   //    printf("Can't identify the return type!!!");
   // }

    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);
    return 0;
}
