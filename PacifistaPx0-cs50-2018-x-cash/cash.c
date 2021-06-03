#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //program that gives the lowest possible amount of couns for change
    float change;
    int count = 0, quarter = 25, converter, dime = 10, nickel = 5, penny = 1;

    do
    {
        change = get_float("what is your change: ");
    }

    while (change < 0.00);

    //rounds up the float value of change
    change = (change * 100 + 0.05) / 100;

    //switches change from float to int also from dollars to cents
    converter = (change * 100);

    //the engine of the program
    while (converter >= 25)
    {
        converter = converter - quarter;
        count++;
    }

    while (converter >= 10 && converter < 25)
    {
        converter = converter - dime;
        count++;
    }

    while (converter >= 5 && converter < 10)
    {
        converter = converter - nickel;
        count++;
    }

    while (converter > 0 && converter < 5)
    {
        converter = converter - penny;
        count++;
    }

    //prints out the lowest possible amount of cents
    printf("%i\n", count);

}