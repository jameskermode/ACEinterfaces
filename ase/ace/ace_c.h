
double energy(char*, double* X, int* Z, double* cell, int* pbc, int Nat);

void forces(char*, double *F, double* X, int* Z, double* cell, int* pbc, int Nat);

void stress(char* , double *S, double* X, int* Z, double* cell, int* pbc, int Nat);

int ace_init(int ACE_version); 

void ace_cleanup();
