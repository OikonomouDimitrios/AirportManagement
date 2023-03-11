#ifndef __TSDDA__
#define __TSDDA__

#include <stdio.h>

typedef int keyType; /* integer key for airport */

typedef struct KeyIndex {
    keyType key;
    int arrayIndex;
} TStoixeiouDDA;


int TSDDA_setValue(TStoixeiouDDA *target, TStoixeiouDDA source);

int TSDDA_setKey(keyType *key, keyType Val);

int TSDDA_setIndex(int *Index, int Val);

/* basikes sygkriseis gia xrhsh sthn eisagcgh-diagarfh-anazhthsh se DDA */
int TSDDA_iso(TStoixeiouDDA, TStoixeiouDDA);

int TSDDA_mikrotero(TStoixeiouDDA, TStoixeiouDDA);

int TSDDA_megalytero(TStoixeiouDDA, TStoixeiouDDA);

int TSDDA_compare(TStoixeiouDDA *s1, TStoixeiouDDA *s2);

void integer_type_to_string(TStoixeiouDDA *s1, char *buffer, size_t buffer_size);

#endif
