from cs50 import get_string


def getNumDigits (number):
    return 1 + (0 if int (number) < 10 else getNumDigits (int (number) / 10))


def getValidity (number):
    n2 = getNthDigit (float (number), 2) * 2
    n4 = getNthDigit (float (number), 4) * 2
    n6 = getNthDigit (float (number), 6) * 2
    n8 = getNthDigit (float (number), 8) * 2
    n10 = getNthDigit (float (number), 10) * 2
    n12 = getNthDigit (float (number), 12) * 2
    n14 = getNthDigit (float (number), 14) * 2
    n16 = getNthDigit (float (number), 16) * 2

    x = getNthDigit (n2, 1) + getNthDigit (n2, 2) if n2 >= 10 else n2
    x += getNthDigit (n4, 1) + getNthDigit (n4, 2) if n4 >= 10 else n4
    x += getNthDigit (n6, 1) + getNthDigit (n6, 2) if n6 >= 10 else n6
    x += getNthDigit (n8, 1) + getNthDigit (n8, 2) if n8 >= 10 else n8
    x += getNthDigit (n10, 1) + getNthDigit (n10, 2) if n10 >= 10 else n10
    x += getNthDigit (n12, 1) + getNthDigit (n12, 2) if n12 >= 10 else n12
    x += getNthDigit (n14, 1) + getNthDigit (n14, 2) if n14 >= 10 else n14
    x += getNthDigit (n16, 1) + getNthDigit (n16, 2) if n16 >= 10 else n16
    x += getNthDigit (number, 1)
    x += getNthDigit (number, 3)
    x += getNthDigit (number, 5)
    x += getNthDigit (number, 7)
    x += getNthDigit (number, 9)
    x += getNthDigit (number, 11)
    x += getNthDigit (number, 13)
    x += getNthDigit (number, 15)

    return getNthDigit (x, 1)


def getNthDigit (number, n):
    return int (((int (number) / pow (10, n - 1))) % 10)


def getCardType (number, length):
    if length == 13 and getNthDigit (number, 13) == 4:
        return "VISA"
    elif length == 15 and getNthDigit (number, 15) == 3 and (getNthDigit (number, 14) == 4 or getNthDigit (number, 14) == 7):
        return "AMEX"
    elif length == 16 and getNthDigit (number, 16) == 4 and getValidity (number) == 0:
        return "VISA"
    elif getNthDigit (number, 16) == 5 and getNthDigit (number, 15) >= 1 and getNthDigit (number, 15) <= 5:
        return "MASTERCARD"
    else:
        return "INVALID"


number = get_string ("Number: ")

length = getNumDigits (number)

print ("INVALID" if (length < 13 or length > 16) or getValidity (number) != 0 else getCardType (number, length))