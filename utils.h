/*
  utils.h - useful utils for embedded
  (c) R Brooks 2014
*/
#ifndef utils_h
#define utils_h

#include <Energia.h>

//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

class Utils
{
  public:
    static long cropLongToXDigits(long val, int totalDigits);
    static long power(int base, int e);
    static long divideByTen(long num);
    static long divideByHundred(long num);
  private:
};

#endif
