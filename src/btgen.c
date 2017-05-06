/*
  Arquivo: BTGEN.C
  Autor: Roberto Bauer
  Observacoes:
              Rotinas de uma estrutura de dados do tipo arvore binaria
*/

#define __BTGEN_C__

#include "main.h"

//  Redefinicoes para aumentar legibilidade do codigo
#define BTRoot              (BT->slkDummyRoot)
#define NodeCount           (BT->uiCount)   // nohs na arvoreh atual

#define CreateData          (*(BT->fCreateData))
#define DeleteData          (*(BT->fDeleteData))
#define DuplicatedNode      (*(BT->fDuplicatedNode))
#define NodeDataCmp         (*(BT->fNodeDataCmp))


#define Path                (BT->pPath)
#define MaxDepth            (BT->iMaxDepth)

#define Traversal           (*(BT->fTraversal))



PRIVATE void InOrder (void *, SBINTREE *, SLINK, int);
PRIVATE void PreOrder (void *, SBINTREE *, SLINK, int);
PRIVATE void PostOrder (void *, SBINTREE *, SLINK, int);

PRIVATE void DelTree (SBINTREE *, SLINK);

PRIVATE void PrintTree (FILE *, SBINTREE *, SLINK, int);

/*
    Funcao: 
              CreateBinTree

    Proposito: 
              Cria e inicia uma estrutura de gerenciamento de uma arvoreh binariah

    Parametros:   
          ponteiros p/ a funcoesh especificas da arvoreh
        void *(*fCreateData)(void *)              -> cria dado
        int   (*fDeleteData)(void *)              -> apaga dado
        int   (*fDuplicatedNode)(SLINK, SLINK)    -> o que fazer c/ dado duplicado
        int   (*fNodeDataCmp)(void *, void *)     -> compara nohs
        int   (*fTraversal)( FILE *, SLINK, int) -> percorre a arvore
    
    Retorno:
          ponteiro SBINTREE
            pBT  - ponteiro para uma estrutura do tipo arvoreh binariah inicializada
            NULL - naoh conseguiu criar a arvoreh binariah

    Descricao:
              Cria uma estrutura de arvoreh binariah e retorna um ponteiro da 
              mesma. Em erro, retorna NULL.
              Esta funcaoh aceita ponteiros para as cinco funcoesh especificas
              de uma arvoreh e inicializa a estrutura com elas, aloca o noh que
              irah apontar para a raiz da arvoreh.
*/
SBINTREE *
CreateBinTree (void *(*fCreateData) (void *),
               BOOL (*fDeleteData) (void *),
               int (*fDuplicatedNode) (SLINK, SLINK),
               int (*fNodeDataCmp) (void *, void *),
               int (*fTraversal) (void *, SLINK, int))
{
  SBINTREE *pBT;

  // aloca uma estrutura que gerencia uma arvore binaria
  pBT = (SBINTREE *) malloc (sizeof (SBINTREE));

  if (pBT)
    {                           //  caso fracasse em alocar memoria
      // tendo alocado, inicializa as variaveis 
      pBT->slkDummyRoot = (SLINK) malloc (sizeof (SNODE));

      // naoh criou o noh raiz falso
      if (pBT->slkDummyRoot == NULL)
        {
          // libera a estrutura de dados arvore
          free (pBT);
          return (NULL);        // indicando o erro
        }
      // tendo sucesso na alocacaoh, inicializa as variaveis
      pBT->slkDummyRoot->sLink[RIGHT] = NULL;
      pBT->slkDummyRoot->sLink[LEFT] = NULL;
      pBT->slkDummyRoot->pData = NULL;

      pBT->uiCount = 0;

      // armazena as funcoesh especificas p/ tratamento da arvore
      pBT->fCreateData = fCreateData;
      pBT->fDeleteData = fDeleteData;
      pBT->fDuplicatedNode = fDuplicatedNode;
      pBT->fNodeDataCmp = fNodeDataCmp;
      pBT->fTraversal = fTraversal;

      return (pBT);             // devolve o enderecoh da arvoreh
    }
  else                          //  caso fracasse em alocar memoria
    return NULL;                // retorna a indicacaoh
}

