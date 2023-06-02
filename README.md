# largenumdivision
largenumberdivision is a small C library designed to divide arbitrarily large numbers. It does this by storing values as character arrays, stored in the predefined `bignum` struct, and passing them to the `l_div` function.
## Use
This library (if you can even call it that) is quite easy to use. Simply include the required header file, and you are good to go.
`
#include "bignum.h"
`
To actually use the `l_div` function, however, you first need to convert all of your values to the `bignum` type, like so:
```
char foo = "123"; // NUL termination is necessary!
char baa = "456";
 
bignum a;
bignum b;
bignum result;

strcpy(a.value, foo);
strcpy(b.value, baa);

```
Please note that it is important that any char arrays must be NUL terminated, or the library will not be able to discern the size of your inputs.

After correctly formatting all values, simply pass them to the `l_div` function
```
result = l_div(a, b);
```
`l_div` returns a `bignum` data type as well, for ease of use and compatability.

Note: the `exp` value in `bignum` gives the power of ten that the result number must be multiplied by in order to get a correct value. There are also placeholder zeroes, which are necessary for proper calculations.
