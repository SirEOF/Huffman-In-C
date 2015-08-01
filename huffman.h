
#ifndef _HUFFMAN
#define _HUFFMAN

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <conio.h>


typedef unsigned char BYTE;

// Element for frequency table
typedef  struct {
  BYTE value;
  int counter;
  char* code;
} Symbol, *pSymbol;

#endif