/*
    Funcao: 
              CreateNode

    Proposito: 
              Criar um noh para a arvoreh binariah

    Parametros:
              SBINTREE *BT -> estrutura arvoreh c/ ponteiro para a funcaoh CreateData
              void *data -> ponteiro generico c/ o dado do noh

    Retorno:
              slkNewNode - ponteiro para uma estrutura do tipo noh
              NULL - naoh conseguiu criar um noh

    Descricao:
              Cria um noh e entaoh chama a funcaoh especifica da aplicacaoh
              CreateData() para criar o dado da estrutura do noh.
              Em caso de falha retorna NULO

*/
SLINK
CreateNode (SBINTREE * BT, void *pData)
{
  SLINK slkNewNode;

  // aloca os ponteiros para seguinte, anterior e dado para o noh
  slkNewNode = (SLINK) malloc (sizeof (SNODE));
  if (slkNewNode == NULL)       // caso fracasse em alocar memoria
    return (NULL);              // retorna indicacaoh de NULO

  // tendo sucesso na alocacaoh, inicializa as variaveis
  slkNewNode->sLink[RIGHT] = NULL;
  slkNewNode->sLink[LEFT] = NULL;

  // agora chama a aplicacaoh especifica para alocacah de dados
  slkNewNode->pData = CreateData (pData);

  // caso naoh consiga alocar memoria para o dado do noh
  if (slkNewNode->pData == NULL)
    {
      free (slkNewNode);        // libera a memoria previamente alocada
      return (NULL);            // retorna NULO para indicar falha
    }
  // alocou toda a memoria nescessaria
  else
    {
      return (slkNewNode);      // retorna o noh criado
    }
}

/*
    Funcao: 
              AddNode

    Proposito: 
              Adiciona um noh na arvore

    Parametros:
              SBINTREE *BT   -> ponteiro para estrutura de dados que trata a arvore
                            c/ ponteiros paras funcoes de comparacaoh e tratamento de duplicados
              void *pData -> noh a ser inserido na arvoreh

    Retorno:
            TRUE  - adicionou na arvoreh o noh, 
                   caso seja duplicata, remove, adiciona ou ignora, 
                   depende da configuracaoh da arvore
            FALSE - naoh conseguiu criar um noh para inserir na arvore

    Descricao:
              Adiciona um noh numa arvoreh binariah
*/
BOOL
AddNode (SBINTREE * BT, void *pData)
{
  SLINK slkNew;                 // ponteiro de para o noh a ser inserido na arvoreh
  SLINK slkPrev;                // ponteiro do link anterior da arvoreh durante a busca 
  SLINK slkCurr;                // ponteiro do link atual da arvoreh durante a busca 
  int iAction;                  // acaoh a ser tomada em caso de nohs duplicados
  int iCompare;                 // resultado da comparacaoh entre dois nohs
  int iDir;                     //  seguir direita/esquerda

  slkNew = CreateNode (BT, pData);      // cria o noh e passa o endereco 
  if (slkNew == NULL)           // naoh conseguiu alocar o noh
    return FALSE;               // retorna indicando fracasso

  // ignora o tratamento especial que se faz para a raiz da arvoreh
  // utiliza um noh falso como raiz da arvore
  // e passa a tratar o noh inicial(raiz) como um noh igual aos demais
  // simplificando a logica de tratamento
  slkPrev = BTRoot;
  // a raiz falsa utiliza apenas a ramificacaoh direita
  iDir = RIGHT;
  // igual ao slkCurr = BTRoot->sLink[RIGHT]
  slkCurr = slkPrev->sLink[RIGHT];

  // desce a arvoreh em busca de uma ramo vazio.
  while (slkCurr != NULL)
    {
      // avanca a posicaoh 1 - atualiza anterior
      slkPrev = slkCurr;
      // Decide qual direcaoh seguir
      iCompare = NodeDataCmp (slkNew->pData, slkCurr->pData);
      if (iCompare == 0)        // noh duplicado
        {
          // o que fazer com noh duplicado
          iAction = DuplicatedNode (slkNew, slkCurr);

          // fDuplicateNode retorna:
          //     1 -> adicionar duplicada
          if (iAction)
            {
              // nada faz
            }
          // fDuplicateNode retorna:
          //     0 -> destruir duplicada
          else                  // if (iAction == 0) 
            {
              // naoh inclui o duplicado
              // apagamos os dados que dependem da implementacaoh do objeto
              DeleteData (slkNew->pData);
              // desalocamos o noh 
              free (slkNew);
              return FALSE;
            }
        }
      // comparacaoh retorna -1(menor), 0(igual) e 1(maior)
      iDir = iCompare < 0;
      // corrige <= (-1 e 0) para 1(LEFT) e >(1) para 0(RIGHT)
      // avanca a posicaoh 2 - atualiza atual
      slkCurr = slkCurr->sLink[iDir];
    }

  // adiciona novo noh
  slkPrev->sLink[iDir] = slkNew;

  // soma o novo noh no total
  ++NodeCount;

  return TRUE;
}


