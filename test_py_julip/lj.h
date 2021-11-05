double energy(char*, double* X, int* Z, double* cell, int* pbc, int Nat);
void forces(char*, double *F, double* X, int* Z, double* cell, int* pbc, int Nat);

void julip_init(); 
void julip_cleanup();

void* init_calculator(char* idstr, char* initcmd);

// void ace_init(); 

// void* load_calculator(char* idstr, char* fpath);