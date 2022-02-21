
double energy(char*, double* X, int* Z, double* cell, int* pbc, int Nat);
//double energy(jl_value_t*, double* X, int* Z, double* cell, int* pbc, int Nat);

void forces(char*, double *F, double* X, int* Z, double* cell, int* pbc, int Nat);
//void forces(jl_value_t*, double *F, double* X, int* Z, double* cell, int* pbc, int Nat);

void stress(char* , double *S, double* X, int* Z, double* cell, int* pbc, int Nat);

int ace_init(int ACE_version); 

void ace_cleanup();

void set_global(char* calcid);
