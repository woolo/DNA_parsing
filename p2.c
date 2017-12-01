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
*/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE 1048576

int a[SIZE];                    /* intitialize an array with 2^20 0 */

int main(int argc, char **argv){
  char c;
  FILE *f;

  if(argc == 2){
    f = fopen(argv[1], "r"); /* "r" for read */
  }
  else{
    fprintf(stderr, "Expected ONLY 1 command line arugemnt!\n");
    return 1;
  }

  unsigned int index;
  unsigned int value;

  int location = 0;

  // now read the file
  while ((c = getc(f)) != EOF){
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
      isValidValue = false;
      break;
    default:
      isValidValue = false;
    }
    if(isValidValue){
      /* shift left the current index by 2 bits*/
      index = index << 2;
      /* "add" value to the index by bitwise OR */
      index = index | value;
      /* Use the lower 20 bits as a hash */
      ++a[index & 0xFFFFF];
      ++location;
    }
    else{
      /* use a for loop to get a valid word (10 consecutive characters) */
      int length = 0;
      while(((c = getc(f)) != EOF ) && length <= 10){
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
          isValidValue = false;
          --location;
          break;
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
        }
      }    // end of for loop
      /* after the for loop we got the valid address */
      ++a[index & 0xFFFFF];
    }  // end of else
  }  // end of while loop

  int max = 0;
  int max_of_index = 0;
  int above400 = 0;
  int above500 = 0;
  int above600 = 0;
  int above700 = 0;
  int above800 = 0;
  int above900 = 0;

  for(int k = 0; k < SIZE; k++){
    int i = k;
    /* the for loop is for printing the word pattern */
    for(int j=0; j < 10; j++){
      switch( (i & 0xC0000) >> 18){  // get the value in of 20th and 19th bit as a 2 bit binary
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
      }
      i = i << 2;
    }
    printf(" %d\n", a[k]);

    max_of_index = a[k] > max ? k : max_of_index;
    max = a[k] > max ? a[k] : max;
    above400 += a[k] > 400 ? 1 : 0;
    above500 += a[k] > 500 ? 1 : 0;
    above600 += a[k] > 600 ? 1 : 0;
    above700 += a[k] > 700 ? 1 : 0;
    above800 += a[k] > 800 ? 1 : 0;
    above900 += a[k] > 900 ? 1 : 0;
  }

  int i = max_of_index;
  for(int j=0; j < 10; j++){
      switch( (i  & 0xC0000) >> 18){  // get the value in of 20th and 19th bit as a 2 bit binary
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
      }
      i = i << 2;
    }
  printf(" has the most cocurrences: %d\n", max);
  printf("Apperance number above 400 is: %d\n", above400);
  printf("Apperance number above 500 is: %d\n", above500);
  printf("Apperance number above 600 is: %d\n", above600);
  printf("Apperance number above 700 is: %d\n", above700);
  printf("Apperance number above 800 is: %d\n", above800);
  printf("Apperance number above 900 is: %d\n", above900);
  printf("Final location value: %d\n", location);

  return 0;
}
