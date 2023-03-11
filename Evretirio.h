#ifndef __Evr__
#define __Evr__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TSEvr.h"
#include "TSDDA.h"

#define MAX_LINE_LENGTH 1024

#define MAXSIZE 72000

typedef struct EvrNode *EvrPtr;

EvrPtr EvrConstruct(int MaxSize);    /* gia erwthma 1 */
int EvrDestruct(EvrPtr *E);        /* gia erwthma 5 */

int EvrInsert(EvrPtr E, TStoixeiouEvr Data); /* gia erwthma 2 */
int EvrSearch(EvrPtr E, keyType key, int InOut, int *found); /* gia erwthma 3 */

int Evr_ReadFromFile(EvrPtr E);

#endif