/*
    Funcao: 
              DeleteNode

    Proposito: 
              Apaga um noh da arvore binaria

    Parametros:
              SBINTREE *BT -> ponteiro para estrutura de dados que trata a arvore
                            c/ ponteiros paras funcoes de comparacaoh e excluir dados
              void *pData -> informacaoh que devera ser excluida da arvoreh

    Retorno:
              TRUE  - exclui o noh e atualizou a arvore
              FALSE - naoh encontrou um noh equivalente p/ exclusaoh

    Descricao:
              Apaga um noh que tenha os dados indicado por pData.
              Verifica se o noh se encontra na arvore,
              Elimina o noh e atualiza a referencia aos demais nohs.
*/
BOOL
DeleteNode (SBINTREE * BT, void *pData)
{
  SLINK slkTmp;                 // noh temporario com o dado de busca
  SLINK slkPrev;                // ponteiro da posicao anterior na arvore
  SLINK slkCurr;                // ponteiro da posicao atual na arvore
  SLINK slkDel;                 // ponteiro para o noh da arvore a ser excluido
  int iCompare;                 // resultado da comparacaoh entre dois nohs
  int iDir;                     // seguir direita/esquerda

  slkTmp = CreateNode (BT, pData);      // formata os dados dentro do noh temporario
  if (slkTmp == NULL)           // naoh conseguiu alocar o noh
    return FALSE;               // retorna indicando fracasso

  // inicia com o noh falso da raiz
  slkPrev = BTRoot;
  iDir = RIGHT;                 // a raiz falsa utiliza apenas a ramificacaoh direita
  slkCurr = slkPrev->sLink[RIGHT];      // igual ao slkCurr = BTRoot->sLink[RIGHT]

  // desce a arvoreh em busca de uma ramo vazio.
  while (slkCurr != NULL &&
         (iCompare = NodeDataCmp (slkTmp->pData, slkCurr->pData)) != 0)
    {
      iDir = iCompare < 0;      // comparacaoh retorna -1(menor), 0(igual) e 1(maior)
      // corrige <= (-1 e 0) para 1(LEFT) e >(1) para 0(RIGHT)
      slkPrev = slkCurr;        // atualiza anterior
      slkCurr = slkCurr->sLink[iDir];   // atualiza atual
    }

  DeleteData (slkTmp->pData);   // apaga o dado do noh
  free (slkTmp);                // libera a memoria do noh temporario

  if (slkCurr == NULL)          // naoh encontrou o noh
    return FALSE;

  // salva a posicaoh do noh a ser excluido
  slkDel = slkCurr;

  // primeiro caso
  // naoh tem filho direito
  // obs.: atende quando naoh tem ambos os filhos
  //
  //              3                         3 
  //           /     \        ==>        /     \
  //          2       5                 1       5
  //        /  `    /   \                     /   \
  //       1       4     6                   4     6
  //
  // substitui com filho esquerdo
  if (slkCurr->sLink[RIGHT] == NULL)
    {
      slkCurr = slkCurr->sLink[LEFT];
    }
  // segundo caso
  // o filho direito naoh tem descendente esquerdo
  //
  //              4                          5 
  //           /     \        ==>         /     \
  //          2       5                  2       6
  //        /   \    ´  \              /   \
  //       1     3       6            1     3        
  //
  // substitui o noh pai com filho direito
  // e coloca o noh irmaoh esquerdo como filho esquerdo   
  else if (slkCurr->sLink[RIGHT]->sLink[LEFT] == NULL)
    {
      slkCurr = slkCurr->sLink[RIGHT];
      slkCurr->sLink[LEFT] = slkDel->sLink[LEFT];
    }
  // ultimo caso
  // encontre o menor descendente esquerdo do filho direito
  //
  //              4                          5 
  //           /     \        ==>         /     \
  //          2       8                  2       8
  //         / \     / \                / \     / \
  //        1   3   5   7              1   3   6   7
  //               ´ \
  //                  6
  //
  // substitui o noh pai pelo menor descendente 
  // do filho direito.
  // por ser o menor, naoh tera filho esquerdo, 
  // caso o menor tenha filho direito, 
  // este assume sua antiga posicaoh.
  else
    {
      SLINK slkSmallFather;     // aponta para o pai do menor noh esquerdo

      // procura o menor noh
      slkSmallFather = slkCurr->sLink[RIGHT];
      while (slkSmallFather->sLink[LEFT]->sLink[LEFT])
        slkSmallFather = slkSmallFather->sLink[LEFT];

      //  aponta para o menor noh da ramificacaoh
      // 'slkCurr' ira ocupar a posicaoh do noh a ser
      // excluido
      slkCurr = slkSmallFather->sLink[LEFT];

      // o neto direito substitui a posicaoh do filho
      slkSmallFather->sLink[LEFT] = slkCurr->sLink[RIGHT];
      // coloca a referencia dos filhos do noh a ser excluido 
      // no noh que irah substitui-lo.
      slkCurr->sLink[LEFT] = slkDel->sLink[LEFT];
      slkCurr->sLink[RIGHT] = slkDel->sLink[RIGHT];
    }

  // atualiza a informacaoh do pai do noh excluido
  slkPrev->sLink[iDir] = slkCurr;

  DeleteData (slkDel->pData);   // apaga o dado do noh
  free (slkDel);                // libera a memoria do noh

  --NodeCount;                  // subtrai o noh do total

  return TRUE;
}

