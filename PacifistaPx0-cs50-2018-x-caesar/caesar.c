#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Caesar Program
//This program returns a ciphertext based on the key the user gives

int main(int argc, string argv[])
{
    //Variables to be used later on
    int key, i, l;


    if (argc != 2)
    {
        printf("Error 462: invalid operation, I require command line argument? \n");
        return 1;
    }

    //converts from string to int
    key = atoi(argv[1]);

    if (key > 0)
    {
        //prompt user for text
        string text = get_string("plaintext: ");

        l = strlen(text);

        printf("ciphertext: ");

        //loops until i is greater than string length supplied in text
        for (i = 0; i < l; i++)
        {
            //checks for if capital letter
            if (isupper(text[i]))
            {
                //sets lowercase  or uppercase letters on ASCII to alphabetical index which starts at A = 0 ->
                //it adds the key and then mods it by 26; adding back 65 or 97 converts back to ASCII
                printf("%c", (((text[i] - 65) + key) % 26) + 65);
            }

            else if (islower(text[i]))
            {
                printf("%c", (((text[i] - 97) + key) % 26) + 97);
            }

            else
            {
                //prints out every other character that is not an alphabet
                printf("%c", text[i]);
            }
        }
    }
    else
    {
        printf("Error 524: invalid operation\nI require a positive number for argument\n");
    }
    printf("\n");
    return 0;
}