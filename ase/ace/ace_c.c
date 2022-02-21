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

int ace_init(int32_t ACE_version) {
   jl_init();
   // jl_eval_string("using Pkg; Pkg.activate(\"../.\"); Pkg.instantiate(; verbose=true);");
   if (ACE_version == 1)
   {
      jl_eval_string("using JuLIP, ACE1");
      jl_value_t *ret1 = jl_eval_string("JuLIP");
      jl_value_t *ret2 = jl_eval_string("ACE1");
      if (!jl_typeis(ret1, jl_module_type)) {
         fprintf(stderr, "ERROR: not a module! JuLIP \n");
      }
      if (!jl_typeis(ret2, jl_module_type)) {
         fprintf(stderr, "ERROR: not a module! ACE1 \n");
      }
      
   }else if (ACE_version == 2)
   {
      jl_eval_string("using JuLIP, ACE");
      jl_value_t *ret1 = jl_eval_string("JuLIP");
      jl_value_t *ret2 = jl_eval_string("ACE");
      if (!jl_typeis(ret1, jl_module_type)) {
         fprintf(stderr, "ERROR: not a module! JuLIP \n");
      }
      if (!jl_typeis(ret2, jl_module_type)) {
         fprintf(stderr, "ERROR: not a module! ACE \n");
      }
   }else 
      return 1;

   if (jl_exception_occurred()) {
      printf("Exception at initialization: %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   _atoms_from_c = jl_eval_string("(X, Z, cell, bc) -> Atoms(X = X, Z = Z, cell=cell, pbc = Bool.(bc))");
   _energyfcn = (jl_value_t*)jl_get_function(jl_main_module, "energy");
   _forcefcn = (jl_value_t*)jl_eval_string("(calc, at) -> mat(forces(calc, at))[:]");
   _stressfcn = (jl_value_t*)jl_eval_string("(calc, at) -> vcat(stress(calc, at)...)");

   jl_value_t* refs0 = jl_eval_string("refs0 = IdDict()");
   jl_function_t* setindex0 = jl_get_function(jl_base_module, "setindex!");
   jl_datatype_t* reft0 = (jl_datatype_t*)jl_eval_string("Base.RefValue{Any}");

   //jl_value_t* calc_root = jl_eval_string(calcid);
   JL_GC_PUSH4(&_atoms_from_c, &_energyfcn, &_forcefcn, &_stressfcn);
   jl_value_t* r_calc_root0 = jl_new_struct(reft0, _atoms_from_c);
   jl_value_t* r_calc_root1 = jl_new_struct(reft0, _energyfcn);
   jl_value_t* r_calc_root2 = jl_new_struct(reft0, _forcefcn);
   jl_value_t* r_calc_root3 = jl_new_struct(reft0, _stressfcn);
   JL_GC_POP();
   if (jl_exception_occurred()) {
      printf("Exception at rooting _atoms_from_c : %s \n", 
            jl_typeof_str(jl_exception_occurred()));
   }
   jl_call3(setindex0, refs0, r_calc_root0, r_calc_root0);
   jl_call3(setindex0, refs0, r_calc_root1, r_calc_root1);
   jl_call3(setindex0, refs0, r_calc_root2, r_calc_root2);
   jl_call3(setindex0, refs0, r_calc_root3, r_calc_root3);
   
   //_energyfcn = (jl_value_t*)jl_eval_string("GC.@preserve IP at X Z cell bc (calc, at) -> energy(calc, at)");
   //_forcefcn = (jl_value_t*)jl_eval_string("GC.@preserve IP at X Z cell bc (calc, at) -> mat(forces(calc, at))[:]");
   //_stressfcn = (jl_value_t*)jl_eval_string("GC.@preserve IP at X Z cell bc (calc, at) -> vcat(stress(calc, at)...)");
   if (jl_exception_occurred()) {
      printf("Exception at defining _atoms_from_c, _energy, _force etc. : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }
   return 0; 
}

void set_global(char* calcid){
  jl_value_t* refs = jl_eval_string("refs = IdDict()");
  jl_function_t* setindex = jl_get_function(jl_base_module, "setindex!");
  jl_datatype_t* reft = (jl_datatype_t*)jl_eval_string("Base.RefValue{Any}");

  jl_value_t* calc_root = jl_eval_string(calcid);
  JL_GC_PUSH1(&calc_root);
  jl_value_t* r_calc_root = jl_new_struct(reft, calc_root);
  JL_GC_POP();
   if (jl_exception_occurred()) {
      printf("Exception at set_global : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }
  jl_call3(setindex, refs, r_calc_root, r_calc_root);
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
   /*         
   jl_array_t* _X = jl_alloc_array_1d(jl_float64, 3 * Nat);
   jl_array_t* _cell = jl_alloc_array_1d(jl_float64, 9);
   jl_array_t* _bc = jl_alloc_array_1d(jl_int32, 3);
   jl_array_t* _Z = jl_alloc_array_1d(jl_int32, Nat);

   if (jl_exception_occurred()) {
      printf("Exception at Allocation in atoms_from_c : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   double *XData = (double*)jl_array_data(_X);
   for (int i = 0; i < 3*Nat; i++) XData[i] = X[i];
   double *cellData = (double*)jl_array_data(_cell);
   for (int i = 0; i < 9; i++) cellData[i] = cell[i]; 
   int32_t *bcData = (int32_t*)jl_array_data(_bc);
   for (int i = 0; i < 3; i++) bcData[i] = bc[i]; 
   int32_t *ZData = (int32_t*)jl_array_data(_Z);
   for (int i = 0; i < Nat; i++) ZData[i] = Z[i]; 
   */
   jl_value_t* jl_float64 = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
   jl_value_t* jl_int32 = jl_apply_array_type((jl_value_t*)jl_int32_type, 1);
   jl_array_t* _X = jl_ptr_to_array_1d(jl_float64, X, Nat*3, 0);
   jl_array_t* _Z = jl_ptr_to_array_1d(jl_int32, Z, Nat, 0);
   jl_array_t* _cell = jl_ptr_to_array_1d(jl_float64, cell, 9, 0);
   jl_array_t* _bc = jl_ptr_to_array_1d(jl_int32, bc, 3, 0);
   
   jl_value_t** gc_args;
   JL_GC_PUSHARGS(gc_args, 4);
   gc_args[0] = (jl_value_t*)_X;
   gc_args[1] = (jl_value_t*)_Z;
   gc_args[2] = (jl_value_t*)_cell;
   gc_args[3] = (jl_value_t*)_bc;

   //jl_value_t* at_args[] = {(jl_value_t*)_X, (jl_value_t*)_Z, (jl_value_t*)_cell, (jl_value_t*)_bc};   
   jl_value_t* at = jl_call(_atoms_from_c, gc_args, 4);
   if (jl_exception_occurred()) {
      printf("Exception at creating atoms object in atoms_from_c : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }
   JL_GC_POP();
   return at;
}


double unbox_float64(jl_value_t* jlval) {
   if jl_typeis(jlval, jl_float64_type) {
      return jl_unbox_float64(jlval);
   } 
   jl_errorf("Can't identify the return type!!! \n");
   return 0.0;
}

double energy(char* calcid, double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){
//double energy(jl_value_t** calc, double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){

   jl_value_t** args; 
   JL_GC_PUSHARGS(args, 2);
   //printf("%s\n", calcid);
   jl_value_t* calc = jl_eval_string(calcid);
   if (jl_exception_occurred()) {
      printf("Exception at retrieving calcid in energy call: %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   args[0] = calc; 

   jl_value_t* at = atoms_from_c(X, Z, cell, pbc, Nat);
   //JL_GC_PUSH1(&at);
   if (jl_exception_occurred()) {
      printf("Exception when creating atoms in energy: %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   args[1] = at;

   jl_set_global(jl_main_module, jl_symbol("at"), at);
   if (jl_exception_occurred()) {
      printf("Exception when making at a global in energy : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

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
//void forces(jl_value_t* calc, double *F, 
//             double* X, int32_t* Z, double* cell, int32_t* pbc, int Nat){
  
   jl_value_t** args; 

   JL_GC_PUSHARGS(args, 2);

   jl_value_t* calc = jl_eval_string(calcid);
   if (jl_exception_occurred()) {
      printf("Exception at retrieving calcid in force call : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }
   args[0] = calc; 
   jl_value_t* at = atoms_from_c(X, Z, cell, pbc, Nat);
   //JL_GC_PUSH1(&at);
   if (jl_exception_occurred()) {
      printf("Exception when creating atoms in force call : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   args[1] = at;

   jl_set_global(jl_main_module, jl_symbol("at"), at);
   
   if (jl_exception_occurred()) {
      printf("Exception when making at global in forces : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }
   
   jl_array_t* _F = (jl_array_t*)jl_call2(_forcefcn, calc, at);

   if (jl_exception_occurred()) {
      printf("Exception at calculating forces : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

   double *Fdata = (double*)jl_array_data(_F);

   for (int i = 0; i < 3*Nat; i++) F[i] = Fdata[i];

   if (jl_exception_occurred()) {
      printf("Exception at writing forces : %s \n", 
             jl_typeof_str(jl_exception_occurred()));
   }

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
