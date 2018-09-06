#define XX_PREDECLS

#define XX_INFO \
            "pcg64_random_r:\n" \
            "      -  result:      64-bit unsigned int (uint64_t)\n" \
            "      -  period:      2^128   (* 2^127 streams)\n" \
            "      -  state type:  N/A (private global)\n" \
            "      -  output func: XSL-RR\n" \
            "\n"
            
#define XX_NUMBITS                  "  64bit:"
#define XX_NUMVALUES                6
#define XX_NUMWRAP                  3
#define XX_PRINT_RNGVAL(value)      printf(" 0x%016llx", value)
#define XX_RAND_DECL                
#define XX_SEEDSDECL(seeds)         pcg128_t seeds[2];
#define XX_SRANDOM_SEEDARGS(seeds)  seeds[0], seeds[1]
#define XX_SRANDOM_SEEDCONSTS       42u, 54u
#define XX_SRANDOM(...)             \
            pcg64_srandom(__VA_ARGS__)
#define XX_RANDOM()                 \
            pcg64_random()
#define XX_BOUNDEDRAND(bound)       \
            pcg64_boundedrand(bound)
#define XX_ADVANCE(delta)           \
            pcg64_advance(delta)

#include "pcg_variants.h"
#if PCG_HAS_128BIT_OPS
    #include "check-base.c"
#else
    #include <stdio.h>
    int main()
    {
        printf("This platform does not support 128-bit integers.\n");
        return 1;
    }
#endif
