/*
Project 4 -Part 2 for CS3003
Author: Zimo Chai (Jerry)

Note:
============================
These code is intended to run under C.

A handy way to do coding and debugging is using C-mode in Emacs.
Key bindings may vary, but by default use:
============================
M-x compile
gcc -o p2 p2.c

Debug using gdb:
============================
M-x gud-gdb
b main
run
n
============================

2. A human chromosome is represented as a long string. Write a C program for counting the occurrences of all words of length 10 in the human chromosome 1

(http://hgdownload.soe.ucsc.edu/goldenPath/hg38/chromosomes/chr1.fa.gz - - you will need to decompress this file). A word in this context is a substring starting at any nucleotide and has a length of 10. Each nucleotide represents the beginning of a word. Yes, these words are overlapping, and are NOT separated by spaces.

The chromosome file must be provided to your program as a command line argument. Sequences of human chromosomes may contain additional letters when the identity of the nucleotide cannot be determined precisely. Words consisting of A, C, G, and T only must be counted; all other words must not.

The counting is NOT case-sensitive.

The output of your program is as follows:

word 1    location 1, location 2, .. , location n

word 2    location 1, location 2, .. , location n

Note: word 1, word 2, and word n are unique.

Desgin:
============================
Since the data we are going to process is huge, we need to think about the tradeoff between computation resource and the RAM we have.
In this implementation, we go through the entire file twice:
For the first time, we need to count the occurence times of each word, which helps us to allocate only RAM that we will use;
For the second time, we store the location to the porper place of the array.
Therefore, although we process the entire file twice, which cause a little bit more computation resource, it saves us tons of RAM comparing to allocating a SIZE*SIZE 2d array.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
  Notice that changing WORDSIZE here is not enough when wordsize really changed.
  This is becuase, still, 0xFFFFF is hardcoded.
*/
#define WORDSIZE 10
/*
  This number is caculated by (NUMBER_OF_VALID_CHARACTER)^WORDSIZE = 4 ^ 10
*/
#define SIZE 1048576

int a[SIZE];                    /* intitialize an array with 4^10 0 */
int *a_2d[SIZE];

int main(int argc, char **argv){
  char c;
  FILE *f;
  unsigned int index;
  unsigned int value;
  int length = 0;   /* length is current length of a complete/incomplete word */
  int location = -WORDSIZE + 1;    /* therefore the location of the first word will be 1 */

  if(argc == 2){
    f = fopen(argv[1], "r"); /* "r" for read */
  }
  else{
    fprintf(stderr, "Expected ONLY 1 command line arugemnt!\n");
    return 1;
  }

  while((c = getc(f)) != EOF ){
    bool isValidValue = true;
    switch(c){
    case 'A':
    case 'a':
      value = 0x0;
      break;
    case 'T':
    case 't':
      value = 0x1;
      break;
    case 'C':
    case 'c':
      value = 0x2;
      break;
    case 'G':
    case 'g':
      value = 0x3;
      break;
    case '\n':
      continue;
    default:
      isValidValue = false;
      length = 0;       /* important to get consecutive 10 characters by reseting length to 0 */
    }
    if(isValidValue){
      ++length;
      /* shift left the current index by 2 bits*/
      index = index << 2;
      /* "add" value to the index by bitwise OR */
      index = index | value;
      /* only record the valid words (10 consecutive characters) */
      if(length >= WORDSIZE){
        ++a[index & 0xFFFFF];
      }
    }
  }    // end of while loop


  /*
    Create the 2d array based on the statistic data of occurence of each pattern.
    We intentionally allocate one more cell (a[i] +1) for the array because we will use
    a_2d[index][0] as a counter for how many elements have been filled into the arrat.
   */
  for (int i=0; i<SIZE; i++){
    a_2d[i] = (int *)malloc( (a[i]+1) * sizeof(int));
  }

  /* now read the file again
     this time, we will remember the location of the file
   */
  fseek(f, 0, SEEK_SET);     /* reset the pointer to the beginning of the file */
  length = 0;                /* reset the length to 0 since this will a new loop of counting */
  while((c = getc(f)) != EOF ){
    ++location;
    bool isValidValue = true;
    switch(c){
    case 'A':
    case 'a':
      value = 0x0;
      break;
    case 'T':
    case 't':
      value = 0x1;
      break;
    case 'C':
    case 'c':
      value = 0x2;
      break;
    case 'G':
    case 'g':
      value = 0x3;
      break;
    case '\n':
      --location;
      continue;
    default:
      isValidValue = false;
      length = 0;       /* important to get 10 consecutive characters by reseting length to 0 */
    }
    if(isValidValue){
      ++length;
      /* shift left the current index by 2 bits*/
      index = index << 2;
      /* "add" value to the index by bitwise OR */
      index = index | value;
      /* Use the lower 20 bits as a hash */
      /* We store the current location to the proper place in the 2nd array.
         Note that we also increase the a_2d[index & 0xFFFFF][0] which indicates
         the number of elements in the 2nd array by 1.
       */
      if(length >= WORDSIZE){
        a_2d[index & 0xFFFFF][++a_2d[index & 0xFFFFF][0]] = location;
      }
    } /* end if statement */
  }   /* end while loop */

  // print out the result
  // k is the row index in this case
  for(int k = 0; k < SIZE; k++){
    int i = k;
    /* the for loop is for printing the word pattern */
    for(int j=0; j < WORDSIZE; j++){
      /* (i & 0xC0000) >> 18 */
      switch((i >>(WORDSIZE - 1)*2) & 0x3){ // get the value in of 20th and 19th bit as a 2 bit binary
      case 0x0:
        printf("%c", 'A');
        break;
      case 0x1:
        printf("%c", 'T');
        break;
      case 0x2:
        printf("%c", 'C');
        break;
      case 0x3:
        printf("%c", 'G');
        break;
      default:
        fprintf(stderr, "Error: Unexpected pasred value.\n");
        return 1;
      }  // end switch
      i = i << 2;
    }  // end inter for loop

    printf(":    ");     /* format */
    // Note that a_2d[row][col] is same as *(*(arr+row)+col)
    /* we start the index of the 2nd array from 1 because a_2d[k][0] hold the value of total number of elements in that array
       of course, we can print that out for debugging reason.
     */
    for (int col = 1; col < a_2d[k][0]+1; col++){
        printf(" %d,", a_2d[k][col]);
    }
    printf("\n");
  }  // end outer for loop

  return 0;
}
