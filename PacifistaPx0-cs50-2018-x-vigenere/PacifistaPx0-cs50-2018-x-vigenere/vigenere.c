#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//Vigenere Program
//This program returns a ciphertext using a sequence of keys the user gives

int main(int argc, string argv[])
{
    //Variables to be used later on
    int j, i, l, p, z, keylen;


    if (argc != 2)
    {
        printf("Error 462: invalid operation, I require command line argument? \n");
        return 1;
    }

    string key = (argv[1]);

    keylen = strlen(key);

    //do this if a non alphabet is inputed for argument 2
    for (z = 0; z < keylen; z++)
    {
        if (!isalpha(key[z]))
        {
            printf("Error 524: invalid operation\nI require alphabetical character(s)\n");
            return 1;
        }
    }

    //prompt user for text
    string text = get_string("plaintext: ");

    l = strlen(text);

    j = 0;
    p = 0;


    printf("ciphertext: ");

    //loop until i is greater than the lenght of text inputed
    for (i = 0; i < l; i++)
    {
        //j and p will keep looping only once if this "for" loops and it will satisfy our program. so we can clone j and p
        //for isupper and islower

        //if the text is alpha and the 'p' character of the key is upper
        if (isupper(text[i]) && isupper(key[p]))
        {

            //formula for looping back if p exceeds the keylen

            //formula for the cyphercode "ci = (pi + kj) mod26"
            printf("%c", (((text[i] - 65) + (key[p] - 65)) % 26) + 65);
            j++;
            p = j % keylen;

        }

        //if the text islower and the 'p' character of the key is upper
        else if (islower(text[i]) && isupper(key[p]))
        {

            //formula for looping back if p exceeds the keylen

            //formula for the cyphercode "ci = (pi + kj) mod26"
            printf("%c", (((text[i] - 97) + (key[p] - 65)) % 26) + 97);
            j++;
            p = j % keylen;

        }

        //if the text isupper and the 'p' character of the key is lower
        else if (isupper(text[i]) && islower(key[p]))
        {

            //formula for the cyphercode "ci = (pi + kj) mod26"
            printf("%c", (((text[i] - 65) + (key[p] - 97)) % 26) + 65);
            j++;
            p = j % keylen;

        }

        ////if the text islower and the 'p' character of the key is lower
        else if (islower(text[i]) && islower(key[p]))
        {

            //formula for the cyphercode "ci = (pi + kj) mod26"
            printf("%c", (((text[i] - 97) + (key[p] - 97)) % 26) + 97);
            j++;
            p = j % keylen;

        }

        else
        {
            //prints out every other character that is not an alphabet
            printf("%c", text[i]);
        }
    }

    printf("\n");
    return 0;
}