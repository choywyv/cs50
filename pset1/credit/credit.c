#include <stdio.h>
#include <math.h>
#include <cs50.h>

int getNumDigits (long number);
long getNthDigit (long number, int n);
long getValidity (long number);
string getCardType (long number, int length);


int main (void) {

  long number;
  int length;
  string result;

  number = get_long ("Number: ");

  length = getNumDigits (number);

  printf ("%s", (length < 13 || length > 16) ? "INVALID\n" :
                (getValidity (number) != 0) ? "INVALID\n" : getCardType (number, length));

}


string getCardType (long number, int length) {
  return ((length == 13) && (getNthDigit (number, 13) == 4)) ? "VISA\n" :
         ((length == 15) && ((getNthDigit (number, 15) == 3) && (getNthDigit (number, 14) == 4 || getNthDigit (number, 14) == 7))) ? "AMEX\n" :
         ((length == 16) && ((getNthDigit (number, 16) == 4) && (getValidity (number) == 0))) ? "VISA\n" :
         ((getNthDigit (number, 16) == 5) && (getNthDigit (number, 15) >= 1 && getNthDigit (number, 15) <= 5) ) ? "MASTERCARD\n" : "INVALID\n";
}


int getNumDigits (long number) {
  return 1 + ((number < 10) ? 0 : getNumDigits (number / 10));
}


long getNthDigit (long number, int n) {
  return ((long) (number / pow (10, n - 1))) % 10;
}


long getValidity (long number) {
  long n2 = getNthDigit (number, 2) * 2,
       n4 = getNthDigit (number, 4) * 2,
       n6 = getNthDigit (number, 6) * 2,
       n8 = getNthDigit (number, 8) * 2,
       n10 = getNthDigit (number, 10) * 2,
       n12 = getNthDigit (number, 12) * 2,
       n14 = getNthDigit (number, 14) * 2,
       n16 = getNthDigit (number, 16) * 2;

  return getNthDigit ( ((n2 >= 10) ? (getNthDigit (n2, 1) + getNthDigit (n2, 2)) : n2) +
                       ((n4 >= 10) ? (getNthDigit (n4, 1) + getNthDigit (n4, 2)) : n4) +
                       ((n6 >= 10) ? (getNthDigit (n6, 1) + getNthDigit (n6, 2)) : n6) +
                       ((n8 >= 10) ? (getNthDigit (n8, 1) + getNthDigit (n8, 2)) : n8) +
                       ((n10 >= 10) ? (getNthDigit (n10, 1) + getNthDigit (n10, 2)) : n10) +
                       ((n12 >= 10) ? (getNthDigit (n12, 1) + getNthDigit (n12, 2)) : n12) +
                       ((n14 >= 10) ? (getNthDigit (n14, 1) + getNthDigit (n14, 2)) : n14) +
                       ((n16 >= 10) ? (getNthDigit (n16, 1) + getNthDigit (n16, 2)) : n16) +
                       getNthDigit (number, 1) +
                       getNthDigit (number, 3) +
                       getNthDigit (number, 5) +
                       getNthDigit (number, 7) +
                       getNthDigit (number, 9) +
                       getNthDigit (number, 11) +
                       getNthDigit (number, 13) +
                       getNthDigit (number, 15), 1);

}