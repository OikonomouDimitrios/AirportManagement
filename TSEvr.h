#ifndef __TSEvr__
#define __TSEvr__

#include <stdio.h>


typedef struct data {
        /*  members for airport data in array */
} TStoixeiouEvr;

int TSEvr_setValue (TStoixeiouEvr *target, TStoixeiouEvr source);

int TSEvr_readValue (FILE *from,  TStoixeiouEvr *Elem);

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem);

#endif
