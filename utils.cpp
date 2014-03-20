#include "utils.h"

long Utils::power(int base, int e)
{
    long result = 1;
    while (e)
    {
        if (e & 1)
            result *= base;
        e >>= 1;
        base *= base;
    }

    return result;
}

long Utils::divideByTen(long num) {
  return (num * 205) >> 11;
}

long Utils::divideByHundred(long num) {
  return (num * 655) >> 16;
}

long Utils::cropLongToXDigits(long val, int totalDigits) {
  long divideFactor = Utils::power(10, totalDigits - 1);
  long iterativeVal = val;
  
  while (iterativeVal > (divideFactor * 100)) {
    iterativeVal = Utils::divideByHundred(iterativeVal);
  }
  while (iterativeVal > (divideFactor * 10)) {
    iterativeVal = Utils::divideByTen(iterativeVal);
  }
  return iterativeVal;
}
