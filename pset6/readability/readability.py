from cs50 import get_string

def getIndex (s):
  return round ((0.0588 * (count_letter (s) / count_word (s)) * 100.00) - (0.296 * (count_sentence (s) / count_word (s)) * 100.00) - 15.8);


def count_letter (s):
  n = 0

  for i in range (len (s)):
    if (s[i].isalpha ()):
      n += 1

  return n;


def count_word (s):
  n = 0

  for i in range (len (s)):
    if (s[i].isspace ()):
      n += 1;

  if ((s[len (s) - 1] == '.') or (s[len (s) - 1] == '!') or (s[len (s) - 1] == '?')):
    n += 1;

  return n;


def count_sentence (s):
  n = 0

  for i in range (len (s)):
    if ((s[i] == '.') or (s[i] == '!') or (s[i] == '?')):
      n += 1;

  return n;


s = get_string ("Text: ")
index = getIndex (s)

if index >= 16:
  print ("Grade 16+")
elif index < 1:
  print ( "Before Grade 1")
else:
  print ("Grade {grade}".format (grade = "" if index >= 16 or index < 1 else index))
