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

/* renew_word gurantee to load 20 consecutive valid bits */
/* int renew_word(FILE *f, int *location){ */
/*   unsigned int index; */
/*   unsigned int value; */
/*   char c; */

/*   for(int i = 0; i < 20; i++){ */
/*     ++*location; */
/*     c = getc(f); */
/*     bool isValidValue = true;  // true */
/*     switch(c){ */
/*     case 'A': */
/*     case 'a': */
/*       value = 0x0; */
/*       break; */
/*     case 'T': */
/*     case 't': */
/*       value = 0x1; */
/*       break; */
/*     case 'C': */
/*     case 'c': */
/*       value = 0x2; */
/*       break; */
/*     case 'G': */
/*     case 'g': */
/*       value = 0x3; */
/*       break; */
/*     default: */
/*       isValidValue = false; */
/*       i = 0;               /\* important to get consecutive 10 bits here *\/ */
/*     } */
/*     if(isValidValue){ */
/*       /\* "add" value to the index by bitwise OR *\/ */
/*       index = index | value; */
/*       /\* shift left the current index by 2 bits*\/ */
/*       index = index << 2; */
/*     } */
/*   } */
/*   return index; */

/* } */

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

  /* // load the first 20 valid bits */
  /* index = renew_word(f, &location); */

  /* ++a[index & 0xFFFFF];           /\* Use the lower 20 bits as a hash *\/ */


  // now read the file
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
      /* Use the lower 20 bits as a hash */
      ++location;
      ++a[index & 0xFFFFF];
      /* shift left the current index by 2 bits*/
      index = index << 2;
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
        default:
          isValidValue = false;
          length = 0;               /* important to get consecutive 10 characters here by set length to 0 */
        }
        if(isValidValue){
          ++length;
          /* "add" value to the index by bitwise OR */
          index = index | value;
          /* shift left the current index by 2 bits*/
          index = index << 2;
        }
      }    // end of for loop
      /* after the for loop we got the valid address */
      ++a[index & 0xFFFFF];
      /* shift left the current index by 2 bits*/
      index = index << 2;
    }  // end of else
  }  // end of while loop

  int max = 0;
  int above400 = 0;
  int above500 = 0;
  int above600 = 0;
  int above700 = 0;
  int above800 = 0;
  int above900 = 0;


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
    max = a[k] > max ? a[k] : max;
    above400 += a[k] > 400 ? 1 : 0;
    above500 += a[k] > 500 ? 1 : 0;
    above600 += a[k] > 600 ? 1 : 0;
    above700 += a[k] > 700 ? 1 : 0;
    above800 += a[k] > 800 ? 1 : 0;
    above900 += a[k] > 900 ? 1 : 0;
  }
  printf("Max number of apperance is: %d\n", max);
  printf("Apperance number above 400 is: %d\n", above400);
  printf("Apperance number above 500 is: %d\n", above500);
  printf("Apperance number above 600 is: %d\n", above600);
  printf("Apperance number above 700 is: %d\n", above700);
  printf("Apperance number above 800 is: %d\n", above800);
  printf("Apperance number above 900 is: %d\n", above900);

  return 0;
}
