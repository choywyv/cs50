import sys, csv, cs50


if len (sys.argv) < 2:
    sys.exit ("Usage roster.py name_of_house")

db = cs50.SQL ("sqlite:///students.db")
rows = db.execute ("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv [1])

for row in rows:
    print (row ["first"] + " " + str ("" if row ["middle"] == None else row ["middle"] + " ") + row ["last"] + ", born " + str (row ["birth"]))




