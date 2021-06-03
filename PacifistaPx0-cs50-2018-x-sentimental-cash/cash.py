import sys

# program that gives the lowest possible amount of couns for change


def cash():
    change, count, quarter, dime, nickel, penny = 0, 0, 25, 10, 5, 1
    while True:
        try:
            change = float(input("change owed: "))
        except ValueError:
            print('Please enter a number')
        if change > 0.00:
            break

     # rounds up the float value of change

    change = (change * 100 + 0.05) / 100

    # switches change from float to int also from dollars to cents

    converter = int(change * 100)

    while converter >= 25:
        converter -= quarter
        count += 1

    while converter >= 10 and converter < 25:
        converter -= dime
        count += 1

    while converter >= 5 and converter < 10:
        converter -= nickel
        count += 1

    while converter > 0 and converter < 5:
        converter -= penny
        count += 1

    # prints out the lowest possible amount of cents

    print("{0}".format(count))

# calls the function


cash()
