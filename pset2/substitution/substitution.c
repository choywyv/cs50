#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string encrypt (string s, string key);
int getIndex (char c);
string checkKey (string key);

const string alphabet = "abcdefghijklmnopqrstuvwxyz";

int main (int argc, string argv[]) {

  if (argc != 2) {
    printf ("%s", "Usage: ./substitution key\n");
    return 1;
  }

  string error = checkKey (argv[1]);
  if (strcmp (error, "") != 0) {
    printf ("%s\n", "s");
    return 1;
  }

  string plaintext = get_string ("plaintext: ");
  printf ("%s", "ciphertext: ");
  printf ("%s\n", encrypt (plaintext, argv[1]));

}


string encrypt (string plaintext, string key) {
  string s = plaintext;
  for (int i = 0; i < strlen (plaintext); i++) s[i] = ! isalpha (plaintext[i]) ? plaintext[i] :
                                                      (isupper (plaintext[i])) ? toupper (key[getIndex (plaintext[i])]) : tolower (key[getIndex (plaintext[i])]);
  return s;
}


int getIndex (char c) {
  for (int i = 0; i < strlen (alphabet); i++) if ((c == alphabet[i]) || (c == toupper (alphabet[i]))) return i;
  return 0;
}


string checkKey (string key) {
  if (strlen (key) != 26) return "Key must contain 26 characters.";

  for (int i = 0; i < strlen (key); i++) if (! isalpha (key[i])) return "Non alphabet found.";

  for (int i = 0; i < strlen (key); i++)
    for (int j = i + 1; j < strlen (key); j++)
      if (key[i] == key [j]) return "Duplicated characters.";

  return "";
}