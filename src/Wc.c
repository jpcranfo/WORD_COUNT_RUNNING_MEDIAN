#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


/*---C program to count words and find running median of words/line in text file
     by Jonathan Preston Cranford
     for Insight Data Engineering
     March 22, 2015
---*/


/*---Highlights of the program
	1)  Technical Highlight:  Dyamic Memory Allocation:  Functions AllOneDDoubleArr, AllOneDIntArr, AllTwoDChaArr, and AllOneDChaArr allocate small initial amount of memory for arrays, then grows the memory reserved for the array dynamically as words and lines of text file are processed.
	2)  Technical Highlight:  Search alogorithm:  I use the binary search algorithm to search through sorted arrays for inertion point more efficiently than just starting at beginning of array and going element by element.
---*/


/*---Summary of key program operations and functions
	1)  In function main(), allocate space for 1D and 2D arrays using functions AllOneDDoubleArr, AllOneDIntArr, and AllTwoDChaArr (AllOneDChaArr is used later)
	2)  In function main(), gets next word from file using function GetWordFrFile, finds place to insert it alphabetically in WordArr using function BinarySearchWord, and adds the word using function AddWord
	3)  In function main(), at the end of a line, finds words per line (WPL) of the line, finds insertion point using BinarySearchWPL, and inserts it in sorted array NumWPLArrSort using function AddNumWPL, then calculates running median and inserts it in running median array MedianArr using function FindMedian
---*/
   
   
   
   
   

/*---Declaration of functions
	AddNumWPL				Adds current number of words per line (CurrNumWPL) to sorted array of number of words per line (NumWPLArrSort), returns pointer to NumWPLArrSort with CurrNumWPL inserted
	AddWord					Adds current word (Word) to sorted array of words (WordArr), returns pointer to WordArr with Word inserted
	AllOneDChaArr				Allocates or reallocates memory of a 1D array of characters (OneDArr), returns pointer to allocated OneDArr
	AllOneDDoubleArr			Allocates or reallocates memory of a 1D array of doubles (OneDArr), returns pointer to allocated OneDArr
	AllOneDIntArr				Allocates or reallocates memory of a 1D array of integers (OneDArr), returns pointer to allocated OneDArr
	AllTwoDChaArr				Allocates or reallocates memory of a 2D array of characers representing array of words (WordArr), returns pointer to allocated WordArr
	BinarySearchWord			Performs binary search on array (WordArr) to find place to insert current word (Word) alphabetically, returns insertion index
	BinarySearchWPL				Performs binary search on array (NumWPLArrSort) to find place to insert current number of words per line (CurrNumWPL) sorted, returns insertion index
	CreateSpaceCurrWPL			Creates space in array NumWPLArrSort to insert current number of words per line (WPL) into the array, return pointer to NumWPLArrSort with newly created space
	CreateSpaceForWord			Creates space in array WordArr to insert current word into the array, returns pointer to WordArr with newly created space
	GetInputFilePointer			Open the input text file for reading, return pointer to the input file
	GetOutputFilePointer			Open output file (for either word count or running median) for writing, returns pointer to output file
	GetWordFrFile				Get the next word from input file (fp), returns pointer to the word
	FindMedian				Finds median from array NumWPLArrSort, returns pointer to median array with newly inserted running median value
	MaxInt					Finds maximum of two integers, returns that maximum
	Mean					Returns mean of two doubles, returns that mean
	RelMem1DChaArr				Releases memory reserved for a 1D character array
	RelMem1DDoubleArr			Releases memory reserved for a 1D double array
	RelMem1DIntArr				Releases memory reserved for a 1D integer array
	RelMem2DChaArr				Releases memory reserved for a 2D character array
---*/
int *AddNumWPL( int *NumWPLArrSort, int CurrNumWPL, int NumLines, int MaxNumLines, int InsIndWPL );
char **AddWord( char **WordArr, char *Word, int *NumWor, int *NumChaArr, int *MaxNumCha, int *MaxNumWor, int InsIndWord, int *WordCountArr );
char *AllOneDChaArr( char *OneDArr, int numrows, int *MaxNumCha, int Lengthen, int Shorten );
double *AllOneDDoubleArr( double *OneDArr, int numrows, int oldnumrows, int resize );
int *AllOneDIntArr( int *OneDArr, int numrows, int oldnumrows, int resize );
char **AllTwoDChaArr( char **WordArr, int WordLength, int *MaxNumWor, int *MaxNumCha, int *NumChaArr, int NumWor, int ResWor, int ResCha );
int BinarySearchWord ( char **WordArr, int inds, int inde, char *Word, int NumWor, int *NumChaArr );
int BinarySearchWPL ( int *NumWPLArrSort, int inds, int inde, int CurrNumWPL, int NumLines );
int *CreateSpaceCurrWPL( int *NumWPLArrSort, int MaxNumLines, int NumLines, int InsIndWPL );
char **CreateSpaceForWord( char **WordArr, int *MaxNumWor, int NumWor, int InsIndWord, int *NumChaArr, int *WordCountArr );
FILE *GetInputFilePointer ( FILE *fp );
FILE *GetOutputFilePointer ( FILE *fpout, int Med );
char *GetWordFrFile ( FILE *fp, int *AtEOF, int *MaxNumCha, int *NumWPLArr, int *NumLines, int *FinishedLine );
double *FindMedian( double *MedianArr, int NumLines, int *NumWPLArrSort );
int MaxInt( int a, int b );
double Mean( double Num1, double Num2 );
void RelMem1DChaArr( char *myarr );
void RelMem1DDoubleArr( double *myarr );
void RelMem1DIntArr( int *myarr );
void RelMem2DChaArr( char **myarr, int numrows );



