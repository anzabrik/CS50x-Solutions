from cs50 import get_int

height = 0

while height <= 0 or height > 8:
    height = get_int("Height: ")


spaces = height - 1
hashes = 1

# Prints row after row

for row in range(height):
    print(' ' * spaces, end="")
    print("#" * hashes, end="")

    # This is the middle of the row
    print("  ", end="")
    print("#" * hashes)
    
    # Increment/decrement spaces and hashes before moving to the following row
    spaces -= 1
    hashes += 1