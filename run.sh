#!/usr/bin/bash

# Combine all text files in wc_input into one text file "ComFil.txt" in the COMB_FILE directory
cd wc_input
mkdir COMB_FILE
cat *.* > COMB_FILE/ComFil.txt

# Give proper permissions
chmod a+rx *
chmod u+w *
cd ../
chmod a+rx *
chmod u+w *

# Make directory wc_output in case it doesn't exist
mkdir wc_output

# Compile and run the C program that counts the words and finds running median
gcc -o Wc src/Wc.c -lm
Wc
