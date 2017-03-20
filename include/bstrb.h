/*
  Arquivo: BSTRB.H
  Autor: Roberto Bauer
  Observacoes:
			Declaracaoh para rotinas da arvore de busca binaria rubro negra
*/

#ifndef __BSTRB_H__
#define __BSTRB_H__

#undef EXTERN
#ifdef __BSTRB_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif



void AddNodeRb(SBINTREE *, void *);
void DelNodeRb(SBINTREE *, void *);



#endif  // #define __BSTRB_H__