/*---main():	Start of main function---*/
int main()
{
	/*---Define variables in function main---*/
	FILE *fp;							//Pointer to input text file
	FILE *fpoutwc;							//Pointer to output file for word count
	FILE *fpoutmed;							//Pointer to output file for running median
	char **WordArr;							//2D Array of characters, representing the words in the text file (1 row = 1 word, each column is character in word)
	int *NumChaArr;							//1D Arry of integers, representing the number of characters/word for each word
	int *WordCountArr;						//1D Arry of integers, representing the number of times word appears, for each word
	int *NumWPLArr;							//1D Arry of integers, representing the number of words/line (WPL) for each line
	int *NumWPLArrSort;						//1D Arry of integers, representing the number of words/line (WPL) for each line, sorted
	double *MedianArr;						//1D Arry of doubles, representing the running median as process lines
	char *Word;							//1D array of characters representing a word
	int MaxNumCha = 20;						//Maximum number of characters allowed for a word
	int MaxNumWor = 100;						//Maximum number of distinct words allowed in the text file
	int OldMaxNumWor;						//Saves old value of MaxNumWor
	int MaxNumLines = 50;						//Maximum number of lines allowed in text file (in terms of memory storage space)
	int NumLines = 0;						//Number of lines processed so far in text file
	int NumWor = 0;							//Number of distinct words processed so far in text file
	int TotNumWor = 0;						//Total number of words, distinct and non-distinct, in text file
	int InsIndWord = 0;						//Insert index for current word in the alphabetical array of words (WordArr)
	int InsIndWPL = 0;						//Insert index for current number of words per line (WPL) in the sorted array of WPL (NumWPLArrSort)
	int FinishedLine = 0;						//1 if finished processing current line of text file, 0 if not yet finished processing current line
	int AtEOF;							//1 if at end of file (EOF), 0 if not at EOF
	int ind;							//Generic use index in array
	char usrinp[100];						//Generic use string to get input from user from command line

	
	
	
	/*---Call functions to open files for reading/writing, and to allocate memory for arrays---*/
	fp = GetInputFilePointer( fp );										//Open input text file
	fpoutwc = GetOutputFilePointer( fpoutwc, 0 );								//Open output text file for word count
	fpoutmed = GetOutputFilePointer ( fpoutmed, 1 );							//Open output text file for running median

	NumChaArr = AllOneDIntArr( NumChaArr, MaxNumWor, MaxNumWor, 0 );					//Allocate memory for 1D array containing characters/word for each word
	WordCountArr = AllOneDIntArr( WordCountArr, MaxNumWor, MaxNumWor, 0 );					//Allocate memory for 1D array keeping track of appearance frequency of words 
	NumWPLArr = AllOneDIntArr( NumWPLArr, MaxNumLines, MaxNumLines, 0 );					//Allocate memory for 1D array keeping track of number words/line
	NumWPLArrSort = AllOneDIntArr( NumWPLArrSort, MaxNumLines, MaxNumLines, 0 );				//Allocate memory for 1D sorted array keeping track of number words/line 
	MedianArr = AllOneDDoubleArr( MedianArr, MaxNumLines, MaxNumLines, 0 );					//Allocate memory for 1D array of running median values
	WordArr = AllTwoDChaArr( WordArr, MaxNumCha, &MaxNumWor, &MaxNumCha, NumChaArr, NumWor, 0, 0 );		//Allocate and initialize memory for WordArr, which holds all words find in document



printf( "\n\n|--------------------------------------------------PROCESSING WORDS IN TEXT FILE--------------------------------------------------|\n\n" );
	do																//Loop to count words and find running median until reach EOF
	{
		/*---This first half of the loop is for calculating and storing running median of words/line---*/
		if ( NumLines == MaxNumLines )												//If number of lines reached space allocated for initial number of lines, need to reallocate space in arrays							
		{
			/*---Reallocating space for arrays---*/
			NumWPLArr = AllOneDIntArr( NumWPLArr, MaxNumLines+50, MaxNumLines, 1 );
			NumWPLArrSort = AllOneDIntArr( NumWPLArrSort, MaxNumLines+50, MaxNumLines, 1 );
			MedianArr = AllOneDDoubleArr( MedianArr, MaxNumLines+50, MaxNumLines, 1 );
			MaxNumLines += 50;												//Increasing maximum number of lines allocated space in memory
		}
		
		Word = GetWordFrFile( fp, &AtEOF, &MaxNumCha, NumWPLArr, &NumLines, &FinishedLine );					//Get the next word from input text file
		if( !AtEOF || ( AtEOF && strlen( Word ) > 0 ) )
			fprintf ( stderr, "\rProcessing words in text file on line #: %d",NumLines );					//Print status of counting words in real time
		
		if ( FinishedLine )													//If finished processing all words in line, then calculate running median
		{
			if ( NumLines > 1 )												//If processing 2nd line or later, do a binary search to find where sorted number of words/per line should go in the array, return the index as InsIndWPL
				InsIndWPL = BinarySearchWPL ( NumWPLArrSort, 0, NumLines-2, NumWPLArr[NumLines-1], NumLines );		//Find insert index InsIndWPL
		
			NumWPLArrSort = AddNumWPL( NumWPLArrSort, NumWPLArr[NumLines-1], NumLines, MaxNumLines, InsIndWPL );		//Insert words per line number into sorted array of words per line
			MedianArr = FindMedian( MedianArr, NumLines, NumWPLArrSort );							//Find running median and store in 1D array MedianArr
			FinishedLine = 0;												//Reset whether of not finished with reading words from current line to 0, which means not yet finished
		}
		
		/*---This second half of the loop is for storing and sorting words, and finding frequency of appearance of words---*/
		if ( strlen( Word ) > 0)												//If length of word > 0 (i.e., not have just white space), then process word
		{
			if ( NumWor == MaxNumWor )											//If number of words reached space allocated for initial number of words, need to reallocate space in arrays
			{
				OldMaxNumWor = MaxNumWor;										//Storing old max number of words able to store in array
				/*---Reallocating space for arrays---*/
				WordArr = AllTwoDChaArr( WordArr, strlen(Word), &MaxNumWor, &MaxNumCha, NumChaArr, NumWor, 1, 0 );
				NumChaArr = AllOneDIntArr( NumChaArr, MaxNumWor, OldMaxNumWor, 1 );
				WordCountArr = AllOneDIntArr( WordCountArr, MaxNumWor, OldMaxNumWor, 1 );
			}
			if ( NumWor > 0 )												//If number of words processed > 0, then do binary search to find index where word should be inserted in alphabetical order
				InsIndWord = BinarySearchWord( WordArr, 0, NumWor-1, Word, NumWor, NumChaArr );				//Find insertion index
			WordArr = AddWord( WordArr, Word, &NumWor, NumChaArr, &MaxNumCha, &MaxNumWor, InsIndWord, WordCountArr );	//Add current word at appropriate spot in the alphabetical array of words
		}
	} while ( !AtEOF );
	
	
	
	printf( "\n\n|--------------------------------------------------A FEW NUMBERS YOU MIGHT LIKE TO KNOW--------------------------------------------------|\n\n" );
	
	printf("OK, all done processing words in text files\n\n");
	
	/*---Find total number of words in text files, and output word and word frequency appearance to wc_result.txt, and output running median to med_result.txt---*/
	for ( ind=0; ind<NumWor; ind++ )
	{
		TotNumWor += WordCountArr[ind];
		fprintf( fpoutwc,"%s\t%d\n",WordArr[ind],WordCountArr[ind] );
	}
	for ( ind = 0; ind < NumLines; ind++ )
		fprintf( fpoutmed,"%.1f\n",trunc( MedianArr[ind]*10.0 )/10.0 );
	
	printf("Here are a few numbers you might like to know:\n");
	printf("\tTotal number of distinct words in text files is %d, and total number of all words, even non-distinct, is %d\n",NumWor, TotNumWor);
	printf("\tCumulative number of lines in text files is %d\n",NumLines);
	printf("\n\nOkay, all done, word count results are in wc_result.txt and running median results are in med_result.txt, both in folder wc_output, have a nice day!  Press enter to exit program:  ");
	gets( usrinp );
	printf("\n\n\n");



	/*---Free some memory by closing files and releasing memory from 1D and 2D arrays---*/
	fclose( fp );
	fclose( fpoutwc );
	fclose( fpoutmed );
	RelMem1DIntArr( NumWPLArr );
	RelMem1DIntArr ( NumWPLArrSort );
	RelMem1DIntArr( WordCountArr );
	RelMem1DIntArr( NumChaArr );
	RelMem1DDoubleArr( MedianArr );
	RelMem1DChaArr( Word );
   
   return 0;
}



