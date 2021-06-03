#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // all variables subjected to change.
    int i, space, height, j = 0, k = 0, l = 0;
    do
    {
        //prompt user for height
        height = get_int("I require a positive number between 0 and 23 for the height of your pyramid: ");
    }
    while (height < 0 || height > 23);

    //we increment i by 1 and reset j, k and l to 0 whenever this loops
    for (i = 1; i <= height; i++, j = 0, k = 0, l = 0)
    {

        //this will loop print spaces until it is time to print #
        for (space = 1; space <= height - i; space++)
        {
            printf(" ");
        }

        //this will loop print #
        while (j != i)
        {
            printf("#");
            j++;
        }

        //print the gap between the half pyramid because k will always be less than i
        if (k < i)
        {
            printf("  ");
        }
        //loop print # after the gaps for the second pyramid
        while (l != i)
        {
            printf("#");
            l++;
        }

        //restart the loop on a new line
        printf("\n");
    }
}