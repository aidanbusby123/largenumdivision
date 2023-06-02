#ifndef BIGNUM_H
#define BIGNUM_H

#include "division.h"

typedef struct bignum {
    char *value;
    int exp;
}bignum;

#endif