/*
    Funcao: 
              FindNode

    Proposito: 
              Encontra um noh da arvore binaria

    Parametros:
              SBINTREE *BT -> ponteiro para estrutura de dados que trata a arvore
                            c/ a funcaoh p/comparacaoh de dados
              void *pData -> informacaoh que devera ser encontrada na arvoreh

    Retorno:
              slkCurr - ponteiro para o noh 
              NULL    - naoh encontrou um noh equivalente

    Descricao:
            Realiza uma formatacaoh nos dados apontados por pData,
            para poder realizar a comparacaoh necessaria durante o
            percurso de busca do noh equivalente.
*/
SLINK
FindNode (SBINTREE * BT, void *pData)
{
  SLINK slkTmp;                 // noh temporario com o dado de busca
  SLINK slkCurr;                // ponteiro da posicao atual na arvore
  int iCompare;                 // resultado da comparacaoh entre dois nohs
  int iDir;                     // seguir direita/esquerda

  // formata os dados dentro do noh temporario
  slkTmp = CreateNode (BT, pData);
  if (slkTmp == NULL)           // naoh conseguiu alocar o noh
    return NULL;                // retorna indicando fracasso

  // inicia com o noh falso da raiz
  slkCurr = BTRoot->sLink[RIGHT];

  // desce a arvoreh em busca de uma ramo vazio.
  while (slkCurr != NULL &&
         (iCompare = NodeDataCmp (slkTmp->pData, slkCurr->pData)) != 0)
    {
      // corrige -1 e 0(<=) para 1(LEFT) e 1(>) para 0(RIGHT)
      iDir = iCompare < 0;
      slkCurr = slkCurr->sLink[iDir];   // atualiza atual
    }

  // apaga o dado do noh
  DeleteData (slkTmp->pData);
  // libera a memoria do noh temporario
  free (slkTmp);

  return slkCurr;
}

