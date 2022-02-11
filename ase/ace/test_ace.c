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
  char calcid = *"IP";
  char cmd = *"IP = read_dict( load_dict(\"/home/eng/essswb/ACEinterfaces/assets/CH_ACE1_test.json\")[\"IP\"])";

  /* required: setup the Julia context */
  jl_init();

  printf("#### STARTING ENERGY AND FORCE CALCULATIONS ####\n"); 
  for (int i = 0; i < 100000; i++)
  {
    double E = energy(&calcid,  &X[0], &Z[0], &cell[0], &pbc[0], Nat);
    forces(&calcid, &F[0], &X[0], &Z[0], &cell[0], &pbc[0], Nat);
  }
  printf("#### 100,000 energy and force evaluations without a problem ####\n");

  jl_atexit_hook(0);
  return 0;

}
