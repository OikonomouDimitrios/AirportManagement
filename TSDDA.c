#include "TSDDA.h"

int TSDDA_setValue(TStoixeiouDDA *target, TStoixeiouDDA source) {}

int TSDDA_setKey(keyType *key, keyType Val) {}

int TSDDA_setIndex(int *Index, int Val) {}

int TSDDA_iso(TStoixeiouDDA s1, TStoixeiouDDA s2) {}

int TSDDA_mikrotero(TStoixeiouDDA s1, TStoixeiouDDA s2) {}

int TSDDA_megalytero(TStoixeiouDDA s1, TStoixeiouDDA s2) {}

int TSDDA_compare(TStoixeiouDDA s1, TStoixeiouDDA s2) {
    return TSDDA_megalytero(s1, s2) ? 1 : TSDDA_iso(s1, s2) ? 0 : -1;
}