/*
    Funcao: 
              InOrder

    Proposito: 
              Percorrer uma arvore binaria ordenamente

    Parametros:
              void *pOut -> aponta a saida dos dados, usada pela funcaoh que percorre a arvore
              SBINTREE *BT -> estrutura de dados da arvore c/ a rotina de travessia
              SLINK slk -> noh atual da arvore sendo percorrida recursivamente
              int iLevel -> profundidade na arvore em que a rotina se encontra

    Retorno:
            nenhum

    Descricao:
              Acessa os nohs da arvoreh em sequencia ordenada, executando a rotina
              apontada por fTraversal para tratar o noh.
*/
PRIVATE void
InOrder (void *pOut, SBINTREE * BT, SLINK slk, int iLevel)
{
  if (slk)
    {
      InOrder (pOut, BT, slk->sLink[LEFT], iLevel + 1);
      Traversal (pOut, slk, iLevel);
      InOrder (pOut, BT, slk->sLink[RIGHT], iLevel + 1);
    }
}

/*
    Funcao: 
              PreOrder

    Proposito: 
              Percorrer uma arvore binaria pre-ordenada

    Parametros:
              void *pOut -> aponta a saida dos dados, usada pela funcaoh que percorre a arvore
              SBINTREE *BT -> estrutura de dados da arvore c/ a rotina de travessia
              SLINK slk -> noh atual da arvore sendo percorrida recursivamente
              int iLevel -> profundidade na arvore em que a rotina se encontra

    Retorno:
            nenhum

    Descricao:
              Acessa os nohs da arvoreh em sequencia pre-ordenada, executando a rotina
              apontada por fTraversal para tratar o noh.
*/
PRIVATE void
PreOrder (void *pOut, SBINTREE * BT, SLINK slk, int iLevel)
{
  if (slk)
    {
      Traversal (pOut, slk, iLevel);
      PreOrder (pOut, BT, slk->sLink[LEFT], iLevel + 1);
      PreOrder (pOut, BT, slk->sLink[RIGHT], iLevel + 1);
    }
}

/*
    Funcao: 
              PostOrder

    Proposito: 
              Percorrer uma arvore binaria em pos-ordem

    Parametros:
              void *pOut -> aponta a saida dos dados, usada pela funcaoh que percorre a arvore
              SBINTREE *BT -> estrutura de dados da arvore c/ a rotina de travessia
              SLINK slk -> noh atual da arvore sendo percorrida recursivamente
              int iLevel -> profundidade na arvore em que a rotina se encontra

    Retorno:
            nenhum

    Descricao:
              Acessa os nohs da arvoreh em sequencia pos-ordenada, executando a rotina
              apontada por fTraversal para tratar o noh.
*/
PRIVATE void
PostOrder (void *pOut, SBINTREE * BT, SLINK slk, int iLevel)
{
  if (slk)
    {
      PostOrder (pOut, BT, slk->sLink[LEFT], iLevel + 1);
      PostOrder (pOut, BT, slk->sLink[RIGHT], iLevel + 1);
      Traversal (pOut, slk, iLevel);
    }
}

