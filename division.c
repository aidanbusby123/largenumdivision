#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "division.h"
#include"bignum.h"

int cmp(char *a, char *b, int sz){ // compare the value of character array
    int a_sz = sz;
    int b_sz = sz;
    for (int i = 0; i < sz; i++){
        if (a[i] == 0){
            a_sz--;
        } else
            break;
    }
    for (int i = 0; i < sz; i++){
        if (b[i] == 0){
            b_sz--;
        } else
            break;
    }
    
    if (a_sz > b_sz)
        return 1;
    else if (a_sz < b_sz)
        return -1;
    else{
        for (int i = 0; i < sz; i++){
            if (a[i] > b[i])
                return 1;
            if (b[i] > a[i])
                return -1;
        }
        return 0;
    }
}

int resize(char **s, int sz){
    *s = realloc(*s, sz);
    for (int k = sz/2; k < sz; k++){
        (*s)[k] = 0;
    }
    for (int k = sz-1; k >= sz/2; k--){
        (*s)[k] = (*s)[k-(sz/2)];
    }
    for (int k = 0; k < sz/2; k++){
        (*s)[k] = 0;
    }
}

bignum l_div(bignum a, bignum b){
    int a_sz = strlen(a.value); // numerator size
    int b_sz = strlen(b.value); // denominator size
    int dvd_sz = a_sz;
    int dvs_sz = b_sz;
    int tmp_sz[2] = {1, 1};
    int quo_sz = 1;
    int tmp_quo_sz = 1;
    int sz = (((a_sz > b_sz) ? a_sz : b_sz)+2)/sizeof(char); // standard array size
    int orig_sz = sz; // original array size, used for referencing different sized arrays
    int precision = 0; // precision of calculation, used to give correct number of sig digs
    int remainder = 0;

    char *quo = malloc(sz*sizeof(char));
    char *temp_quo = malloc (sz*sizeof(char)); // temporary quotient variable

    // store a and b in new arrays

    char *dividend = malloc(sz*sizeof(char)); // dividend reference value
    char *divisor = malloc(sz*sizeof(char)); // divisor reference value
    char *dvd = malloc(sz*sizeof(char)); // temporary dividend array
    char *dvs = malloc(sz*sizeof(char)); // temporary divisor array
    char *temp[2] = {malloc(sz*sizeof(char)), malloc(sz*sizeof(char))}; // temporary array, used for various things

    struct bignum res; // bignum struct for result

    int exp = 0; // store quotient magnitude
    int shift = 0; //temporary version of exp, find out how far left dividend has been shifted

    int i = 0;
    for (i = 0; i < sz; i++){
        dividend[i] = divisor[i] = dvd[i] = dvs[i] = temp[0][i] = temp[1][i] = quo[i] = 0;
    }
    for (i = 0; i < a_sz; i++){ // store a in dvd (reversed)
        dividend[i+(sz-a_sz)] = dvd[i+(sz-a_sz)] = a.value[i] - '0';
    }

    for (i = 0; i < b_sz; i++){ // store b in dvs (reversed)
        divisor[i+(sz-b_sz)] = dvs[i+(sz-b_sz)] = b.value[i] - '0';
    }

    
    i = 0;

    while (dvd_sz != 0 && quo_sz < MAX_PRECISION){ // main algorithm
        for (int k = sz-1; k >= sz-orig_sz; k--){ // reset dvs (temporary divisor)
            temp[0][k] = dvs[k] = divisor[orig_sz-(sz-k)];
        }
        for (int k = 0; k < sz; k++){
            temp_quo[k] = temp[1][k] = 0;
        }
        temp_quo[sz-1] = temp[1][sz-1] = 1;
        tmp_sz[0] = dvs_sz = b_sz;
        tmp_sz[1] = 1;
        // Left shift dividend until greater than divisor, if neccessary
        if (dvs_sz > dvd_sz){ 

            if (sz-dvs_sz <= 1 || sz-(quo_sz+dvs_sz-dvd_sz) <= 1){ // out of space in array
                sz *= 2;
                dvd = &(dvd[0]);
                dvs = &(dvs[0]);
                temp[0] = &(temp[0][0]);
                temp[1] = &(temp[1][0]);
                quo = &(quo[0]);
                temp_quo = &(temp_quo[0]);
                resize(&dvd, sz);
                resize(&dvs, sz);
                resize(&(temp[0]), sz);
                resize(&(temp[1]), sz);
                resize(&quo, sz);
                resize(&temp_quo, sz);
 
            }
            for (int k = sz-dvs_sz-1; k < sz-(dvs_sz-dvd_sz); k++){
                dvd[k] = dvd[k + (dvs_sz-dvd_sz)];
            }
            for (int k = sz-(dvs_sz-dvd_sz); k < sz; k++){
                dvd[k] = 0;
            }
            for (int k = sz-dvs_sz+dvd_sz-quo_sz; k < sz-(dvs_sz-dvd_sz); k++){
                quo[k] = quo[k + (dvs_sz-dvd_sz)];
            }

            for (int k = sz-(dvs_sz-dvd_sz); k < sz; k++){
                quo[k] = 0;
            }        
            exp -= dvs_sz-dvd_sz;
            quo_sz += dvs_sz-dvd_sz;
            dvd_sz = dvs_sz;
            
            
        }
        while (cmp(dvd, dvs, sz) == -1){ // decimal left shift dividend by 1, (multiply by ten)
            for (int k = sz-dvd_sz-1; k < sz-1; k++){
                dvd[k] = dvd[k+1];
            }
            for (int k = sz-quo_sz-1; k < sz-1; k++){
                quo[k] = quo[k+1];
            }
            
            dvd[sz-1] = 0;
            quo[sz-1] = 0;
            exp--;
            shift++;
            dvd_sz++;
            quo_sz++;
            if (sz-dvd_sz <= 1 || sz-quo_sz <= 1){ // out of space in array
                sz *= 2;
                dvd = &(dvd[0]);
                dvs = &(dvs[0]);
                temp[0] = &(temp[0][0]);
                temp[1] = &(temp[1][0]);
                quo = &(quo[0]);
                temp_quo = &(temp_quo[0]);
                resize(&dvd, sz);
                resize(&dvs, sz);
                resize(&(temp[0]), sz);
                resize(&(temp[1]), sz);
                resize(&quo, sz);
                resize(&temp_quo, sz);

            }                     
        }

        // Multiply divisor by 2 until greater than dividend, get previous value


        while (tmp_sz[0] < dvd_sz){

            for (int k = 0; k < sz; k++){
                dvs[k] = temp[0][k];
                temp_quo[k] = temp[1][k];
            }
            dvs_sz = tmp_sz[0];
            tmp_quo_sz = tmp_sz[1];
            for (int k = sz-1; k >= (sz-tmp_sz[0]); k--){
                temp[0][k] *= 2;

            }
            for (int k = sz-1; k >= (sz-tmp_sz[0]); k--){
                if (k == (sz-tmp_sz[0]) && (temp[0][k] >= 10)){
                    tmp_sz[0]++;
                }
                temp[0][k-1] += temp[0][k] / 10;
                temp[0][k] %= 10;            
            }
            for (int k = sz-1; k >= (sz-tmp_sz[1]); k--){
                temp[1][k] *= 2;
            }
            for (int k = sz-1; k >= (sz-tmp_sz[1]); k--){
                if (k == (sz-tmp_sz[1]) && (temp[1][k] >= 10)){
                    tmp_sz[1]++;
                }
                temp[1][k-1] += temp[1][k] / 10;
                temp[1][k] %= 10;
            }
            if ((sz-tmp_sz[0] <= 1) || (sz-tmp_sz[1] <= 1)){
                sz *= 2;
                dvd = &(dvd[0]);
                dvs = &(dvs[0]);
                temp[0] = &(temp[0][0]);
                temp[1] = &(temp[1][0]);
                quo = &(quo[0]);
                temp_quo = &(temp_quo[0]);
                resize(&dvd, sz);
                resize(&dvs, sz);
                resize(&temp[0], sz);
                resize(&temp[1], sz);
                resize(&quo, sz);
                resize(&temp_quo, sz);
            }
        }
        while (cmp(dvd, temp[0], sz) == 1){
            for (int k = 0; k < sz; k++){
                dvs[k] = temp[0][k];
                temp_quo[k] = temp[1][k];
            }
            dvs_sz = tmp_sz[0];
            tmp_quo_sz = tmp_sz[1];
            for (int k = sz-1; k >= (sz-tmp_sz[0]); k--){
                temp[0][k] *= 2;

            }
            for (int k = sz-1; k >= (sz-tmp_sz[0]); k--){
                if (k == (sz-tmp_sz[0]) && (temp[0][k] >= 10)){
                    tmp_sz[0]++;
                }
                temp[0][k-1] += temp[0][k] / 10;
                temp[0][k] %= 10;            
            }
            for (int k = sz-1; k >= (sz-tmp_sz[1]); k--){
                temp[1][k] *= 2;
            }
            for (int k = sz-1; k >= (sz-tmp_sz[1]); k--){
                if (k == (sz-tmp_sz[1]) && (temp[1][k] >= 10)){
                    tmp_sz[1]++;
                }
                temp[1][k-1] += temp[1][k] / 10;
                temp[1][k] %= 10;
            }
            if ((sz-tmp_sz[0] <= 1) || (sz-tmp_sz[1] <= 1)){
                sz *= 2;
                dvd = &(dvd[0]);
                dvs = &(dvs[0]);
                temp[0] = &(temp[0][0]);
                temp[1] = &(temp[1][0]);
                quo = &(quo[0]);
                temp_quo = &(temp_quo[0]);
                resize(&dvd, sz);
                resize(&dvs, sz);
                resize(&temp[0], sz);
                resize(&temp[1], sz);
                resize(&quo, sz);
                resize(&temp_quo, sz);
            }

        }
        for (int k = sz-1; k >= sz-tmp_quo_sz; k--){
            quo[k] += temp_quo[k];
        }
        for (int k = sz-1; k > 0; k--){
            quo[k-1] += quo[k] / 10;
            if ((k == sz-quo_sz) && (quo[k] >= 10))
                quo_sz++;
            quo[k] %= 10;
        }

        // Subtract divisor from dividend
        for (int k = sz-1; k >= 0; k--){
            dvd[k] -= dvs[k];
            if (dvd[k] < 0){
                dvd[k] += 10;
                dvd[k-1] -= 1;
            }
        }
        dvd_sz = sz;
        for (int k = 0; k < sz; k++){
            if (dvd[k] == 0)
                dvd_sz--;
            else
                break;
        }
        remainder = dvd_sz;
        i++;
    }
    if (remainder == 0){
        precision = quo_sz;
    } else {
        precision = quo_sz-1;
    }
    res.exp = exp;
    res.value = malloc((precision+1)*sizeof(char));
    for (int i = 0; i < precision; i++){
        res.value[i] = quo[i + sz-quo_sz] + '0';
    }
    res.value[quo_sz] = 0;

    return res;
}
