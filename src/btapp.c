/*
  Arquivo: BTAPP.C
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh p/ arvore binaria
*/

#define __BTAPP_C__

#include "main.h"


void *
CreateData (void *data)
{
  SNODEDATA *pNewData;

  // aloca sua estrutura de dados
  pNewData = (SNODEDATA *) malloc (sizeof (SNODEDATA));
  if (pNewData == NULL)
    return (NULL);

  // move os valores para a estrutura de dados
  pNewData->uCont = 1;
  pNewData->pWord = strdup ((char *) data);

  if (pNewData->pWord == NULL)  // erro copiando a string
    {
      free (pNewData);
      return (NULL);
    }
  else
    {
      return (pNewData);        // retorna o endereco da estrutura
    }
}

BOOL
DeleteData (void *data)
{
  // duas reservas de memoria foram criadas
  // uma para a estrutura SNODEDATA, 
  // outra para a string
  // neste caso SNODEDATA consiste em : um ponteiro e um inteiro.
  // O inteiro junto do ponteiro retorna para a memoria 
  // quando a estrutura for liberada
  // Sendo nescessario liberar a string antes do memoria do ponteiro
  free (((pND) data)->pWord);
  free (data);
  return TRUE;
}

// Estah funcaoh determina o que fazer quando se insere um noh na lista
// onde jah se encontra outro noh com a mesma informacaoh.
//
// Note que estah funcaoh deve retornar um dos seguintes valores
//      0   apagar o noh duplicado
//      1   inserir o noh duplicado
//
// Neste caso estamos contando as palavras, se uma palavra duplicada eh
// encontrada, incrementa-se o contador e naoh insere o dado duplicado
//
int
DuplicatedNode (SLINK slkNewNode, SLINK slkListNode)
{
  pND pnd;

  // transforma o ponteiro de dados do noh, no ponteiro da aplicacaoh
  pnd = slkListNode->pData;
  // adiciona-se uma ocorrencia para a palavra existente
  pnd->uCont += 1;

  return 0;
}

//

int
NodeDataCmp (void *first, void *second)
{
  return (strcmp (((pND) first)->pWord, ((pND) second)->pWord));
}

// serah usada na rotina de processamento de dados
// para o exemplo o dado processado ira ser impresso na tela
// em outros casos poderia, armazenar, filtrar ou tratar os
// valores para outros aplicativos.
int
PrintData (void *fOut, SLINK slkNode, int iLevel)
{
  return fprintf (fOut,
//                      "(%i) %s %u", 
                  "(%i) %s", iLevel, ((pND) slkNode->pData)->pWord      //,
//                      ((pND) slkNode->pData)->uCont
    );
}

int
PrintDataAvl (void *fOut, SLINK slkNode, int iLevel)
{
  return fprintf (fOut,
                  "(%i) %s [%i]",
                  iLevel, ((pND) slkNode->pData)->pWord, slkNode->iHeight);
}

int
PrintDataRb (void *fOut, SLINK slkNode, int iLevel)
{
  return fprintf (fOut,
                  "(%i) %s [%c]",
                  iLevel,
                  ((pND) slkNode->pData)->pWord,
                  (slkNode->bIsRed ? 'r' : 'b'));
}
