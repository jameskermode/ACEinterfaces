#include <julia.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ace_c.h"
#include "test_ace.h"

int main(void){
  double X[9] = {-1.62840152, -0.37075213,-0.00382227, 
                -0.64824885, 0.40373766, 0.01092238,
                0.74097139, 0.26398960, -0.21126373};
  int Z[3] = {6, 6, 6};
  double cell[9] = {50.0,  0.0,  0.0,  0.0, 50.0,  0.0,  0.0,  0.0, 50.0};
  int pbc[3] = {0, 0, 0};
  int Nat = 3;
  double F[9];

  /* required: setup the Julia context */
  int a = ace_init(1);
  printf("Initialization successful if 0:\n %d\n", a);
  jl_eval_string("IP = read_dict( load_dict(\"/home/cdt1906/Documents/phd/ACE_dev/interfaces/ACEinterfaces/assets/CH_ACE1_test.json\")[\"IP\"])");
  // The following is to protect IP from being deallocated by the GC
  /*
  jl_value_t* refs = jl_eval_string("refs = IdDict()");
  jl_function_t* setindex = jl_get_function(jl_base_module, "setindex!");
  jl_datatype_t* reft = (jl_datatype_t*)jl_eval_string("Base.RefValue{Any}");

  jl_value_t* calc_root = jl_eval_string("IP");
  JL_GC_PUSH1(&calc_root);
  jl_value_t* r_calc_root = jl_new_struct(reft, calc_root);
  JL_GC_POP();
  jl_call3(setindex, refs, r_calc_root, r_calc_root);
  */
  set_global(&"IP"[0]);
  printf("#### STARTING ENERGY AND FORCE CALCULATIONS ####\n"); 
  for (int i = 0; i < 5000000; i++)
  {
    //double E = energy(calc,  &X[0], &Z[0], &cell[0], &pbc[0], Nat);
    //forces(calc, &F[0], &X[0], &Z[0], &cell[0], &pbc[0], Nat);
    double E = energy(&"IP"[0],  &X[0], &Z[0], &cell[0], &pbc[0], Nat);
    forces(&"IP"[0], &F[0], &X[0], &Z[0], &cell[0], &pbc[0], Nat);
    if (i % 5000 == 0)
    {
      printf("Step %d successful\n", i);
    }
    
  }
  printf("#### 5,000,000 energy and force evaluations without a problem ####\n");

  jl_atexit_hook(0);
  return 0;

}
