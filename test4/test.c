  
#include <julia.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();

   /* create random data defining structure */ 
   double A[] = { 0.0, 0.0, 0.0, 3.0, 0.2, 0.5, 0.1, 2.5, 2.8 };
   double B[] = {5.0, 2.0, 0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 3.0};

   /* create the JuLIP structure */ 
   jl_value_t* jl_float64 = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
   jl_array_t* _A = jl_alloc_array_1d(jl_float64, 9);
   jl_array_t* _B = jl_alloc_array_1d(jl_float64, 9);

   double *AData = (double*)jl_array_data(_A);
   for (int i = 0; i < 9; i++) AData[i] = A[i];
   double *BData = (double*)jl_array_data(_B);
   for (int i = 0; i < 9; i++) BData[i] = B[i]; 

    jl_value_t **args;
    JL_GC_PUSHARGS(args, 2); // args can now hold 2 `jl_value_t*` objects
    args[0] = (jl_value_t*)_A;
    args[1] = (jl_value_t*)_B;

   jl_function_t *display = jl_get_function(jl_base_module, "display");

   jl_call1(display, (jl_value_t*)_A); 
   printf("\n");

   jl_call1(display, (jl_value_t*)_B);   
   printf("\n");

   JL_GC_POP();

    jl_atexit_hook(0);
    return 0;
}
