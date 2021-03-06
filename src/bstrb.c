/*
  Arquivo: BSTRB.C
  Autor: Roberto Bauer
  Observacoes:
              Rotinas para arvore busca binaria rubro negra
*/

#define __BSTRB_C__

#include "main.h"




//  Redefinicoes para aumentar legibilidade do codigo
#define BSTRoot             (BST->slkDummyRoot)
#define NodeCount           (BST->uiCount)      // nohs na arvoreh atual
#define Balance             (BST->bBalance)

#define DuplicatedNode      (*(BST->fDuplicatedNode))
#define NodeDataCmp         (*(BST->fNodeDataCmp))


/*
    Funcao: 
        RedSons

    Proposito:
        Devolve o total de filhos vermelhos de um noh

    Parametros:
        SLINK slkNode -> verifica os filhos deste noh

    Retorno:
        int - total de filhos vermelhos

    Descricao:
        verifica a cor de cada filho e retorna quantos s�o vermelhos
        uma das definicoes para a arvore rubro-negra, exige que todas as folhas
        sejam pretas, eh considerado preto um noh vazio, portanto um noh valido 
        vermelho q naoh contenha filhos validos, aponta para dois nohs nulos e
        como tal, obedece a regra.
        OU SEJA, consideramos um noh negro se ele for nulo ou indicando cor
        diferente de vermelho
*/
int
RedSons (SLINK slkNode)
{
  return (slkNode == NULL) ? 0 :
            (slkNode->sLink[RIGHT] != NULL && slkNode->sLink[RIGHT]->bIsRed == TRUE) +
            (slkNode->sLink[LEFT ] != NULL && slkNode->sLink[LEFT ]->bIsRed == TRUE);
}

/*
    Funcao: 
        WhoIsRed

    Proposito:
        Suporte a RedSons, devolve qual dos dois filhos de um noh eh vermelho

    Parametros:
        SLINK slkNode -> verifica os filhos deste noh

    Retorno:
        int -   0: RIGHT
                1: LEFT

    Descricao:
        Precisa ser usada em conjunto com a funcaoh RedSons, somente em 
        casos que a RedSons retorna indicando apenas 1 filho.
*/
int
WhoIsRed (SLINK slkNode)
{
  return (slkNode->sLink[LEFT] != NULL && slkNode->sLink[LEFT]->bIsRed == TRUE);
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
        InsNodeRB

    Proposito:
        Adicionar um novo noh em uma arvore Rubro Negra balanceada

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
*/
PRIVATE SLINK
InsNodeRB (SBINTREE * BST, SLINK slkNode, SLINK slkNew)
{
  // chegou no fim da ramificacaoh
  // insere o dado na arvore
  if (slkNode == NULL)
    {
      // nohs recem inseridos, tem a cor vermelha
      slkNew->bIsRed = TRUE;

      // soma o novo noh no total
      NodeCount++;

      // retorna o enderecoh do noh para o link
      return slkNew;
    }
  else
    {
      int iNumRedSons;
      int iCmp;
      int iDir;

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
          slkNode->sLink[iDir] =
            InsNodeRB (BST, slkNode->sLink[iDir], slkNew);

          // filho vermelho
          if (slkNode->sLink[iDir]->bIsRed)
            {
              // se tiver um neto vermelho, 
              // implica em violar um requisito para arvoreh rubro negra
              // deve corrigir
              if (RedSons (slkNode->sLink[iDir]))
                {
                  // Tio vermelho,
                  // basta apenas fazer a troca de cores dos irm�o e pai para corrigir
                  if (slkNode->sLink[!iDir] != NULL
                      && slkNode->sLink[!iDir]->bIsRed)
                    {
                      // troca de cor
                      slkNode->sLink[LEFT]->bIsRed = FALSE;
                      slkNode->sLink[RIGHT]->bIsRed = FALSE;
                      slkNode->bIsRed = TRUE;
                    }
                  // tio preto
                  else
                    {
                      // tendo um tio negro, 
                      // precisa tratar os quatro casos que exigem rota��o
                      // faz dupla rotacaoh se for
                      // Left Right Case
                      // Right Left Case
                      if (WhoIsRed (slkNode->sLink[iDir]) == !iDir)
                        {
                          slkNode->sLink[iDir] =
                            Rotate (slkNode->sLink[iDir], !iDir);
                        }
                      // e somente rotacaoh simple quando
                      // Left Left Case
                      // Right Right Case
                      slkNode = Rotate (slkNode, iDir);
                      // troca de cores
                      slkNode->sLink[!iDir]->bIsRed = TRUE;
                      slkNode->bIsRed = FALSE;
                    }
/*
                      if (iDir == LEFT)
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
                          if (WhoIsRed(slkNode->sLink[LEFT]) == LEFT)
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
                          else //if (WhoIsRed(slkNode->sLink[LEFT]) == RIGHT)
                          {
                              slkNode->sLink[LEFT] = RotateLeft(slkNode->sLink[LEFT]);
                              slkNode = RotateRight(slkNode);
                          }

                          slkNode->sLink[RIGHT]->bIsRed = TRUE;
                          slkNode->bIsRed = FALSE;
                      }
                      else //if (iDir == RIGHT)
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
                          if (WhoIsRed(slkNode->sLink[RIGHT]) == RIGHT)
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
                          else // if (WhoIsRed(slkNode->sLink[RIGHT]) == LEFT)
                          {
                              slkNode->sLink[RIGHT] = RotateRight(slkNode->sLink[RIGHT]);
                              slkNode = RotateLeft(slkNode);
                          }
                          slkNode->sLink[LEFT]->bIsRed = TRUE;
                          slkNode->bIsRed = FALSE;
                      }
*/
                }
            }
        }
    }

  // retorna a si mesmo
  return slkNode;
}