/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------My Functions---------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*---GetInputFilePointer:		Opens the file with combined text from all text files in it for reading, and return pointer to the file---*/
FILE *GetInputFilePointer ( FILE *fp )
{
	/*---Define variables in function GetInputFilePointer---*/
	char usrinp[100];								//Generic use string to get input from user from command line
	
	
	
	do										//Loop to try opening input file until file name exists or user exits program
	{
		printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n|--------------------------------------------------OBTAINING INPUT TEXT FILE--------------------------------------------------|\n\n" );
		printf( "Attempting to read file ./wc_input/COMB_FILE/ComFil.txt, which has the combined text of all text files in directory wc_input, and should have been created at the beginning of executing run.sh ...\n\n");
		
		fp = fopen( "wc_input/COMB_FILE/ComFil.txt","r" );			//Open the input file
		if ( fp == NULL )							//Check for errors opening input file
		{
			printf( "\n\n\t\tOOPS!  The file ComFil.txt in ./wc_input/COMB_FILE/ does not exist for some reason.  Hit enter to try reading it again, or type 0 and press enter to exit\n" );
			gets( usrinp );
			if ( strlen( usrinp ) == 1 && usrinp[0] == '0' )		//Exit program if user enters a 0
			{
				printf("\n\n\nGoodbye, enjoy!\n\n\n");
				exit(0);
			}
			printf( "\n\n\n\n\n\n" );
		}
	}
	while ( fp == NULL );
	printf( "\t\tOK, I've got the file ComFile.txt, which contains combined text of all text files\n\n" );
	
	return fp;									//Return the pointer to the file to main()
}



