# WORD_COUNT_RUNNING_MEDIAN
This collection of files and folder contains the complete program to count words in text files and calculate running median of words/line with each line in text files


Typical way to compile and run Template from Linux-like environment - type the following (minus "Step X)) at command line is ./run.sh



	How to use this program:  three options for input text files
	
1)  Run as is, and it grabs text from four input files, each containing one of the sentences given in the Insight Data Engineering project description.  On a PC Intel Core Duo CPU, 2 GHz, 3 GB of RAM, the run time was nearly instantaneous.

2)  Working in wc_input directory, remove all *.txt files in the folder, and in their place, copy the two files in directory LOREM_IPSUM into wc_input/.  These two text files are a bit longer, and show the program can handle modest size text files.  On a PC Intel Core Duo CPU, 2 GHz, 3 GB of RAM, the run time was a few seconds.

3)  Working in wc_input directory, remove all *.txt files in the folder, and in their place, copy the one file in directory LARGE_FILE into wc_input/.  This text file is pretty large, around 980 kb.  It shows my program can handle even larger text files.  It can handle even larger text files too, but at some point it would be prudent to encorporate parallel computing.  Thus, my next step in making this program even better would be to encorporate parallel computing capabilities for very large files.  On a PC Intel Core Duo CPU, 2 GHz, 3 GB of RAM, the run time was about 1 min. 40 sec.



	Initial list of files and folders, and descriptions:
	
Base folder

run.sh:		 	 The Linux or Linux-like shell script to run the entire C program

src:	 			 Contains C source code
 
wc_input:			 Contains the input text files
 
wc_output:			 Folder that stores output files, which are wc_result.txt and med_result.txt
 
src/

Wc.c		 		 The C source code
 
wc_input/

Line1.txt .. Line4.txt:	 Four text files, each containing one sentence from the four sentence example given in Insight Data Engineering project description
 
LOREM_IPSUM/:			 Contains two text files, generated using Lorem Ipsum machine:  http://www.lipsum.com/
 
LARGE_FILE/:			 Contains one large text file that is 980 kb, generated using a random nonsense word generator I developed in C (not included)
 
 
wc_input/LOREM_IPSUM/:

LoremIpsum1.txt:		 Contains words generated using Lorem Ipsum machine:  http://www.lipsum.com/
 
LoremIpsum2.txt:		 Contains more words generated using Lorem Ipsum machine:  http://www.lipsum.com/
 
wc_input/LARGE_FILE/:
 RandomSentences.txt:		 A 980 kb file of nonsense words (with spaces and line feeds), generated using a C program I developed in C (not included)

 
 
 
	Files and folders that Template creates:
	
Wc:		 		 The executable generated from compiling C code

wc_output/wc_result.txt:	 The word count results

wc_output/med_result.txt:	 The running median of words/line results

wc_input/COMB_FILE/ComFil.txt	 Program creates directory COMB_FILE, and within it, creates ComFil.txt, which is the combined text of all text files in wc_input/ directory


	
