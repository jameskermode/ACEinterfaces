#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <julia.h>

#include "ace_c.h"

void println() { printf("\n"); }

jl_function_t* _atoms_from_c; 
jl_value_t* _energyfcn;
jl_value_t *_forcefcn;
jl_value_t *_stressfcn;


void ace_init() {
   jl_init();
   jl_eval_string("using Pkg; Pkg.activate(\".\"); Pkg.instantiate(; verbose=true);");
   jl_eval_string("using JuLIP, ACE");
   _atoms_from_c = jl_eval_string("(X, Z, cell, bc) -> Atoms(X = X, Z = Z, cell=cell, pbc = Bool.(bc))");
   _energyfcn = (jl_value_t*)jl_get_function(jl_main_module, "energy");
   _forcefcn = (jl_value_t*)jl_eval_string("(calc, at) -> mat(forces(calc, at))[:]");
   _stressfcn = (jl_value_t*)jl_eval_string("(calc, at) -> vcat(stress(calc, at)...)");
   return; 
}

void ace_cleanup() {
  jl_atexit_hook(0);
  return; 
}



jl_value_t* atoms_from_c(double* X, 
                         int32_t* Z, 
                         double* cell, 
                         int32_t* bc, 
                         int Nat) {

   jl_value_t* jl_float64 = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
   jl_value_t* jl_int32 = jl_apply_array_type((jl_value_t*)jl_int32_type, 1);
   jl_array_t* _X = jl_alloc_array_1d(jl_float64, 3 * Nat);
   jl_array_t* _cell = jl_alloc_array_1d(jl_float64, 9);
   jl_array_t* _bc = jl_alloc_array_1d(jl_int32, 3);
   jl_array_t* _Z = jl_alloc_array_1d(jl_int32, Nat);

   double *XData = (double*)jl_array_data(_X);
   for (int i = 0; i < 3*Nat; i++) XData[i] = X[i];
   double *cellData = (double*)jl_array_data(_cell);
   for (int i = 0; i < 9; i++) cellData[i] = cell[i]; 
   int32_t *bcData = (int32_t*)jl_array_data(_bc);
   for (int i = 0; i < 3; i++) bcData[i] = bc[i]; 
   int32_t *ZData = (int32_t*)jl_array_data(_Z);
   for (int i = 0; i < Nat; i++) ZData[i] = Z[i]; 

   jl_value_t* at_args[] = {(jl_value_t*)_X, (jl_value_t*)_Z, (jl_value_t*)_cell, (jl_value_t*)_bc};   
   return jl_call(_atoms_from_c, at_args, 4);
}


double unbox_float64(jl_value_t* jlval) {
   if jl_typeis(jlval, jl_float64_type) {
      return jl_unbox_float64(jlval);
   } 
   jl_errorf("Can't identify the return type!!! \n");
   return 0.0;
}

double energy(char* calcid, double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){

   jl_value_t** args; 
   JL_GC_PUSHARGS(args, 2);

   jl_value_t* calc = jl_eval_string(calcid);
   args[0] = calc; 

   jl_value_t* at = atoms_from_c(X, Z, cell, pbc, Nat);
   args[1] = at;

   jl_set_global(jl_main_module, jl_symbol("at"), at);

   double E = 0.0; 
   jl_value_t* jlE;
   jlE = jl_call2(_energyfcn, calc, at);   
   if (jl_exception_occurred()) {
      printf("Exception at jl_call2(_energyfcn, calc, at) : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
      jl_errorf("Something when wrong calling the energy\n");
   } else {
      E = unbox_float64(jlE); 
   }

   JL_GC_POP();

   return E;
}



void forces(char* calcid, double *F, 
              double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){
  
   jl_value_t** args; 
   JL_GC_PUSHARGS(args, 2);

   jl_value_t* calc = jl_eval_string(calcid);
   args[0] = calc; 
   jl_value_t* at = atoms_from_c(X, Z, cell, pbc, Nat);
   args[1] = at;

   jl_array_t* _F = (jl_array_t*)jl_call2(_forcefcn, calc, at);
   double *Fdata = (double*)jl_array_data(_F);
   for (int i = 0; i < 3*Nat; i++) F[i] = Fdata[i];

   JL_GC_POP();

   return; 
}


void stress(char* calcid, double *S, 
              double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){
  
   jl_value_t** args; 
   JL_GC_PUSHARGS(args, 2);

   jl_value_t* calc = jl_eval_string(calcid);
   args[0] = calc; 
   jl_value_t* at = atoms_from_c(X, Z, cell, pbc, Nat);
   args[1] = at;

   jl_array_t* _S = (jl_array_t*)jl_call2(_stressfcn, calc, at);
   double *Sdata = (double*)jl_array_data(_S);
   for (int i = 0; i < 9; i++) S[i] = Sdata[i];

   JL_GC_POP();

   return; 
}