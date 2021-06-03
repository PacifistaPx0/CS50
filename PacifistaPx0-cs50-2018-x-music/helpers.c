// Helper functions for music

#include <stdio.h>
#include <cs50.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{

    int a = (fraction[0] - 48), b = (fraction[2] - 48);

    //formula to convert to eights
    int final = ((a * 8) / b);
    return final;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    //variables of the first letter, accidental and octave
    int letter = note[0], accidental = 0, octave;

    //this determines the number of semitones it takes from A which starts at 0 to the other letters
    switch (note[0])
    {
        case 'A' :
            letter = 0;
            break;

        case 'B' :
            letter = 2;
            break;

        case 'G' :
            letter = -2;
            break;

        case 'F' :
            letter = -4;
            break;

        case 'E' :
            letter = -5;
            break;

        case 'D' :
            letter = -7;
            break;

        case 'C' :
            letter = -9;
            break;
    }

    //in the case the note has an accidental of # the octave is placed at the 3rd array
    if (note[1] == '#')
    {
        octave = note[2];

        //adjusting the ASCII value of the character of the octave
        octave -= 48;

        //one is added if the octave is a # i.e moves up one semitone
        accidental = 1;
    }

    else if (note[1] == 'b')
    {
        octave = note[2];
        octave -= 48;

        //one is subtracted if the octave is a b i.e moves down one semitone
        accidental = -1;
    }

    //the octave is in the second array
    else
    {
        octave = note[1];
        octave -= 48;
    }

    //formula that works for both when the octave is higher or less than 4
    //by merging the 3 formulas (the letter, the octave and the accidental) in respect to the power of 2

    double n = (letter + (12.0 * (octave - 4)) + accidental) / 12.0;

    //from the genral equation 440 * 2 ^ n(which is the result from the formula)
    float freq = 440.0 * pow(2, n);

    int finalfreq = round(freq);
    return finalfreq;
}


// Determines whether a string represents a rest
bool is_rest(string s)
{
    // if the first character of s is NUL return true, else false
    if (s[0] == '\0')
    {
        return true;
    }
    else
    {
        return false;
    }
}
