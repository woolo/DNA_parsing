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

#define SIZE 1048575

int a[SIZE];                    /* intitialize an array with 2^20 0 */

int main(int argc, char **argv){
  char c;
  FILE *f;

  if(argc == 2){
    f = fopen(argv[1], "r"); /* "r" for read */
  }
  else{
    printf("Expected ONLY 1 command line arugemnt!\n");
    return 1;
  }

  unsigned int index;
  unsigned int value;
  unsigned low20bits = index & 0xFFFFF; /* use only the lower 20 bits */

// load the first 20 valid bits
    for(int i = 0; i < 20; i++){
      c = getc(f);
      bool isValidValue = true;  // true
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
      default:
        isValidValue = false;
        i = 0;               /* important to get consecutive 10 bits here */
      }
      if(isValidValue){
        /* "add" value to the index by bitwise OR */
        index = index | value;
        /* shift left the current index by 2 bits*/
        index = index << 2;
      }
    }

    ++a[index & 0xFFFFF];           /* Use the lower 20 bits as a hash */

    int location = 20;
    // now read the rest of the file
    while ((c = getc(f)) != EOF){
      bool isValidValue = true;  // true
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
      default:
        isValidValue = false;
      }
      if(isValidValue){
        /* "add" value to the index by bitwise OR */
        index = index | value;
        ++a[index & 0xFFFFF];  /* Use the lower 20 bits as a hash */
        /* shift left the current index by 2 bits*/
        index = index << 2;
      }
}

for(int k = 0; k < SIZE; k++){
  int i = k;
  for(int j=0; j < 10; j++){
    switch( (i & 0xC0000) >> 18){  // get the value in of 20th and 19th bit
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
      printf("Error: We should never see this\n");
    }
    i = i << 2;
  }
  printf(" %d\n", a[k]);
}
  return 0;
}
