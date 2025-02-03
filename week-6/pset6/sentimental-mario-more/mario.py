# TODO

while True:
    height = input("Height: ")
    try:
        height = int(height)
    except:
        continue

    if height > 0 and height < 9:
        break

for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i + "  " + "#" * i)
