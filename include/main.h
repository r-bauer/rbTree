/*
  Arquivo: MAIN.H
  Autor: Roberto Bauer
  Observacoes: Cabecalho principal p/ concentrar os demais cabecalhos
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __MAIN_C__
    #define EXTERN
#else
    #define EXTERN      extern
#endif

#ifdef WIN_COMPILER

#endif

#define AVL_TREE

#define PRIVATE       static

typedef enum tagBOOL { FALSE, TRUE } BOOL;

typedef unsigned char		BYTE;
typedef unsigned short int	WORD;
typedef unsigned long int	DWORD;



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "btgen.h"
#include "btapp.h"
#include "btdriver.h"
#include "bstavl.h"
#include "bstrb.h"


#endif                                  /* #ifndef __MAIN_H__ */
