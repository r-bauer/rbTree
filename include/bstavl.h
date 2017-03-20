/*
  Arquivo: BSTAVL.H
  Autor: Roberto Bauer
  Observacoes:
			Declaracaoh para rotinas da arvore AVL
*/

#ifndef __BSTAVL_H__
#define __BSTAVL_H__

#undef EXTERN
#ifdef __BSTAVL_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif



void AddNodeAvl(SBINTREE *, void *);
void DelNodeAvl(SBINTREE *, void *);




#endif  // #define __BSTAVL_H__

