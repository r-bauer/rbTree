/*
  Arquivo: BSTAVL.C
  Autor: Roberto Bauer
  Observacoes:
              Rotinas para arvore busca binaria AVL
*/

#define __BSTAVL_C__

#include "main.h"


//  Redefinicoes para aumentar legibilidade do codigo
#define BSTRoot             (BST->slkDummyRoot)
#define NodeCount           (BST->uiCount)  // nohs na arvoreh atual

#define DuplicatedNode      (*(BST->fDuplicatedNode))
#define NodeDataCmp         (*(BST->fNodeDataCmp))




/*
    Funcao: 
        Higher

    Proposito:
        Retorna o maior numero

    Parametros:
        int a, int b -> numeros a serem comparados

    Retorno:
        maior numero de dois

    Descricao:
        Retorna o maior numero apos comparacaoh.
*/
PRIVATE int
Higher (int a, int b)
{
  return (a > b) ? a : b;
}

/*
    Funcao: 
        Height

    Proposito:
        Retorna a altura de um noh dentro da arvore

    Parametros:
        SLINK slkNode -> noh da arvore

    Retorno:
        Fator de balancoh do noh slkNode.

    Descricao:
        Retorna o dado de altura armazenado no noh.         
*/
PRIVATE int
Height (SLINK slkNode)
{
  if (slkNode)
    return slkNode->iHeight;
  else                          //      if (slkNode == NULL)
    return 0;
}

/*
    Funcao: 
        Balance

    Proposito:
        Retorna o fator de balancoh de um noh

    Parametros:
        SLINK slkNode -> noh da arvore que tera o fator de balanco calculado

    Retorno:
        Fator de balancoh do noh slkNode.

    Descricao:
        Calcula o fator de balancoh no noh, atraves da subtracaoh da
        altura da subarvore esquerda com a altura da subarvore direita.         
*/
PRIVATE int
Balance (SLINK slkNode)
{
  if (slkNode)
    return Height (slkNode->sLink[LEFT]) - Height (slkNode->sLink[RIGHT]);
  else                          //      if (slkNode == NULL)
    return 0;
}

/*
    Funcao: 
        Rotate

    Proposito:
        Realiza uma rotacaoh entre os nosh da arvore e subarvore

    Parametros:
        SLINK slkNode -> noh da arvore daonde serah efetuada a rotacaoh
        int iDir      -> sentido da rotacaoh.

    Retorno:
        SLINK slkChild -> o novo noh pai resultante da rotacaoh.

    Descricao:
        T1, T2 e T3 saoh subarvores enraizadas em y (lado esquerda) ou em x (no lado direito)

                          y                            x
                         / \     Rotacaoh Direita    /  \
                        x   T3   --------------->   T1   y 
                       / \      <---------------        / \
                      T1  T2    Rotacaoh Esquerda      T2  T3

        Indices em ambas as arvores obedecem a ordem:
          indice(T1) < indice(x) < indice(T2) < indice(y) < indice(T3)
        De tal forma que a propriedade de busca binaria naoh eh violado em nenhum dos casos.
        Como as duas rotacaoesh se espelham, eh possivel definir a logica da troca de nohs
        de uma rotacaoh esquerda e para mudar para direita, inverter os valores de direcaoh.
*/
PRIVATE SLINK
Rotate (SLINK slkNode, int iDir)
{
  SLINK slkChild;
  SLINK slkGrandChild;

  slkChild = slkNode->sLink[iDir];
  slkGrandChild = slkChild->sLink[!iDir];

  // Rotacaoh esquerda
  //
  //        node(x)                           child(y)  
  //      /        \                         /        \
  //    T1          child(y)          node(x)           T3
  //               /        \  ==>  /       \
  // grandchild(T2)          T3   T1         grandchild(T2)
  //
  // Rotacaoh direita
  //
  //               node(y)                child(x)  
  //              /       \              /        \
  //      child(x)         (T3)        T1          node(y)
  //     /        \              ==>             /        \
  //  T1           grandchild(T2)   grandchild(T2)         T3
  //
  slkChild->sLink[!iDir] = slkNode;
  slkNode->sLink[iDir] = slkGrandChild;

  slkNode->iHeight = Higher (Height (slkNode->sLink[RIGHT]),
                             Height (slkNode->sLink[LEFT])) + 1;
  slkChild->iHeight = Higher (Height (slkChild->sLink[RIGHT]),
                              Height (slkChild->sLink[LEFT])) + 1;

  return slkChild;
}

