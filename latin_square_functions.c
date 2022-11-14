/******************************************************************************
 * @file: latin_square_functions.c
 *
 * jgermano
 * jgermano
 * 907 811 3306
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 * Added include string.h
 * Changed the return statement for the verify rows and columns to be
 * a variable that will be set to 0 or 1 based on if there are duplicates
 * instead of it just returning 0 at the end
 *
 * @creator: YOUR PREFERED NAME (YOUR WISC EMAIL)
 * @modified: SUBMISSION DATE
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "latin_square_functions.h"

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

// ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE 

/******************************************************************************
 * Globals
 *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

/******************************************************************************
 * Helper functions
 *****************************************************************************/

// ADD ANY HELPER FUNCTIONS YOU MIGHT WRITE HERE

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * There are many approaches that will work to read in the latin square data.
 * In any approach you choose, you will need to do at least the following:
 *     1) open the file 
 *     2) read in the text from the file
 *     3) figure out the dimensions of the latin square (n)
 *     4) reserve memory for the latin_square. This requires 2 steps
 *         4a) reserve an array of pointers to the rows
 *         4b) reserve an array of characters for each row
 *     5) fill in the latin_square data structure 
 *     6) close the file
 *
 * @param filename The name of the file to read in
 * @param latin_square_in A pointer to the latin square variable in main
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 */
void Read_Latin_Square_File(const char *filename, 
                            char ***latin_square_in, 
                            size_t *n) {
    /* BEGIN MODIFYING CODE HERE */
    FILE *file;
    file = fopen(filename, "r");

    //Make sure we have a valid file
    if(file == NULL){
        *latin_square_in = NULL;
        *n = 0;
        printf("Error with opening file, try retyping file name");
    }

    //Create a string for the line that is being read and allocate memory for it
    //I set it to 94 as the specification said for a valid square the max size is 94x94
    char *inputLine;
    inputLine = (char *)malloc(1002*sizeof(char));

    //Deal with the case of an empty file
    if(fgets(inputLine, 1002, file) == NULL){
        *latin_square_in = NULL;
        *n = 0;
        return;
    }

    //Must decrement by one to account for the /n character at the end of each line
    *n = strlen(inputLine) - 1;

    //reserve an array of pointers to the rows
    *latin_square_in = (char **)malloc((*n)* sizeof(char *));

    //Need to allocate memory for characters too
    for(int i = 0; i < *n; i++){
        *(*latin_square_in + i) = (char *)malloc((*n)* sizeof(char));
    }

    //Populate the latin square
    for(int i = 0; i < *n; i++){
        for(int j = 0; j < *n; j++){
            //Retrieve the character from the first row then go row by row
            *(*(*latin_square_in + i) + j) = *(inputLine + j);
        }
        fgets(inputLine, 1002, file);
    }
    fclose(file);
    free(inputLine);
//    Free_Memory(*n, *latin_square_in);
    return;
    /* END MODIFYING CODE HERE */
}

/* 
 * This function checks to see that exactly n symbols are used and that 
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square) {
    if(latin_square == NULL){
        printf("Verify_Alphabet - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    //Only 94 ascii values can be used
    int numValidChars = 94;

    //Need an array to track the frequency of the characters
    int *frequency;
    frequency = (int *)malloc(numValidChars * sizeof(int));

    for(int i = 0; i < numValidChars; i++){
        *(frequency + i) = 0;
    }


    //Search through the array for each of the possible ascii characters, return 0 if they are not in the range of 33 to 126
    for(int i = 0 ; i < n; i++){
        for(int j = 0; j < n; j++){
            int index = (int)(*(*(latin_square + i) + j)) - 33;

            if((*(*(latin_square + i) + j)) < 33 || (*(*(latin_square + i) + j)) > 126){
                free(frequency);
                return 0;
            }
            *(frequency + index) += 1;
        }
    }

    //Iterate over the frequency array and check whether the alphabet is valid
    for(int i = 0; i < numValidChars; i++){
        if(*(frequency + i) != 0 && *(frequency + i) != n){
            free(frequency);
            return 0;
        }
    }

    free(frequency);
    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have 
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 * 
 * Note: Test all rows first then test all columns.
 * 
 * Error messages have been included for you. Do not change the format out the 
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
 * 
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
 int Verify_Rows_and_Columns(const size_t n, char **latin_square){
    if (latin_square == NULL) {
        printf("Verify_Rows_and_Columns - latin_square is NULL\n"); 
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */

    //Initializes similar variables to verify alphabet function
    //Needs to track duplicates and have an int is valid that practically
    //acts as boolean to check whether our Latin Square is still valid
    int numValidChars = 94;
    int numDuplicates = 0;
    int isValidLS = 1;
    int *frequency;
    int index;
    frequency = (int *)malloc(numValidChars * sizeof(int));


    //Check the rows for duplicates
    for(int row = 0; row < n; row++){
        for(int i = 0; i < numValidChars; i++){
            *(frequency + i) = 0;
        }
        numDuplicates = 0;
        //Iterate over each column to see if the character is already present in a given column
        for(int column = 0; column < n; column++) {
            index = (int)(*(*(latin_square + row) + column)) - 33;
            if(*(frequency + index) != 0 && numDuplicates == 0){
                printf("Error in row %d\n", row);
                isValidLS = 0;
                numDuplicates = 1;
            }
            *(frequency + index) += 1;
        }
    }

    //Check the columns for duplicates
    for(int column = 0; column < n; column++){
        for(int i = 0; i < numValidChars; i++){
            *(frequency + i) = 0;
        }
        numDuplicates = 0;
        //Iterate over each row to see if the character is already present in a given column
        for(int row = 0; row < n; row++){
            index = (int)(*(*(latin_square + row) + column)) - 33;
            if(*(frequency + index) != 0 && numDuplicates == 0){
                printf("Error in column %d\n", column);
                isValidLS = 0;
                numDuplicates = 1;
            }
            *(frequency + index) += 1;
//            if(index > 93 || index < 0){
//                printf("Row: %d \t Column: %d \t Index: %d \n", row, column, index);
//            }
        }
    }
    free(frequency);
    /* END MODIFYING CODE HERE */
    return isValidLS;
}

/* 
 * This function calls free to allow all memory used by the latin_square 
 * verification program to be reclaimed.
 *
 * Note: you will have n+1 calls to free
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */ 
void Free_Memory(const size_t n, char **latin_square) {
    /* BEGIN MODIFYING CODE HERE */
    for (int i = 0; i < n; i++) {
        free(*(latin_square + i));
    }

    free(latin_square);
    return;
    /* END MODIFYING CODE HERE */
}


