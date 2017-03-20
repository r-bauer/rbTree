/*
  Arquivo: BTAPP.H
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh p/ arvore binaria
*/


#ifndef __BTAPP_H__
#define __BTAPP_H__

#undef EXTERN
#ifdef __BTAPP_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif


// noh consiste em:
typedef struct tagSNODEDATA
{
    char *pWord;		// ponteiros para palavras
    unsigned int uCont;	// e a contagem de ocorrencias
}SNODEDATA;

typedef SNODEDATA *      pND;

EXTERN void *CreateData( void * );
EXTERN BOOL  DeleteData( void * );
EXTERN int   DuplicatedNode( SLINK, SLINK );
EXTERN int   NodeDataCmp( void *, void * );
EXTERN int   PrintData(void *, SLINK, int);
EXTERN int PrintDataAvl(void *, SLINK, int);
EXTERN int PrintDataRb(void *, SLINK, int);



#endif  // #define __BTAPP_H__
