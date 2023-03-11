#include <stdio.h>
#include <string.h>
#include "Evretirio.h"

#define MAX_LINE_LENGTH 1024

int main() {

    EvrPtr E = EvrConstruct(MAXSIZE);
    Evr_ReadFromFile(E);

    return 0;
}