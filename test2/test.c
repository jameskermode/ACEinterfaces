  
#include <julia.h>
#include <stdio.h>

JULIA_DEFINE_FAST_TLS() // only define this once, in an executable (not in a shared library) if you want fast code.

int main(int argc, char *argv[])
{
    /* required: setup the Julia context */
    jl_init();
    jl_eval_string("using Pkg; Pkg.activate(\".\"); Pkg.instantiate(; verbose=true);");

    /* run Julia commands */
    jl_eval_string("using ACE, JuLIP");

   /* load a potential */ 
    jl_eval_string("D = load_json(\"randpot.json\"); V = read_dict(D)");

   /* create a structure */ 
   jl_eval_string("at = bulk(:Si, cubic=true) * 3");
   jl_eval_string("rattle!(at, 0.1)");

   /* evaluate the energy on the structure */ 
   jl_value_t *jl_E = jl_eval_string("energy(V, at)");
   if jl_typeis(jl_E, jl_float64_type) {
      double E = jl_unbox_float64(jl_E);
      printf("E = %f \n", E);
   } else {
      printf("Can't identify the return type!!!");
   }

    /* strongly recommended: notify Julia that the
         program is about to terminate. this allows
         Julia time to cleanup pending write requests
         and run all finalizers
    */
    jl_atexit_hook(0);
    return 0;
}
