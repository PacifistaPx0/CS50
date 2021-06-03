# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

the longest word in the dictionary i.e the Max word length for a word
(an invented long word said to mean a lung disease caused by inhaling very fine ash and sand dust.)

## According to its man page, what does `getrusage` do?

in speller.c getrusage records the statistic of the &before (a point in time before the size has been calculated) and then after the size has been evaluated, it records the &after (a point in time after size has been evaluated). Then it calculates the difference between the time (When calculate(&a, &b); is used)

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

we pass before and after by reference so changes can reflect outside the function. We are inserting addresses of both pointers.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

Main goes about reading the words from a file via the for loop. basically, we declare a variable c of type int and then set it to a file. The for loop then checks for the condition and if the c is not equal to the End of File, the loop then proceeds to evaluate the body checking for any conditions that will not agree with what we intend to read from the file.

The loop goes on to check if the word is greater than the max length 'and if' the word is digit and ignores the word and resets index to to 0 else we mustve found a word. It then terminates the previous word.

The loop goes on to update timing, print misspelled words and reset index to 0. And finally, the loop sets c into the file we are about to read.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

fgetc is used so we are not likely to face the problem that might occur using fscanf to read a text file which doesnt always end with a line return or space. That is to say fscanf will go on to pick commas and alphanumeric chars as words which isnt what we want

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

So as to keep the variables of the struct immutable