/*
    Funcao: 
              TraversalTree

    Proposito: 
              Percorrer uma arvore binaria executando rotinas em ordem especifica

    Parametros:
              void *pOut -> aponta a saida dos dados, usada pela funcaoh que percorre a arvore
              SBINTREE *BT -> Arvoreh a ser atravessada e rotina de travessia
              ETRAVERSAL eType -> tipo de travessia, ordenada, posordenada ou preordenada

    Retorno:
            TRUE - Travessia de arvoreh completada
            FALSE - Arvore vazia

    Descricao:
              Ponto de entrada para as rotinas de travessia recursivas, executa as 
              acoes indicadas por fTraversal na ordem indicada.
*/
BOOL
TraversalTree (void *pOut, SBINTREE * BT, ETRAVERSAL eType)
{
  if (BTRoot->sLink[RIGHT])
    {
      switch (eType)
        {
        default:
        case INORDER:
          InOrder (pOut, BT, BTRoot->sLink[RIGHT], 0);
          break;

        case PREORDER:
          PreOrder (pOut, BT, BTRoot->sLink[RIGHT], 0);
          break;

        case POSTORDER:
          PostOrder (pOut, BT, BTRoot->sLink[RIGHT], 0);
          break;
        }
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}



/*
    Funcao: 
              DelTree

    Proposito: 
              Libera nohs de uma arvore binaria em pos-ordem

    Parametros:
              SBINTREE *BT -> estrutura de dados da arvore c/ a rotina de exclusaoh do noh
              SLINK slk -> noh atual da arvore sendo percorrida recursivamente

    Retorno:
            nenhum

    Descricao:
              Acessa os nohs da arvoreh em sequencia pos-ordenada, eliminando o noh
              da posicaoh atual, por ser uma travessia pos-ordenada, o noh liberado
              naoh serah referenciado novamente durante o resto do percurso
*/
PRIVATE void
DelTree (SBINTREE * BT, SLINK slk)
{
  if (slk)
    {
      DelTree (BT, slk->sLink[LEFT]);
      DelTree (BT, slk->sLink[RIGHT]);

      DeleteData (slk->pData);  // apaga o dado do noh
      free (slk);               // libera a memoria do noh

      --NodeCount;              // subtrai o noh do total
    }
}


/*
    Funcao: 
          DestroyBinTree

    Proposito: 
          Libera toda a memoria de uma arvore binaria previamente criada

    Parametros:
          SBINTREE *BT -> Arvoreh a ser destruida

    Retorno:
          TRUE - memoria  da estrutura da arvore e todos os nohs liberados
          FALSE - arvoreh invalida

    Descricao:
           Ponto de entrada para as rotinas de travessia recursivas, executa a
          a rotina DelTree para eliminar nohs individualmente, terminado o
          processo de atravessar e liberar os nohs, libera o noh raiz falso
          e a estrutura de dados que gerencia a arvore.

*/
BOOL
DestroyBinTree (SBINTREE * BT)
{
  if (BT)
    {
      // percorre a arvore em pos ordem,
      // apagando cada noh
      DelTree (BT, BTRoot->sLink[RIGHT]);

      // libera a raiz falsa
      free (BT->slkDummyRoot);

      // libera a estrutura arvore
      free (BT);

      return TRUE;
    }
  else
    return FALSE;
}


/*
    Funcao: 
              PrintTree

    Proposito: 
              Imprime o noh da arvore e linhas de ramificacaoh

    Parametros:
              FILE *fOut -> arquivo em que a arvore sera impressa.
              SBINTREE *BT -> Arvoreh a ser desenhada
                            Path - buffer com o caminho percorrido pela funcaoh recursiva
                            MaxDepth - indica a profundidade(altura) da arvore impressa
              SLINK slk -> noh atual da arvore sendo percorrida recursivamente
              int iLevel -> profundidade na arvore em que a rotina se encontra

    Retorno:
            nenhum

    Descricao:
              Acessa os nohs da arvoreh em sequencia ordenada da esquerda para direita,
              durante a impressaoh da noh na linha, verifica as ramificacoes anteriores
              atraves do buffer Path, sempre que ocorre desvio da esquerda p/ direita ou
              vice-versa, deve incluir '|' como indicacaoh de linha horizontal de uma
              ramificacaoh.
*/
PRIVATE void
PrintTree (FILE * fLog, SBINTREE * BT, SLINK slk, int iLevel)
{
  if (slk)
    {
      // verifica a profundidade da arvore
      //if (MaxDepth < iLevel)
      if (iLevel > MaxDepth)
        {
          MaxDepth = iLevel;
        }

      // Segue a ramificacaoh direita
      Path[iLevel] = 'R';
      PrintTree (fLog, BT, slk->sLink[RIGHT], iLevel + 1);

      // imprime o noh atual
      {
        char *pDraw;
        int iX;

        // aloca o tamanho necessario para montar a linha ser desenhada
        // utiliza memoria do heap ao inves da pilha(stack)
        pDraw = malloc (iLevel * 3 + 4);
        if (pDraw)
          {
            pDraw[0] = ' ';
            pDraw[1] = ' ';
            pDraw[2] = ' ';
            for (iX = 1; iX < iLevel; ++iX)
              {
                // Path = RRRRLRRRL
                // sequencia RL(direita/esquerda) OU LR(esquerda/direita)
                if (Path[iX - 1] != Path[iX])
                  {
                    // desenha linha horizontal de uma ramificacaoh anterior
                    pDraw[iX * 3] = ' ';
                    pDraw[iX * 3 + 1] = '|';
                    pDraw[iX * 3 + 2] = ' ';
                  }
                // sequencia LL(esquerda/esquerda) ou RR(direita/direita)
                else
                  {
                    // naoh tem linha de ramificaoh acima
                    pDraw[iX * 3] = ' ';
                    pDraw[iX * 3 + 1] = ' ';
                    pDraw[iX * 3 + 2] = ' ';
                 }
              }
            // desenha o noh na linha atual
            pDraw[iLevel * 3] = ' ';
            pDraw[iLevel * 3 + 1] = Path[iLevel-1] == 'R' ? '/' : '\\';
            pDraw[iLevel * 3 + 2] = '-';
            pDraw[iLevel * 3 + 3] = '\0';

            fprintf (fLog, "%s", pDraw);

            free (pDraw);
          }
      }
      Traversal (fLog, slk, iLevel);
//              fprintf(fLog, " f:%2i ", slk->iHeight);
//              fprintf(fLog, " f:%2i ", slk->sLink[LEFT]->iHeight - slk->sLink[RIGHT]->iHeight);
      fprintf (fLog, "\n");

      // Segue ramificacaoh esquerda
      Path[iLevel] = 'L';
      PrintTree (fLog, BT, slk->sLink[LEFT], iLevel + 1);
    }
}

/*
    Funcao: 
              PrintForest

    Proposito: 
        Imprime em um arquivo de saida a arvore resultante
              

    Parametros:
        FILE *fOut -> arquivo em que a arvore sera impressa.
        SBINTREE *BT -> Arvoreh a ser desenhada
        Path - buffer com o caminho percorrido pela funcaoh recursiva
        MaxDepth - indica a profundidade(altura) da arvore impressa

    Retorno:
        TRUE - Impressaoh concluida
        FALSE - Arvore vazia

    Descricao:
              Ponto de entrada para a rotina de imperssaoh recursivas, 
        reserva um buffer para manter registro do caminho percorrido pela funcaoh,
        inicia a variavel de profundidade da arvore.
*/
BOOL
PrintForest (FILE * fOut, SBINTREE * BT)
{
  if (BTRoot->sLink[RIGHT])
    {
      // infelizmente naoh tem como saber o tamanho preciso da arvore
      // aloca esperando o pior caso
      Path = malloc (NodeCount);
      if (Path)
        {
          // comecah no nivel 0, logo profundidade 0
          MaxDepth = 0;
          PrintTree (fOut, BT, BTRoot->sLink[RIGHT], 0);
          // naoh mais necessarioh, libera
          free (Path);

          // informacoesh da arvore
          fprintf (fOut, "\nTotal de nohs: %u\n", NodeCount);
          fprintf (fOut, "Profundidade: %u\n", MaxDepth);

          return TRUE;
        }
    }
  else
    {
      fprintf (fOut, "Arvoreh vazia\n");
    }

  return FALSE;
}
