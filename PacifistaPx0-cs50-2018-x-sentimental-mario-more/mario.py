import sys

# mario implemented python


def mario():
    # prompt user for none negative integer and error check using valueError

    while True:
        try:
            height = int(input("What is your height: "))
        except ValueError:
            print('Please enter a valid integer')
            continue
        if height >= 0 and height < 24:
            break
    for i in range(1, height+1):
        j, k, l = 0, 0, 0

        # this will loop print spaces until it is time to print
        for space in range(height-i):
            print(" ", end="")
            space += 1

        # this will loop print #
        while j != i:
            print("#", end="")
            j += 1

        # print the gap between the half pyramid because k will always be less than i
        if k < i:
            print("  ", end="")

        # loop print # after the gaps for the second pyramid
        while l != i:
            print("#", end="")
            l += 1

        # restart the loop on a new line
        print()


mario()
