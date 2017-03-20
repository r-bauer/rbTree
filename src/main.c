/*
  Arquivo: MAIN.C
  Autor: Roberto Bauer
  Observacoes: 
			Arquivo central do programa, funcaoh main()
*/

#define __MAIN_C__

#include "main.h"



int
main (int argc, char *argv[])
{
  SBINTREE *BT, *AVL, *RB;
  int iRet = EXIT_SUCCESS;

  // configura a estrutura de dados da arvore binaria
  BT = CreateBinTree (CreateData,
                      DeleteData, DuplicatedNode, NodeDataCmp, PrintData);
  if (BT == NULL)
    {
      fprintf (stderr, "Erro criando arvore binaria\n");
      iRet = EXIT_FAILURE;
    }
  else
    {
      AVL = CreateBinTree (CreateData,
                           DeleteData,
                           DuplicatedNode, NodeDataCmp, PrintDataAvl);

      if (AVL == NULL)
        {
          fprintf (stderr, "Erro criando arvore busca binaria AVL\n");
          iRet = EXIT_FAILURE;
        }
      else
        {
          RB = CreateBinTree (CreateData,
                              DeleteData,
                              DuplicatedNode, NodeDataCmp, PrintDataRb);

          if (RB == NULL)
            {
              fprintf (stderr,
                       "Erro criando arvore busca binaria RedBlack\n");
              iRet = EXIT_FAILURE;
            }
          else
            {
              // controla as opcoes para teste
              // na arvore criada
              // possui um loop interno que serah finalizado 
              // quando o usuario escolher a opcaoh
              Menu (BT, AVL, RB);

              // libera toda a memoria utilizada pela arvore binaria RedBlack
              DestroyBinTree (RB);
            }
          // libera toda a memoria utilizada pela arvore busca binaria AVL
          DestroyBinTree (AVL);
        }
      // libera toda a memoria utilizada pela arvore binaria
      DestroyBinTree (BT);
    }


  return (iRet);
}
