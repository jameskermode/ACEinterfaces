#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lj.h"

#define EPSILON 1.0;
#define SIGMA 1.0;

double LJ(double r, double eps, double sigm){
  double r_sc = pow( (sigm / r), 6);
  return 4 * eps * (pow(r_sc, 2) - r_sc);
}

double LJ_d(double r, double eps, double sigm){
  return 24 * eps * pow(sigm, 6) * (-2 * pow(sigm, 6)* pow(r, -13) + pow(r, -7));
}

double energy(double* positionss, int Nats){
    double Energy = 0.0;
    for (int i = 0; i < Nats - 1; i++)
    {
      for (int j = i + 1; j < Nats; j++)
      {
        double D = 0.0;
        D += pow(positionss[3*i] - positionss[3*j], 2);
        D += pow(positionss[3*i+1] - positionss[3*j+1], 2);
        D += pow(positionss[3*i+2] - positionss[3*j+2], 2);
        D = pow(D, 0.5);
        // printf("C distance: %f\n", D);
        Energy += LJ(D, 1.0, 1.0);
      }
    }
    return Energy;
}

void forces(double* positions, int Nats, double* Forces){
  for (int i = 0; i < 3*Nats; i++)
  {
    Forces[i] = 0.0;
  }
  
  for (int i = 0; i < Nats - 1; i++)
    {
      for (int j = i + 1; j < Nats; j++)
      {
        double D = 0.0;
        D += pow(positions[3*i] - positions[3*j], 2);
        D += pow(positions[3*i+1] - positions[3*j+1], 2);
        D += pow(positions[3*i+2] - positions[3*j+2], 2);
        D = pow(D, 0.5);
        // printf("C distance: %f\n", D);
        double F = LJ_d(D, 1.0, 1.0) / pow(1, 1/3);
        double f1 = (positions[3*i] - positions[3*j]) / D * F;
        double f2 = (positions[3*i+1] - positions[3*j+1]) / D * F;
        double f3 = (positions[3*i+2] - positions[3*j+2]) / D * F;
        Forces[3*i] += f1;
        Forces[3*j] -= f1;
        Forces[3*i + 1] += f2;
        Forces[3*j + 1] -= f2;
        Forces[3*i + 2] += f3;
        Forces[3*j + 2] -= f3;
      }
    }
}
