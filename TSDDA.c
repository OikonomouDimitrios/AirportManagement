#include "TSDDA.h"

int TSDDA_setValue(TStoixeiouDDA *target, TStoixeiouDDA source) {}

int TSDDA_setKey(keyType *key, keyType Val) {
    if (key == NULL) {
        return -1;
    }
    *key = Val;

    return 0;
}

int TSDDA_setIndex(int *Index, int Val) {
    if (Index == NULL) {
        return -1;
    }
    *Index = Val;

    return 0;
}

int TSDDA_iso(TStoixeiouDDA s1, TStoixeiouDDA s2) {
    return s1.key == s2.key;
}

int TSDDA_megalytero(TStoixeiouDDA s1, TStoixeiouDDA s2) {
    return s1.key > s2.key;
}

int TSDDA_compare(TStoixeiouDDA *s1, TStoixeiouDDA *s2) {
    return TSDDA_megalytero(*s1, *s2) ? 1 : TSDDA_iso(*s1, *s2) ? 0 : -1;
}

void integer_type_to_string(TStoixeiouDDA *s1, char *buffer, size_t buffer_size) {
    int value = s1->key;
    snprintf(buffer, buffer_size, "%d", value);
}