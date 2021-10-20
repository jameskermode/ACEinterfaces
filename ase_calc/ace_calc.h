/* Function prototypes for ace_calc.c */

void init_calc(char IP_name);
double energy(double* cell, short* bc, double* positionss, int* Z);
double forces(double* cell, short* bc, double* positionss, int* Z);
double stress(double* cell, short* bc, double* positionss, int* Z);
