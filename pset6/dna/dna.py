import sys, csv

if len (sys.argv) < 3:
    sys.exit ("Usage: python dna.py data.csv sequence.txt")

with open (sys.argv[1], "r") as db:
    dbreader = csv.reader (db)
    data = [row for row in dbreader]

with open (sys.argv[2], "r") as sequences:
    dna = sequences.read ()

matches = []

for i in range (1, len (data[0])):
    count = 1
    STR = data[0][i]

    while STR * count in dna:
        count += 1
    matches.append (str (count - 1))

for i in range (1, len (data)):
    if data[i][1:len (data[0])] == matches:
        sys.exit (data[i][0])

print ("No Match")