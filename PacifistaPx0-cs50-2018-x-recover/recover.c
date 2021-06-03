#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    // return error if command line argument isnt 2
    if(argc != 2)
    {
        fprintf(stderr, "Two command line argument required\n");
        return 1;
    }

    // set file_1 to second command line arg
    char *file_1 = argv[1];
    FILE *file_ptr = fopen(file_1, "r");

    //Ensure no error
    if(file_ptr == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return 2;
    }


    //Variables to be used
    //allocate 512 memory block to buffer
    unsigned char *buffer = malloc(512); int jpg_no = 0;
    FILE *pictures = NULL;

    //iterate over memory card
    while(fread(buffer, 512, 1, file_ptr) == 1)
    {
        //if jpeg signature found
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous jpg file
            if(pictures != NULL)
            {
                fclose(pictures);
            }

            // create filename
            //7 array block of space for the filename
            char filename[7];
            sprintf(filename, "%03i.jpg", jpg_no);

            // open new image file
            pictures = fopen(filename, "w");



            //increment jpg_no
            jpg_no++;
        }
        if(!jpg_no) continue;

        // write jpg info into current file
        fwrite(buffer, 512, 1, pictures);
    }

    fclose(file_ptr);
    fclose(pictures);

    return 0;
}