/*---GetOutputFilePointer:		Opens the files for writing word count and running median to, and return pointer to the files---*/
FILE *GetOutputFilePointer ( FILE *fpout, int Med )
{
	/*---Define variables in function GetFilePointer---*/
	char usrinp[100];								//Generic use string to get input from user from command line
	
	
	
	do										//Loop to try opening output file until file name exists or user exits program
	{
		printf( "\n\n|--------------------------------------------------OPENING TEXT FILES FOR WRITING OUTPUT--------------------------------------------------|\n\n" );
		if ( !Med )								//Switch, if Med == 0 then open word count file, otherwise open running median file
		{
			printf( "Attempting to open file ./wc_output/wc_result.txt ...\n\n");
			fpout = fopen( "wc_output/wc_result.txt","w" );			//Open the output file
		
			if ( fpout == NULL )						//Check for errors opening output file
			{
				printf( "\n\n\t\tOOPS!  Cannot open wc_output/wc_result.txt for writing, check that folder wc_output exists.  Hit enter to try reading it again, or type 0 and press enter to exit\n" );
				gets( usrinp );
				if ( strlen( usrinp ) == 1 && usrinp[0] == '0' )	//Exit program if user enters a 0
				{
					printf("\n\n\nGoodbye, enjoy!\n\n\n");
					exit(0);
				}
				printf( "\n\n\n\n\n\n" );
			}
		}
		else
		{
			printf( "Attempting to open file ./wc_output/med_result.txt ...\n\n");
			fpout = fopen( "wc_output/med_result.txt","w" );		//Open the output file
		
			if ( fpout == NULL )						//Check for errors opening output file
			{
				printf( "\n\n\t\tOOPS!  Cannot open wc_output/med_result.txt for writing, check that folder wc_output exists.  Hit enter to try reading it again, or type 0 and press enter to exit\n" );
				gets( usrinp );
				if ( strlen( usrinp ) == 1 && usrinp[0] == '0' )	//Exit program if user enters a 0
				{
					printf("\n\n\nGoodbye, enjoy!\n\n\n");
					exit(0);
				}
				printf( "\n\n\n\n\n\n" );
			}
		}	
	}
	while ( fpout == NULL );
	
	if ( !Med )
		printf( "\t\tOK, I've opened wc_result.txt for writing word count answers to\n\n" );
	else
		printf( "\t\tOK, I've opened med_result.txt for writing running median answers to\n\n" );
	
	return fpout;									//Return the pointer to the file to main()	
}



