/*
  Arquivo: BTDRIVER.H
  Autor: Roberto Bauer
  Observacoes:

*/

#ifndef __BTDRIVER_H__
#define __BTDRIVER_H__

#undef EXTERN
#ifdef __BTDRIVER_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif

EXTERN void Menu( SBINTREE *, SBINTREE *, SBINTREE * );


#endif  // #define __BTDRIVER_H__