/*
// T1, T2, T3 and T4 are subtrees.
// Rotate Left
//
//        x                       y  
//      /   \                  /     \
//    T1      y               x       T3
//          /   \    ==>    /   \
//        T2     T3       T1     T2
//
#define RotateLeft(x)           Rotate((x), RIGHT)

// Rotate Right
//
//           y                    x  
//        /     \              /     \
//       x        T3        T1         y
//     /   \           ==>           /   \
//   T1     T2                     T2     T3
//
#define RotateRight(x)      Rotate((x), LEFT)
*/


/*
    Funcao: 
        InsNodeAvl

    Proposito:
        Adicionar um novo noh em uma arvore AVL balanceada

    Parametros:
        SBINTREE *BST    -> ponteiro para estrutura de dados que trata a arvore c/ ponteiros
                        paras funcoes de comparacaoh e tratamento de duplicados
        SLINK slkNode   -> posicaoh atual da arvore que serah percorrida para adicaoh de slkNew
        SLINK slkNew    -> noh a ser inserido na arvoreh

    Retorno:
        SLINK slkNode  -> em caso de balanceameto a rotina pode alterar o enderecoh original 
                         de slkNode

    Descricao:
            A partir da raiz, desce a ramificacaoh obdecendo a regra de busca binaria, para encontrar a 
        posicaoh em que irah ser inserido, sendo um novo elemento irah se posicionar no fim da ramificacaoh 
        como uma nova folha, que irah ser a situacaoh padraoh, uma vez que naoh aceita incluir duplicado
        nesta implementacaoh.
            Ao terminar de incluir o novo noh, a rotina retorna recursivamente o caminho percorrido ateh 
        a raiz novamente, neste momento eh possivel analisar o balanceamento de cada noh visitado durante 
        a travessia.
            Ao verificar o balanceamento da arvore, se utiliza a solucaoh AVL para corrigir os valores 
        cuja diferencah seja maior do que 1. Realiza uma rotacaoh simples por 'fora' nos casos DD ou EE, 
        ou uma rotacaoh dupla por 'dentro' nos casos DE ou ED.

*/
PRIVATE SLINK
InsNodeAvl (SBINTREE * BST, SLINK slkNode, SLINK slkNew)
{
  // chegou no fim da ramificacaoh
  // insere o dado na arvore
  if (slkNode == NULL)
    {
      // indica o fator de balanco
      // acabou de ser inserido, naoh tem filhos, logo = 1 de altura
      slkNew->iHeight = 1;

      // soma o novo noh no total
      NodeCount++;

      // retorna o enderecoh do noh para o link
      return slkNew;
    }
  else
    {
      int iCmp;
      int iDir;
      int iBalance = 0;

      iCmp = NodeDataCmp (slkNew->pData, slkNode->pData);

      // comparacaoh retorna -1(menor), 0(igual) e 1(maior)
      // corrige <= (-1 e 0) para 1(LEFT) e >(1) para 0(RIGHT)
      iDir = iCmp < 0;

      if (iCmp == 0)            // noh duplicado
        {
          // o que fazer com noh duplicado
          DuplicatedNode (slkNew, slkNode);

          //     0 -> destruir duplicada
          // naoh inclui o duplicado
          // apagamos os dados que dependem da implementacaoh do objeto
          DeleteData (slkNew->pData);
          // desalocamos o noh 
          free (slkNew);
        }
      else                      // noh maior ou menor
        {
          // avanca a posicaoh 
          slkNode->sLink[iDir] = InsNodeAvl (BST,
                                             slkNode->sLink[iDir], slkNew);
        }

      // atualiza altura
      slkNode->iHeight = Higher (Height (slkNode->sLink[RIGHT]),
                                 Height (slkNode->sLink[LEFT])) + 1;

      // calcula o balancoh atual no noh
      iBalance = Balance (slkNode);
/*
      // esquerda pesada
      if (iBalance > 1)
      {
          int iCmpL;

          iCmpL = NodeDataCmp(slkNew->pData, slkNode->sLink[LEFT]->pData);
          // Rotate Left Left
          //
          //         z                                      y 
          //        / \                                   /   \
          //       y   T4      RightRotate(z)            x      z
          //      / \          ------------->          /  \    /  \
          //     x   T3                               T1  T2  T3  T4
          //    / \
          //  T1   T2
          // Left Left Case
          if (iCmpL < 0)
          {
              slkNode = RotateRight(slkNode);
          }

          // Rotate Left Right
          //
          //      z                               z                            x
          //     / \                            /   \                         /  \
          //    y   T4  LeftRotate(y)          x    T4  RightRotate(z)      y      z
          //   / \      ------------>         /  \      ------------->     / \    / \
          // T1   x                          y    T3                     T1  T2 T3  T4
          //     / \                        / \
          //   T2   T3                    T1   T2
          // Left Right Case
          if (iCmpL > 0)
          {
              slkNode->sLink[LEFT] =  RotateLeft(slkNode->sLink[LEFT]);
              slkNode = RotateRight(slkNode);
          }
      }

      // direita pesada
      if (iBalance < -1)
      {
          int iCmpR;

          iCmpR = NodeDataCmp(slkNew->pData, slkNode->sLink[RIGHT]->pData);

          // Rotate Right Right
          // 
          //    z                            y
          //  /  \                         /   \
          // T1   y     LeftRotate(z)     z      x
          //     /  \   ------------>    / \    / \
          //    T2   x                  T1  T2 T3  T4
          //        / \
          //      T3  T4
          // Right Right Case
          if (iCmpR > 0)
          {
              slkNode = RotateLeft(slkNode);
          }
 
          //Rotate Right Left
          //
          //    z                            z                            x
          //   / \                          / \                          /  \
          // T1   y   RightRotate(y)      T1   x      LeftRotate(z)    z      x
          //     / \  ------------->         /  \     ------------>   / \    / \
          //    x   T4                      T2   y                  T1  T2  T3  T4
          //   / \                              /  \
          // T2   T3                           T3   T4
          // Right Left Case
          if (iCmpR < 0)
          {
              slkNode->sLink[RIGHT] = RotateRight(slkNode->sLink[RIGHT]);
              slkNode = RotateLeft(slkNode);
          }
      }
*/

      if (iBalance < -1 || iBalance > 1)
        {
          BOOL bDouble = FALSE;

          iDir = (iBalance > 1);

          iCmp = NodeDataCmp (slkNew->pData, slkNode->sLink[iDir]->pData);

          bDouble = iDir ? (iCmp > 0) : (iCmp < 0);

          if (bDouble)
            {
              slkNode->sLink[iDir] = Rotate (slkNode->sLink[iDir], !iDir);
            }

          slkNode = Rotate (slkNode, iDir);
        }
    }

  // retorna a si mesmo
  return slkNode;
}


