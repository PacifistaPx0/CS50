import sys

# cs50 caesar implemented in python


def caesar():

    # making sure the program runs with 2 command line argument

    if len(sys.argv) == 2:
        a = sys.argv[1]
        key = int(a)

        if key > 0:
            text = input("plaintext: ")

            print("ciphertext: ")

            for c in text:

                # condition for uppercase letters

                if c.isalpha() and ord(c) in range(65, 91):
                    if (ord(c) + key) > 90:

                        # sets uppercase letters on ASCII to alphabetical index which starts at A = 0 ->
                        # it adds the key and then mods it by 26; adding back 65 or 97 converts back to ASCII

                        print(chr(((ord(c) - 65 + key) % 26) + 65), end="")
                    else:
                        print(chr(ord(c) + key), end="")

                # condition for lowercase letters

                elif c.isalpha() and ord(c) in range(97, 123):
                    if (ord(c) + key) > 122:

                        # sets lowercase letters on ASCII to alphabetical index which starts at a = 0 ->
                        # it adds the key and then mods it by 26; adding back 65 or 97 converts back to ASCII

                        print(chr(((ord(c) - 97 + key) % 26) + 97), end="")
                    else:
                        print(chr(ord(c) + key), end="")

                else:
                    print(c, end="")

        else:
            print("Invalid operation, I require a positive number for command argument")
            exit(2)
        print()

    elif len(sys.argv) != 2:
        print("I require a command line argument")
        exit(1)

# call the function


caesar()

