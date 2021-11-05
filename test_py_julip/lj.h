double energy(void*, double* X, int* Z, double* cell, int* pbc, int Nat);
void forces(double* positions, int Nats, double* Forces);

void julip_init(); 
void julip_cleanup();