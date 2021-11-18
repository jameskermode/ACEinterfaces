
double energy(char*, double* X, int* Z, double* cell, int* pbc, int Nat);

void forces(char*, double *F, double* X, int* Z, double* cell, int* pbc, int Nat);

void stress(char* , double *S, double* X, int* Z, double* cell, int* pbc, int Nat);

void ace_init(); 

void ace_cleanup();
