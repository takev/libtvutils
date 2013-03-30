
def grouped_hex(x):
    xs = hex(x)[2:-1].upper()

    s = ""
    for i, c in enumerate(xs):
        s+= c
        if (len(xs) - i - 2) % 64 == 63:
            s+= "\n"
        elif (len(xs) - i - 2) % 8 == 7:
            s+= " "

    return s


