import sys, csv, cs50


if len (sys.argv) < 2:
    sys.exit ("Usage import.py data.csv")

db = cs50.SQL ("sqlite:///students.db")
db.execute ("DELETE FROM students")
db.execute ("DELETE FROM sqlite_sequence where name='students'")

with open (sys.argv[1], "r") as csvfile:
    dbreader = csv.DictReader (csvfile)

    for line in [row for row in dbreader]:
        db.execute ("INSERT INTO students (first, middle, last, house, birth) values (?, ?, ?, ?, ?)", line ["name"].split () [0], None if len (line ["name"].split ()) == 2 else line ["name"].split () [1], line ["name"].split () [1] if len (line ["name"].split ()) == 2 else line ["name"].split () [2], line ["house"], line ["birth"])

