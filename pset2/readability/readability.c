#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float count_letter (string s);
float count_word (string s);
float count_sentence (string s);
float getIndex (string s);
int getNumDigits (long number);

int main (void) {

  string s = get_string ("Text: "), result = "Grade ";
  int index = getIndex (s);
  float indexx = getIndex (s);
  char strindex[getNumDigits (index)];

  sprintf (strindex, "%i", index);

  printf ("%s%s%s", ((index >= 16) ? "Grade 16+" : (index < 1) ? "Before Grade 1" : "Grade "),
                   (((index >= 16) || (index < 1)) ? "" : strindex), "\n"
         );
}


float count_letter (string s) {
  long n = 0;

  for (long i = 0; i < strlen (s); i++)
    if (isalpha (s[i])) n++;

  return n;
}


float count_word (string s) {
  long n = 0;

  for (long i = 0; i < strlen (s); i++)
    if (isspace (s[i])) n++;

  if ((s[strlen (s) - 1] == '.') || (s[strlen (s) - 1] == '!') || (s[strlen (s) - 1] == '?')) n++;

  return n;
}


float count_sentence (string s) {
  long n = 0;

  for (long i = 0; i < strlen (s); i++)
    if ((s[i] == '.') || (s[i] == '!') || (s[i] == '?')) n++;

  return n;
}


float getIndex (string s) {

  return round ((0.0588 * (count_letter (s) / count_word (s)) * 100.00) - (0.296 * (count_sentence (s) / count_word (s)) * 100.00) - 15.8);

}


int getNumDigits (long number) {
  return 1 + ((number < 10) ? 0 : getNumDigits(number / 10));
}