/*
    Funcao: 
        AddNodeAvl

    Proposito:
        Adicionar um bloco de dados em uma arvore AVL

    Parametros:
        SBINTREE *BST   -> ponteiro para estrutura de dados que trata a arvore
                     c/ ponteiros paras funcoes de comparacaoh e tratamento de duplicados
        void *pData -> noh a ser inserido na arvoreh

    Retorno:
        nenhum

    Descricao:
        Ao receber o bloco de informacaoh apontado por pData, eh criado um noh
        que sera adicionado na arvore, para isso acontecer, estah funcaoh eh 
        ponto de entrada para chamada recursiva InsNodeAvl, que irah percorrer a 
        arvore a partir da raiz em direcaoh a posicaoh de insercaoh.
        Junto da inicializacaoh do noh a ser adicionado.

*/
void
AddNodeAvl (SBINTREE * BST, void *pData)
{
  SLINK slkNew;

  slkNew = CreateNode (BST, pData);     // cria o noh e passa o endereco 
  if (slkNew)                   // conseguiu alocar o noh
    {
      BSTRoot->sLink[RIGHT] = InsNodeAvl (BST, BSTRoot->sLink[RIGHT], slkNew);
    }
}

/*
    Funcao:
        RemoveNodeAvl

    Proposito:
        Remove um noh de uma arvore AVL balanceada

    Parametros:
        SBINTREE *BST    -> ponteiro para estrutura de dados que trata a arvore c/ ponteiros
                           paras funcoes de comparacaoh e tratamento de duplicados
        SLINK slkNode   -> posicaoh atual da arvore que serah percorrida para exclusaoh
        SLINK slkFind   -> informacaoh de busca a ser excluida da arvore.

    Retorno:
        SLINK slkNode  -> em caso de balanceameto a rotina pode alterar o enderecoh original de slkNode

    Descricao:
        Realiza uma busca recursiva pelo noh a ser excluido, slkFind possue uma copia do dado que estah sendo
        feita a busca, ao encontrar o noh que deve ser excluido, tresh situacoes podem ocorrer:
            1 - o noh eh uma folha (naoh tem filhos), 
                    basta excluir
            2 - possui apenas um filho(subarvore), 
                    filho toma lugar do pai na hierarquia da arvore, e exclui antigo pai
            3 - tem os dois filhos,
                    procura o menor descendente direito,
                    troca de posicaoh com o descendente,
                    reinicia busca recursiva que irah encontrar o noh na situacaoh 1 ou 2.
            Existe um fato importante em excluir o noh que se encontra no fim de uma ramificacaoh, obriga a rotina
        a percorrer a arvore ateh o extremo, ao retornar recursivamente, podera analisar o balanceamento de 
        cada noh percorrido.
            Durante o retorno do processo de exclusaoh, verifica-se o balanceamento da arvore e utiliza a solucaoh 
        AVL para corrigir os valores cuja diferencah seja maior do que 1. Realiza uma rotacaoh simples por 'fora' nos
        casos DD ou EE, ou uma rotacaoh dupla por 'dentro' nos casos DE ou ED.
*/
PRIVATE SLINK
RemoveNodeAvl (SBINTREE * BST, SLINK slkNode, SLINK slkFind)
{
  // chegou no fim da ramificacaoh
  // naoh encontrou o noh a ser excluido
  if (slkNode == NULL)
    {
      // libera o noh de busca
      DeleteData (slkFind->pData);
      free (slkFind);
    }
  else
    {
      int iCmp;
      int iDir;
      int iBalance = 0;

      iCmp = NodeDataCmp (slkFind->pData, slkNode->pData);

      // comparacaoh retorna -1(menor), 0(igual) e 1(maior)
      // corrige <= (-1 e 0) para 1(LEFT) e >(1) para 0(RIGHT)
      iDir = iCmp < 0;

      // encontramos o noh a ser excluido
      if (iCmp == 0)            // noh duplicado
        {

          if (slkNode->sLink[RIGHT] == NULL || slkNode->sLink[LEFT] == NULL)
            {
              SLINK slkDel;

              // salva a posicaoh do noh a ser excluido
              slkDel = slkNode;

              if (slkNode->sLink[RIGHT] == NULL)
                {
                  slkNode = slkNode->sLink[LEFT];
                }
              else
                {
                  slkNode = slkNode->sLink[RIGHT];
                }

              // desalocamos o noh de busca
              DeleteData (slkFind->pData);
              free (slkFind);

              DeleteData (slkDel->pData);       // apaga o dado do noh
              free (slkDel);    // libera a memoria do noh

              NodeCount--;      // subtrai o noh do total
            }
          // ultimo caso
          // encontre o menor descendente esquerdo do filho direito
          // substitui pelo menor descendente 
          else
            {
              SLINK slkMinor;
              void *pTmp;

              slkMinor = slkNode->sLink[RIGHT];
              while (slkMinor->sLink[LEFT])
                {
                  slkMinor = slkMinor->sLink[LEFT];
                }

              // neste caso eh mais simples trocar os dados entre os nohs
              pTmp = slkNode->pData;
              slkNode->pData = slkMinor->pData;
              slkMinor->pData = pTmp;


              // manda procurar na ramificacao com que se fez a troca
              slkNode->sLink[RIGHT] =
                RemoveNodeAvl (BST, slkNode->sLink[RIGHT], slkFind);
            }
        }
      else                      // noh maior ou menor
        {
          // avanca a posicaoh 
          slkNode->sLink[iDir] =
            RemoveNodeAvl (BST, slkNode->sLink[iDir], slkFind);
        }

      if (slkNode)
        {
          // atualiza altura
          slkNode->iHeight =
            Higher (Height (slkNode->sLink[RIGHT]),
                    Height (slkNode->sLink[LEFT])) + 1;

          // calcula o balancoh atual no noh
          iBalance = Balance (slkNode);

        /*
        // esquerda pesada
        if (iBalance > 1)
        {
            // Rotate Left Left
            //
            //         z                                      y 
            //        / \                                   /   \
            //       y   T4      RightRotate(z)            x      z
            //      / \          ------------->          /  \    /  \
            //     x   T3                               T1  T2  T3  T4
            //    / \
            //  T1   T2
            // Left Left Case
            if (Balance(slkNode->sLink[LEFT]) >= 0)
            {
                slkNode = RotateRight(slkNode);
            }

            // Rotate Left Right
            //
            //      z                               z                            x
            //     / \                            /   \                         /  \
            //    y   T4  LeftRotate(y)          x    T4  RightRotate(z)      y      z
            //   / \      ------------>         /  \      ------------->     / \    / \
            // T1   x                          y    T3                     T1  T2 T3  T4
            //     / \                        / \
            //   T2   T3                    T1   T2
            // Left Right Case
            if (Balance(slkNode->sLink[LEFT]) < 0) 
            {
                slkNode->sLink[LEFT] = RotateLeft(slkNode->sLink[LEFT]);
                slkNode = RotateRight(slkNode);
            }
         }

        // direta pesada
        if (iBalance < -1)
        {
            // Rotate Right Right
            // 
            //    z                            y
            //  /  \                         /   \
            // T1   y     LeftRotate(z)     z      x
            //     /  \   ------------>    / \    / \
            //    T2   x                  T1  T2 T3  T4
            //        / \
            //      T3  T4
            // Right Right Case
            if (Balance(slkNode->sLink[RIGHT]) <= 0)
            {
                slkNode = RotateLeft(slkNode);
            }

            //Rotate Right Left
            //
            //    z                            z                            x
            //   / \                          / \                          /  \
            // T1   y   RightRotate(y)      T1   x      LeftRotate(z)    z      x
            //     / \  ------------->         /  \     ------------>   / \    / \
            //    x   T4                      T2   y                  T1  T2  T3  T4
            //   / \                              /  \
            // T2   T3                           T3   T4
            // Right Left Case
            if (Balance(slkNode->sLink[RIGHT]) > 0)
            {
                slkNode->sLink[RIGHT] = RotateRight(slkNode->sLink[RIGHT]);
                slkNode = RotateLeft(slkNode);
            }
        }
        */

          if (iBalance < -1 || iBalance > 1)
            {
              BOOL bDouble;

              iDir = (iBalance > 1);

              iBalance = Balance (slkNode->sLink[iDir]);

              bDouble = iDir ? (iBalance < 0) : (iBalance > 0);

              if (bDouble)
                slkNode->sLink[iDir] = Rotate (slkNode->sLink[iDir], !iDir);

              slkNode = Rotate (slkNode, iDir);
            }
        }
    }

  // retorna a si mesmo
  return slkNode;

}


/*
    Funcao:
        DelNodeAvl

    Proposito:
        Remover um bloco de dados em uma arvore AVL

    Parametros:
        SBINTREE *BST -> ponteiro para estrutura de dados que trata a arvore
                        c/ ponteiros paras funcoes de comparacaoh e tratamento de duplicados
        void *pData -> informacaoh a ser excluida da arvoreh

    Retorno:
        nenhum

    Descricao:
        Ao receber o bloco de informacaoh apontado por pData, eh criado um noh
        que sera utlizado para procurar o dado a ser removido na arvore, para 
        isso acontecer, estah funcaoh eh ponto de entrada para chamada recursiva 
        RemoveNodeAvl que irah percorrer a arvore.
*/
void
DelNodeAvl (SBINTREE * BST, void *pData)
{
  SLINK slkFind;

  slkFind = CreateNode (BST, pData);    // cria o noh e passa o endereco
  if (slkFind)                  // conseguiu alocar o noh
    {
      BSTRoot->sLink[RIGHT] =
        RemoveNodeAvl (BST, BSTRoot->sLink[RIGHT], slkFind);
    }
}