/*
    Funcao: 
        AddNodeRb

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
        ponto de entrada para chamada recursiva InsNodeRB, que irah percorrer a 
        arvore a partir da raiz em direcaoh a posicaoh de insercaoh.
        Junto da inicializacaoh do noh a ser adicionado.

*/
void
AddNodeRb (SBINTREE * BST, void *pData)
{
  SLINK slkNew;

  slkNew = CreateNode (BST, pData);     // cria o noh e passa o endereco 
  if (slkNew)                   // conseguiu alocar o noh
    {
      BSTRoot->sLink[RIGHT] = InsNodeRB (BST, BSTRoot->sLink[RIGHT], slkNew);

      // em alguns caso o processo de balanceamento pode resultar com raiz c/ a cor vermelh
      BSTRoot->sLink[RIGHT]->bIsRed = FALSE;
    }
}




/*
    Funcao:
        RemoveNodeRB

    Proposito:
        Remove um noh de uma arvore RB balanceada

    Parametros:
        SBINTREE *BST    -> ponteiro para estrutura de dados que trata a arvore c/ ponteiros
                           paras funcoes de comparacaoh e tratamento de duplicados
        SLINK slkNode   -> posicaoh atual da arvore que serah percorrida para exclusaoh
        SLINK slkFind   -> informacaoh de busca a ser excluida da arvore.

    Retorno:
        SLINK slkNode  -> em caso de balanceamento a rotina pode alterar o enderecoh original de slkNode

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
*/
PRIVATE SLINK
RemoveNodeRB (SBINTREE * BST, SLINK slkNode, SLINK slkFind)
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

              // comparacaoh retorna 0 (LEFT == NULL) e 1 (RIGHT == NULL)
              iDir = (slkNode->sLink[RIGHT] == NULL);
              // carrega o vetor que naoh for nulo, 1(LEFT) e para 0(RIGHT)
              slkNode = slkNode->sLink[iDir];
              // Obs.: funciona mesmo quando ambos saoh nulos

              //   R E D  B L A C K
              // caso o noh excluido tenha um filho
              // atualiza a cor
              //  se o noh filho ou o noh excluido forem vermelho
              // naoh precisa fazer rebalanceamento

              // tem filho
              if ((slkNode) && (slkNode->bIsRed != slkDel->bIsRed))
                {
                  slkNode->bIsRed = FALSE;
                  Balance = FALSE;
                }
              else
                {
                  // foi excluido um noh preto
                  // o balanco da arvore foi alterado
                  Balance = TRUE;
                }
              //   R E D  B L A C K


              // desalocamos o noh de busca
              DeleteData (slkFind->pData);
              free (slkFind);

              DeleteData (slkDel->pData);       // apaga o dado do noh
              free (slkDel);    // libera a memoria do noh

              NodeCount--;      // subtrai o noh do total

              return slkNode;
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
              iDir = RIGHT;
              slkNode->sLink[iDir] =
                RemoveNodeRB (BST, slkNode->sLink[iDir], slkFind);
            }
        }
      else                      // noh maior ou menor
        {
          // avanca a posicaoh 
          slkNode->sLink[iDir] =
            RemoveNodeRB (BST, slkNode->sLink[iDir], slkFind);
        }


      //////////////////////////////////////////////////////////////////
      //////////               R E D  B L A C K             ////////////
      //////////////////////////////////////////////////////////////////

      // Esta voltando na arvore
      if (Balance)
        {
          Balance = FALSE;
          // slkNode eh pai do noh excluido

          // caso 1.1 e 1.2
          // pai do noh excluido eh vermelho
          if (slkNode->bIsRed == TRUE)
            {
              // caso 1.1 algum neto vermelho
              if (RedSons (slkNode->sLink[!iDir]))
                {
                  // zig-zag (RIGHT LEFT OU LEFT RIGHT)
                  if (WhoIsRed (slkNode->sLink[!iDir]) == iDir)
                    {
                      // dupla rotacaoh
                      // Rotate Left Right
                      //
                      //      z                               z                              y
                      //     red                             red   color change(z)          red
                      //     / \                            /   \                          /   \
                      //    x   nil LeftRotate(x)          y     nil  RightRotate(z)     x       z
                      //  black                           red                          black   black
                      //   / \      ------------>         /  \       ------------->     / \     / \
                      // T1   y                          x    T3                      T1  T2  T3   nil
                      //     red                       black
                      //     / \                        / \
                      //   T2   T3                    T1   T2
                      //
                      // Rotate Right Left
                      //
                      //     x                              x                                  y
                      //    red                            red    color change(x)             red
                      //   /   \                          /   \                             /     \
                      // nil    z    RightRotate(z)     nil    y      LeftRotate(x)        x       z
                      //      black                           red                       black     black
                      //     /     \  ------------->         /   \     ------------>   /     \   /     \
                      //    y      T4                      T2     z                 nil      T2 T3      T4
                      //   red                                  black
                      //  /   \                                /     \
                      // T2   T3                             T3       T4
                      slkNode->sLink[!iDir] =
                        Rotate (slkNode->sLink[!iDir], iDir);
                    }
                  // zig-zig (RIGHT RIGHT OU LEFT LEFT)
                  // simples rotacaoh
                  // Rotate Left Left
                  //
                  //             z                                        y
                  //            red      change color(x, y & z)          red
                  //           /   \                                   /     \
                  //          y    nil      RightRotate(z)            x       z
                  //        black                                 black       black
                  //       /     \          ------------->       /     \     /     \
                  //      x       T3                           T1       T2  T3     nil
                  //     red
                  //    /   \
                  //  T1     T2
                  //
                  // Rotate Right Right
                  //
                  //      z                                   y
                  //     red     change color(x, y & z)      red
                  //    /   \                              /     \
                  // nil     y      LeftRotate(z)       z           x
                  //       black                      black       black
                  //      /     \   ------------>    /     \     /     \
                  //    T2       x                 nil      T2 T3       T4
                  //            red
                  //           /   \
                  //         T3     T4
                  slkNode = Rotate (slkNode, !iDir);

                  // troca as cores
                  slkNode->bIsRed = TRUE;
                  slkNode->sLink[RIGHT]->bIsRed = FALSE;
                  slkNode->sLink[LEFT]->bIsRed = FALSE;
                }
              // caso 1.2 nenhum neto vermelho
              else
                {
                  //
                  //
                  //             y                                      y
                  //            red        change color(x & y)        black
                  //           /   \                                 /     \
                  //          x    nil                              x      nil
                  //        black                                  red
                  //       /     \          ------------->       /     \
                  //     T1       T2                            T1      T2
                  //
                  //      x                                   x
                  //     red       change color(x & y)      black
                  //    /   \                              /     \
                  // nil     y                         nil          y
                  //       black                                   red 
                  //      /     \      ------------>             /     \
                  //    T1       T2                            T1       T2 
                  //
                  slkNode->bIsRed = FALSE;
                  if (slkNode->sLink[!iDir] != NULL)
                    slkNode->sLink[!iDir]->bIsRed = TRUE;
                }
            }
          // caso 2.1.1, 2.1.2, 2.2.1 e 2.2.2
          // pai do noh excluido eh preto
          else                  // if (slkNode->bIsRed == FALSE)
            {
              // caso 2.1.1 e 2.1.2
              // irmaoh do noh excluido eh vermelho
              if (slkNode->sLink[!iDir] != NULL &&
                  slkNode->sLink[!iDir]->bIsRed == TRUE)
                {
                  // caso 2.1.1
                  // existe um bisneto vermelho em uma sequencia zig zag
                  if (RedSons (slkNode->sLink[!iDir]->sLink[iDir]))
                    {
                      //          z                             z                               y
                      //        black                         black                           black
                      //       /     \                       /     \                        /       \
                      //      w       nil                   y       nil                    w         z
                      //     red                           black                          red      black
                      //    /   \      LeftRotate(w)      /     \      RightRotate(z)    /   \    /     \
                      //  T1     y                        w      T4                     T1    x   T4    nil
                      //       black   ------------>     red           ------------->       black
                      //      /     \                   /   \                              /     \
                      //     x       T4               T1     x       change color(x)     T2       T3
                      //    red                             red
                      //   /   \                           /   \
                      // T2     T3                       T2     T3
                      //
                      //       w                               w                                    x
                      //     black                           black   color change(y)              black
                      //    /     \                         /     \                             /       \
                      // nil       z    RightRotate(z)   nil       x      LeftRotate(w)        w         z
                      //          red                            black                       black      red 
                      //         /   \    ------------->        /     \    ------------>    /     \    /   \
                      //        x     T4                      T1       z                 nil      T1   y    T4
                      //      black                                   red                            black
                      //     /     \                                 /   \                          /     \
                      //   T1       y                               y     T4                      T2       T3
                      //           red                             red
                      //          /   \                           /   \
                      //        T2     T3                       T2     T3
                      //
                      slkNode->sLink[!iDir] =
                        Rotate (slkNode->sLink[!iDir], iDir);
                      slkNode = Rotate (slkNode, !iDir);

                      if (slkNode->sLink[!iDir]->sLink[iDir] != NULL)
                        slkNode->sLink[!iDir]->sLink[iDir]->bIsRed = FALSE;
                    }
                  // caso 2.1.2 
                  // nenhum bisneto vermelho em uma sequencia zig zag
                  else
                    {
                      //          z                                  x
                      //        black                              black
                      //       /     \                            /     \
                      //      x       nil      RightRotate(z)   T1       z
                      //     red                                       black
                      //    /   \          ------------->             /     \
                      //  T1     y                                   y       nil
                      //       black       change color (x & y)     red
                      //      /     \                              /   \
                      //    T2       T3                          T2     T3
                      //
                      //       x                                  z
                      //     black     change color(z & y)      black
                      //    /     \                            /     \
                      // nil       z      LeftRotate(x)       x       T3
                      //          red                       black
                      //         /   \   ------------>     /     \
                      //        y     T3                nil       y
                      //      black                              red
                      //     /     \                            /   \
                      //   T1       T2                        T1     T2
                      slkNode = Rotate (slkNode, !iDir);

                      if (slkNode->sLink[iDir] != NULL)
                        slkNode->sLink[iDir]->bIsRed = FALSE;

                      if (slkNode->sLink[iDir]->sLink[!iDir] != NULL)
                        slkNode->sLink[iDir]->sLink[!iDir]->bIsRed = TRUE;
                    }
                }
              // caso 2.2.1 e 2.2.2
              // irmaoh do noh excluido eh preto
              else
                {
                  if (slkNode->sLink[!iDir] != NULL)
                    {
                      // caso 2.2.1 - pai preto, irmaoh preto, sobrinho vermelho
                      if (RedSons (slkNode->sLink[!iDir]))
                        {
                          //           z                              z                              y
                          //         black                          black     change color(y)      black
                          //        /     \                        /     \                       /       \
                          //       x       nil                    y       nil   ---------->     x         z
                          //     black                           red                          black     black
                          //    /     \      LeftRotate(x)      /   \    RightRotate(z)      /     \   /     \
                          //  T1       y                       x     T3                    T1      T2 T4      nil
                          //          red   ------------>    black
                          //         /   \                  /     \
                          //       T2     T3              T1       T2
                          //
                          //        x                             x                                  y
                          //      black                         black       change color(y)        black
                          //     /     \                       /     \                           /       \
                          //  nil       z                   nil       y    LeftRotate(x)       x           z
                          //          black                          red                     black       black
                          //         /     \    RightRotate(z)      /   \  ------------>    /     \     /     \
                          //        y       T3                    T1     z               nil       T1 T2       T3
                          //       red                                 black
                          //      /   \         ------------->        /     \
                          //    T1     T2                           T2       T3
                          //

                          if (WhoIsRed (slkNode->sLink[!iDir]) == iDir)
                            {
                              slkNode->sLink[!iDir] =
                                Rotate (slkNode->sLink[!iDir], iDir);
                            }
                          // zig-zig (RIGHT RIGHT OU LEFT LEFT)
                          slkNode = Rotate (slkNode, !iDir);

                          slkNode->bIsRed = FALSE;
                          slkNode->sLink[!iDir]->bIsRed = FALSE;
                        }
                      // caso 2.2.2 - pai preto, irmaoh preto, naoh tem sobrinho vermelho
                      else
                        {
                          //           y                              y
                          //         black                          black
                          //        /     \                        /     \
                          //       x       nil                    x       nil
                          //     black                           red
                          //    /     \     Color change(x)     /   \
                          //  T1       y                       x     T3
                          //
                          //        x                             x
                          //      black                         black
                          //     /     \                       /     \
                          //  nil       y                   nil       y
                          //          black                          red
                          //         /     \    Color change(y)     /   \
                          //        y       T3                    T1     T2
                          //
                          slkNode->sLink[!iDir]->bIsRed = TRUE;

                          Balance = TRUE;
                        }
                    }
                }
            }
        }

      //////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////

    }

  // retorna a si mesmo
  return slkNode;

}


/*
    Funcao:
        DelNodeRb

    Proposito:
        Remover um bloco de dados em uma arvore Rubro Negra

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
        RemoveNodeRB que irah percorrer a arvore.
*/
void
DelNodeRb (SBINTREE * BST, void *pData)
{
  SLINK slkFind;

  slkFind = CreateNode (BST, pData);    // cria o noh e passa o endereco
  if (slkFind)                          // conseguiu alocar o noh
    {
      BSTRoot->sLink[RIGHT] =
        RemoveNodeRB (BST, BSTRoot->sLink[RIGHT], slkFind);
    }
}