/*---AllOneDIntArr:		Allocate or reallocate memory for OneDArr of integers, returns pointer to 1D array---*/
int *AllOneDIntArr( int *OneDArr, int numrows, int oldnumrows, int resize )
{
	/*---Define variables in function AllOneDIntArr---*/
	int *tmp;									//Temporary 1D array for use in resizing OneDArr, so not run chance of corrupting memory during realloc
	int row;									//Generic use row counter in a 1D array
	
	
	
	/*---If not reallocate arrays then do first procedure, else, then reallocate arrays in second procedures---*/
	if ( !resize )
	{
		OneDArr = (int *)malloc( numrows*sizeof(int) );				//Allocating space for 1D array of integers
		
		/*---Checking for memory allocation errors---*/
		if ( OneDArr == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for OneDArr (a 1D array of integers), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		for ( row=0; row<numrows; row++ )					//Loop to initialize values in OneDArr	
		{
			OneDArr[row] = 0;
		}  
	}
	else
	{
		tmp = (int *)realloc( OneDArr, numrows*sizeof(int) );
		
		/*---Checking for memory allocation errors---*/
		if ( tmp == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for array tmp (a 1D array of integers), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		for ( row=oldnumrows; row<numrows; row++ )				//Loop to initialize values in tmp	
		{
			tmp[row] = 0;
		}  
		
		OneDArr = tmp;
	}
	
	return OneDArr;									//Return the 1D array, now allocated with space in memory
}



/*---AllOneDDoubleArr:		Allocate or reallocate memory for OneDArr of doubles, returns pointer to 1D array---*/
double *AllOneDDoubleArr( double *OneDArr, int numrows, int oldnumrows, int resize )
{
	/*---Define variables in function AllOneDDoubleArr---*/
	double *tmp;									//Temporary 1D array for use in resizing OneDArr, so not run chance of corrupting memory during realloc
	int row;									//Generic use row counter in a 1D array
	
	
	
	/*---If not reallocate arrays then do first procedure, else, then reallocate arrays in second procedures---*/
	if ( !resize )
	{
		OneDArr = (double *)malloc( numrows*sizeof(double) );			//Allocating space for 1D array of doubles
		
		/*---Checking for memory allocation errors---*/
		if ( OneDArr == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for OneDArr (a 1D array of doubles), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		for ( row=0; row<numrows; row++ )					//Loop to initialize values in OneDArr	
		{
			OneDArr[row] = 0.0;
		}  
	}
	else
	{
		tmp = (double *)realloc( OneDArr, numrows*sizeof(double) );
		
		/*---Checking for memory allocation errors---*/
		if ( tmp == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for tmp (a 1D array of doubles), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		for ( row=oldnumrows; row<numrows; row++ )				//Loop to initialize values in tmp	
		{
			tmp[row] = 0.0;
		}  
		
		OneDArr = tmp;
	}
	
	return OneDArr;									//Return the 1D array to main(), now allocated with space in memory
}



/*---AllOneDChaArr:		Allocate or reallocate memory for OneDArr of characters, returns pointer to 1D array---*/
char *AllOneDChaArr( char *OneDArr, int numrows, int *MaxNumCha, int Lengthen, int Shorten )
{
	/*---Define variables in function AllOneDCharArr---*/
	char *tmp;									//Temporary 1D array for use in resizing OneDArr, so not run chance of corrupting memory during realloc
	int row;									//Generic use row counter in a 1D array
	
	
	
	/*---If not reallocate arrays then do first procedure, else if want to shorten the array then do second procedure, else we are lengthening the array---*/
	if ( !Lengthen && !Shorten )
	{
		OneDArr = (char *)malloc( numrows*sizeof(char) );			//Allocate space for 1D array of characters
		
		/*---Checking for memory allocation errors---*/
		if ( OneDArr == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for OneDArr (a 1D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		for ( row=0; row<numrows; row++ )					//Loop to initialize values in OneDArr	
		{
			OneDArr[row] = '0';
		}  
	}
	else if( Shorten )								//Make array shorter, use less memory
	{
		tmp = (char *)malloc( (numrows)*sizeof(char) );
		strncpy( tmp, OneDArr, numrows );
		tmp[numrows-1] = (char)0;
		
		/*---Checking for memory allocation errors---*/
		if ( tmp == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for tmp (a 1D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		OneDArr = tmp;
	}
	else										//Make array longer, use more memory
	{
		tmp = (char *)malloc( (numrows)*sizeof(char) );
		strncpy( tmp, OneDArr, numrows );
		for ( row=*MaxNumCha; row<numrows; row++ )
			tmp[row] = '0';
		
		/*---Checking for memory allocation errors---*/
		if ( tmp == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for tmp (a 1D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		
		OneDArr = tmp;
	}
	
	return OneDArr;									//Return the 1D array to main(), now allocated with space in memory
}



/*---AllTwoDChaArr:		Allocate or reallocate memory for WordArr, returns pointer to 2D character array---*/
char **AllTwoDChaArr( char **WordArr, int WordLength, int *MaxNumWor, int *MaxNumCha, int *NumChaArr, int NumWor, int ResWor, int ResCha )
{
	/*---Define variables in function AllTwoDChaArr---*/
	char **tmp;									//Temporary 2D array for use in resizing WordArr, so not run chance of corrupting memory during realloc
	int row, col;									//Generic use row and column counter in a 2D array
	int OldMaxNumWor = *MaxNumWor;							//Old max number of words allocated in memory of 2D array WordArr
	int OldMaxNumCha = *MaxNumCha;							//Old max number of characters/word allocated in memory of 2D array WordArr
	
	
	
	/*---If not reallocate arrays then do first procedure, else if want to lengthen max number of characters/word then do second procedure, else if reallocate max number of words then do third procedure---*/
	if ( !ResWor && !ResCha)
	{
		WordArr = (char **)malloc( *MaxNumWor*sizeof(char *) );			//2D array of character, allocating space for rows (words)
		
		/*---Checking for memory allocation errors---*/
		if ( WordArr == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordArr (2D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		/*---Allocate memory for columns in WordArr, and initialize to '0'---*/
		for ( row=0; row<*MaxNumWor; row++ )
		{
			WordArr[row] = (char *)malloc( WordLength*sizeof(char) );			
			if ( WordArr[row] == NULL )					//Checking for memory allocation errors
			{
				fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordArr (2D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 ) ;
			}
		}
		for ( row=0; row<*MaxNumWor; row++ )					//Loop to initialize values in WordArr	
		{
			for ( col=0; col<WordLength; col++ )
				WordArr[row][col] = '0';
		}  
	}
	else if ( ResCha )
	{
		tmp = (char **)realloc( WordArr, *MaxNumWor*sizeof(char *) );
		
		/*---Checking for memory allocation errors---*/
		if ( WordArr == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordArr (2D array of characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		/*---Allocate memory for columns in WordArr, and initialize to '0'---*/
		for ( row=0; row<*MaxNumWor; row++ )
		{
			tmp[row] = (char *)malloc( WordLength*sizeof(char) );
			tmp[row] = (char *)realloc( WordArr[row], WordLength*sizeof(char) );	
			if ( tmp[row] == NULL )						//Checking for memory allocation errors
			{
				fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordArr (2D array of chracters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 ) ;
			}
		}
		
		*MaxNumCha = WordLength;
		WordArr = tmp;
	}
	else if ( ResWor )
	{
		*MaxNumWor = *MaxNumWor + 100;						//Add 100 to the max number of words WordArr can hold
					
		tmp = (char **)realloc( WordArr, *MaxNumWor*sizeof(char *) );
		
		/*---Checking for memory allocation errors---*/
		if ( tmp == NULL )
		{
			fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for tmp (temporary 2D array to hold characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
			exit( 1 );
		}
		
		/*---Allocate memory for columns in tmp for new rows---*/
		for ( row=OldMaxNumWor; row<*MaxNumWor; row++ )
		{
			tmp[row] = (char *)malloc( *MaxNumCha*sizeof(char) );	
			if ( tmp[row] == NULL )						//Checking for memory allocation errors
			{
				fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for tmp (temporary 2D arry to hold characters), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
				exit( 1 ) ;
			}
		}
		for ( row=OldMaxNumWor; row<*MaxNumWor; row++ )				//Loop to initialize values in tmp	
		{
			for ( col=0; col<*MaxNumCha; col++ )
				tmp[row][col] = '0';
		}  
		
		WordArr = tmp;
	}
	
	return WordArr;									//Return the 2D array to main(), now allocated with space in memory
}



/*---GetWordFrFile:		Get the next word from the file ComFil.txt---*/
char *GetWordFrFile ( FILE *fp, int *AtEOF, int *MaxNumCha, int *NumWPLArr, int *NumLines, int *FinishedLine )
{
	int c = 0;										//Decimal value of read character
	int ct = 0;										//Generic counter
	char *Word;										//A word from file
	int NumCha = 0;										//Number of characters in current word
	
	
	
	/*---Initialize some variables---*/
	Word = AllOneDChaArr( Word, *MaxNumCha, MaxNumCha, 0, 0 );				//Allocate space for holding word
	
	/*---Get a word---*/
	while ( c != EOF )									//Grab characters of word from text file while not EOF
	{
		c = fgetc( fp );
		if ( (c >= 65 && c <= 90) || (c >= 97 && c <= 122) )				//Only valid characters in word are upper or lowercase letters
		{
			if ( ct == *MaxNumCha )							//If this word has too many characters, increase space allocated to word
				Word = AllOneDChaArr( Word, *MaxNumCha+*MaxNumCha, MaxNumCha, 1, 0 );
			if ( c >= 65 && c <= 90 )						//Upper and lowercase are equivalent, and want lowercase
				c = c + 32;
			Word[ct] = (char)c;
			ct++;
		}
		else if ( ct > 0 && ( (c>=0 && c<=32) || c==EOF ))				//If get characters that delineate words, then now have entire word and break out of loop of collecting characters
		{
			NumWPLArr[*NumLines] += 1; 
			if ( c == 10 || c==13 || c==EOF )
			{
				*NumLines += 1;
				*FinishedLine = 1;
			}
			break;				
		}
		else if ( ct == 0 && ( c == 10 || c == 13 || c==EOF ) )				//Deal with encountering blank lines, don't count them as word, but count them in running median
		{
			if ( c != EOF )
			{
				*NumLines += 1;
				*FinishedLine = 1;
			}
			break;
		}
	}
	NumCha = ct+1;										//Just accounting for array index starting at 0 in C programming
	Word = AllOneDChaArr( Word, NumCha, MaxNumCha, 0, 1 );
	if ( c == EOF )
		*AtEOF = 1;
	else 
		*AtEOF = 0;
		
	return Word;										//Return the current word in the text file to main()
}



/*---BinarySearchWPL:		Perform binary search for specific words per line (WPL) in array of WPL---*/
int BinarySearchWPL ( int *NumWPLArrSort, int inds, int inde, int CurrNumWPL, int NumLines )
{
	/*---Define variables in function BinarySearchWPL---*/
	int indm;										//Index of middle of integer array
	
	
	
	/*---Calculate midpoint index, and recursive call based on where target word is with respect to inds and inde; return indm if found target word---*/
	indm = (int) ( ( inds + inde ) / 2 );
	if ( inds == inde )									//If not find current number of WPL in sorted array of WPL, return proper index for insertion
	{
		if ( CurrNumWPL > NumWPLArrSort[inds] )
			return inds+1;
		else if ( CurrNumWPL < NumWPLArrSort[inds] )
			return inds;
		else if ( CurrNumWPL == NumWPLArrSort[inds] )
			return inds;
	}
	else											//Else, keep searching for the insertion index
	{
		if ( CurrNumWPL < NumWPLArrSort[0] )						//If current number of WPL is less than minimum value in sorted array of WPL, return insertion point as beginning index
			return 0;
		if ( CurrNumWPL > NumWPLArrSort[NumLines-2] )					//If current number of WPL is greater than maximum value in sorted array of WPL, return insertion point as end index
			return NumLines-1;
		if ( CurrNumWPL == NumWPLArrSort[inds] )					//If current number of WPL is equal to that at starting index (inds), then return inds as the insertion index
			return inds;
		else if ( CurrNumWPL == NumWPLArrSort[inde] )					//If current number of WPL is equal to that at ending index (inde), then return inde as the insertion index
			return inde;
		else if ( NumWPLArrSort[indm] > CurrNumWPL )					//If value at middle index (indm) greater than current number of WPL, then recursive call to BinarySearchWPL with new ending index as indm-1
			if ( indm == inds )
				return inds;
			else
				return BinarySearchWPL( NumWPLArrSort, inds, indm-1, CurrNumWPL, NumLines );
		else if ( NumWPLArrSort[indm] < CurrNumWPL )					//If value at middle index (indm) less than current number of WPL, then recursive call to BinarySearchWPL with new starting index as indm+1
			if ( indm == inde )
				return inde;
			else
				return BinarySearchWPL( NumWPLArrSort, indm+1, inde, CurrNumWPL, NumLines );
		else
			return indm;								//If WPL at indm == current number of words per line, then return indm as the insertion index
	}
}



/*---AddNumWPL:			Add current number of words per line (WPL) to the array of sorted WPL---*/
int *AddNumWPL( int *NumWPLArrSort, int CurrNumWPL, int NumLines, int MaxNumLines, int InsIndWPL )
{
	CreateSpaceCurrWPL( NumWPLArrSort, MaxNumLines, NumLines-1, InsIndWPL );		//Create space in array of sorted number of WPL to add the current number of WPL
	NumWPLArrSort[InsIndWPL] = CurrNumWPL;							//Add the current num WPL to sorted array of number of WPL
	
	return NumWPLArrSort;									//Return the sorted array of WPL to main(), now with WPL of a new line in text file
}



/*---CreateSpaceCurrWPL:		Create space for adding current number of words per line (WPL) to array of sorted number of WPL---*/
int *CreateSpaceCurrWPL( int *NumWPLArrSort, int MaxNumLines, int NumLines, int InsIndWPL )
{
		/*---Define variables in function CreateSpaceCurrWPL---*/
		int ind;									//Generic index in array
		
		
		
		/*---Move down all elements from end of sorted array up to insertion index, to make space for adding current number of WPL---*/
		if ( NumLines > 0 )
		{
			for ( ind=NumLines; ind>=InsIndWPL; ind-- )
			{
				NumWPLArrSort[ind] = NumWPLArrSort[ind-1];
				
				if ( NumWPLArrSort == NULL )					//Checking for memory allocation errors
				{
					fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for NumWPLArrSort (1D arry to hold orderd list of number words per line), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
					exit( 1 ) ;
				}
			}
		}
		
		return NumWPLArrSort;								//Return the sorted number of WPL, now with space to add a new WPL
}



/*---FindMedian:		Find median value of WPL up to line NumLines and store in array MedianArr---*/
double *FindMedian( double *MedianArr, int NumLines, int *NumWPLArrSort )
{
	/*---Define variables in function FindMedian---*/
	int Mid;										//Middle index in the array of lines processed so far
	double HalfNumLines = (double)(NumLines/2.0);						//Half of the number of lines processed so far
	
	

	if ( fabs(HalfNumLines-round(HalfNumLines)) < 1e-10 )					//If have even number of lines processed so far, find median by using mean
	{
		Mid = ( int )( floor( HalfNumLines ) );
		MedianArr[NumLines-1] = Mean( (double)NumWPLArrSort[Mid-1], (double)NumWPLArrSort[Mid] );
	}
	else											//Else if have odd number of lines processed so far, find median as value in middle of array
	{
		Mid = ( int )( floor( HalfNumLines ) );
		MedianArr[NumLines-1] = (double)NumWPLArrSort[Mid];
	}
	
	return MedianArr;									//Return the array of running median values, now with new running median value
		
}



/*---Mean:			Find mean of two double numbers---*/
double Mean( double Num1, double Num2 )
{
	return ( (Num1 + Num2)/2.0 );
}



/*---BinarySearchWord:		Perform binary search for index to insert word in alphabetical order---*/
int BinarySearchWord ( char **WordArr, int inds, int inde, char *Word, int NumWor, int *NumChaArr )
{
	/*---Define variables in function BinarySearchWord---*/
	int indm;										//Index of middle of array of words
	
	
	
	/*---Calculate midpoint index, and recursive call based on where target word is with respect to inds and inde; return indm if found target word---*/
	indm = (int) ( ( inds + inde ) / 2 );
	if ( inds == inde )										//If not find current word in sorted array of words, return proper index for insertion
	{
		if ( strncmp( Word,WordArr[inds],MaxInt( strlen(Word),NumChaArr[inds] ) ) > 0 )
			return inds+1;
		else if ( ( strncmp( Word,WordArr[inds],MaxInt( strlen(Word),NumChaArr[inds] ) ) < 0 ) )
			return inds;
		else if ( strncmp( Word,WordArr[inds],MaxInt( strlen(Word),NumChaArr[inds] ) ) == 0  )
			return inds;
	}
	else													//Else, keep searching for the insertion index
	{
		if ( strncmp( Word,WordArr[0],MaxInt( strlen(Word),NumChaArr[0] ) ) < 0 )			//If current word is less than minimum value in sorted array of words (alphabetically), return insertion point as beginning index
			return 0;
		if ( strncmp( Word,WordArr[NumWor-1],MaxInt( strlen(Word),NumChaArr[NumWor-1] ) ) > 0 )		//If current word is greater than maximum value in sorted array of words (alphabetically), return insertion point as end index
			return NumWor;
		if ( strncmp( Word,WordArr[inds],MaxInt( strlen(Word),NumChaArr[inds] ) ) == 0 )		//If current word is equal to word in start index (inds), return insertion point as inds
			return inds;
		else if ( strncmp( Word,WordArr[inde],MaxInt( strlen(Word),NumChaArr[inde] ) ) == 0 )		//If current word is equal to word in end index (inde), return insertion point as inde
			return inde;
		else if ( strncmp( WordArr[indm],Word,MaxInt( strlen(Word),NumChaArr[indm] ) ) > 0 )		//If word at middle index (indm) greater than current word, then recursive call to BinarySearchWord with new ending index as indm-1
			if ( indm == inds )
				return inds;
			else
				return BinarySearchWord( WordArr, inds, indm-1, Word, NumWor, NumChaArr );
		else if ( strncmp( WordArr[indm],Word,MaxInt( strlen(Word),NumChaArr[indm] ) ) < 0 )		//If word at middle index (indm) less than current word, then recursive call to BinarySearchWord with new starting index as indm+1
			if ( indm == inde )
				return inde;
			else
				return BinarySearchWord( WordArr, indm+1, inde, Word, NumWor, NumChaArr );	
		else
			return indm;										//If word at indm == current word, then return indm as the insertion index
	}
}



/*---MaxInt:			Find maximum of two integers---*/
int MaxInt( int a, int b )
{
	if ( a >= b )
		return a;
	else
		return b;
}



/*---AddWord:		Add the current word to the array of alphabetical words (WordArr)---*/
char **AddWord( char **WordArr, char *Word, int *NumWor, int *NumChaArr, int *MaxNumCha, int *MaxNumWor, int InsIndWord, int *WordCountArr )
{
	if ( strlen( Word ) > 0 && strncmp( Word, WordArr[InsIndWord], MaxInt( strlen(Word),NumChaArr[InsIndWord] ) ) != 0 )		//Add word to array of words (WordArr) if not already there
	{
		CreateSpaceForWord( WordArr, MaxNumWor, *NumWor, InsIndWord, NumChaArr, WordCountArr );					//Create space in WordArr to add current word
		if ( strlen(Word) > *MaxNumCha )											//If current word longer than all rest encountered so far, increase memory space for holding words in WordArr
			WordArr = AllTwoDChaArr( WordArr, strlen(Word), MaxNumWor, MaxNumCha, NumChaArr, *NumWor, 0, 1 );	
		WordArr[InsIndWord] = Word;												//Add word to alphabetical WordArr
		NumChaArr[InsIndWord] = strlen( Word );											//Add number of characters in word to NumChaArr
		WordCountArr[InsIndWord] = 1;												//Add frequency of word appearance to WordCountArr
		*NumWor = *NumWor + 1;													//Increase number of words processed
	}
	else if ( strlen( Word ) > 0 && strncmp( Word, WordArr[InsIndWord], MaxInt( strlen(Word),NumChaArr[InsIndWord] ) ) == 0 )	//If word already there, just increase counter for frequency of that word appearance
	{
		WordCountArr[InsIndWord] = WordCountArr[InsIndWord] + 1;
	}
	
	return WordArr;															//Return alphabetical array of words (WordArr) with new word inserted to main()
}



/*---CreateSpaceForWord:		Create space for adding current word to array of sorted words (WordArr)---*/
char **CreateSpaceForWord( char **WordArr, int *MaxNumWor, int NumWor, int InsIndWord, int *NumChaArr, int *WordCountArr )
{
		/*---Define variables in function CreateSpaceForWord---*/
		int ind;											//Generic index in array

		
			
		/*---Move down all elements from end of sorted arrays up to insertion index, to make space for adding current word---*/
		if ( NumWor > 0 )
		{
			for ( ind=NumWor; ind>=InsIndWord; ind-- )
			{
				WordArr[ind] = WordArr[ind-1];
				NumChaArr[ind] = NumChaArr[ind-1];
				WordCountArr[ind] = WordCountArr[ind-1];
				
				if ( WordArr[ind] == NULL )							//Checking for memory allocation errors
				{
					fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordArr (2D arry to hold words), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
					exit( 1 ) ;
				}
				
				if ( NumChaArr == NULL )							//Checking for memory allocation errors
				{
					fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for NumChaArr (1D arry to hold number of characters for each word), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
					exit( 1 ) ;
				}
				
				if ( WordCountArr == NULL )							//Checking for memory allocation errors
				{
					fprintf( stderr,"\n\n\nERROR - unable to allocate required memory for WordCountArr (1D arry to hold number of times each word appears in text files), try freeing up some memory by closing other programs and try again.  Now exiting\n\n\n" );
					exit( 1 ) ;
				}
			}
		}
}



/*---RelMem2DChaArr:		Releases memory reserved for a 2D character array---*/
void RelMem2DChaArr( char **myarr, int numrows )
{
	/*---Define variables in function RelMem2DChaArr---*/
	int row;												//Generic use row in 2D array (row)
	
	
	
	for ( row=0; row<numrows; row++ )
 		 free( myarr[row] );
	free(myarr);
}



/*---RelMem1DIntArr:		Releases memory reserved for a 1D integer array---*/
void RelMem1DIntArr( int *myarr )
{
	free(myarr);
}



/*---RelMem1DChaArr:		Releases memory reserved for a 1D character array---*/
void RelMem1DChaArr( char *myarr )
{
	free(myarr);
}



/*---RelMem1DDoubleArr:		Releases memory reserved for a 1D double array---*/
void RelMem1DDoubleArr( double *myarr )
{
	free(myarr);
}
