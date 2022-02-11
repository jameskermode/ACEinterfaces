#include <ace_c.h>
#include <iostream>

int main(void){
  float X[9] = {-1.62840152, -0.37075213,-0.00382227, 
                -0.64824885, 0.40373766, 0.01092238,
                0.74097139, 0.26398960, -0.21126373};
  int Z[3] = {6, 6, 6};
  float cell[9] = {50.0,  0.0,  0.0,  0.0, 50.0,  0.0,  0.0,  0.0, 50.0};
  int pbc[3] = {0, 0, 0};
  int Nat = 3;
  char calcid = "IP";
  char cmd = "IP = read_dict( load_dict(\"/home/cdt1906/Documents/phd/ACE_dev/interfaces/ACEinterfaces/assets/CH_ACE1_test.json\")[\"IP\"])";

  cout << "#### STARTING ENERGY CALCULATIONS ####" << "\n";
  for (int i = 0; i < 100000; i++)
  {
    float E = energy(&calcid,  &X, &Z, &cell, &pbc, Nat);
  }
  


}