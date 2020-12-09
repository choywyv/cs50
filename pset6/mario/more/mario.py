from cs50 import get_int

while True:
    n = get_int ("Height: ")
    if n > 0 and n <= 8:
        break

x = 1
y = n - 1

while x <= n:
    print (" " * y + "#" * x + "  " + "#" * x)
    x += 1
    y -= 1
