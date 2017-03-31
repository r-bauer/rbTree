/*
  Arquivo: BTGEN.H
  Autor: Roberto Bauer
  Observacoes:

*/

#ifndef __BTGEN_H__
#define __BTGEN_H__

#undef EXTERN
#ifdef __BTGEN_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif

#define RIGHT   0
#define LEFT    1


//  tipo de dados abstrato noh
typedef struct tagSNODE
{
    struct tagSNODE *sLink[2];  //  ligacaoh para os nosh das subarvores
    void        *pData;     //  ponteiro generico de dados

    // dado utilizado somente por arvores AVL
    int     iHeight;    // altura do noh na arvore AVL

    // dado utilizado somente por arvores Rubro negras
    int     iColor;     // cor noh na arvore rubro negra
    BOOL        bIsRed;

} SNODE;

// define um tipo ponteiro para o noh
typedef SNODE *      SLINK;



// estrutura de dados
typedef struct tagSBINTREE
{
    SLINK           slkDummyRoot;   // noh raiz falso, simplifica operacoes
    unsigned int    uiCount;        // total de nohs na arvore
    BOOL            bBalance;       // controla as rotinas de balanceamento

    void *  ( * fCreateData )       ( void * );    //      Criar Dado
    //        Eh passado um ponteiro do objeto definido para a
    //        aplicacaoh, espera-se o retorno de um ponteiro
    //        para o que venha a ser o dado armazenado na arvore.

    BOOL    ( * fDeleteData )       ( void * );    //      Apagar Dado
    //        Eh passado um ponteiro do objeto definido para a
    //        aplicacaoh que deve estar armazenado na arvore
    //        binaria. fDeleteData deve destruir o objeto

    int     ( * fDuplicatedNode )   ( SLINK, SLINK );    //      Noh Duplicado
    //        Eh passado dois ponteiro. O primeiro ponteiro eh o
    //        noh que deve ser adicionado e o segundo eh o noh que
    //        jah se encontra na arvore, mas que eh igual(duplicada)
    //        a informacao do primeiro ponteiro.
    //        fDuplicateNode retorna:
    //              0 -> destruir duplicada
    //              1 -> adicionar duplicada

    int     ( * fNodeDataCmp )      ( void *, void * );    //      Compara Dados do Noh
    //      fNodeDataCmp:       Eh passado dois ponteiros do objeto definido para o
    //                          dado e deve compara-lo, retornando um numero que eh
    //                          < 0, zero, ou > 0, dependendo da relacaoh entre o
    //                          primeiro e segundo objeto.


    int     iMaxDepth;  // a profundidade da arvore
    char    *pPath;     // o caminho da funcaoh recursiva PrintTree

    int     ( * fTraversal )      ( void *, SLINK, int ); // percorre a arvore
    //      fTraversal:        funcao de auxilio para percorrer a arvore
    //              recursivamente, usada para gerar log e debugs.
    //              saoh utilizados dois ponteiros, o primeiro para saida
    //              de dados, e o segundo para entrada de dados, o
    //              inteiro indica o nivel da arvore em que a rotina
    //              estah executando.

} SBINTREE;




// primitivas genericas da arvore binaria
EXTERN SBINTREE *CreateBinTree( void *  ( * fCreateData )       ( void * ),         // criar dado
                                BOOL    ( * fDeleteData )       ( void * ),         // apaga dado
                                int     ( * fDuplicatedNode )   ( SLINK, SLINK ),   // noh duplicado
                                int     ( * fNodeDataCmp )      ( void *, void *),  // compara
                                // auxilio de teste
                                int     ( * fTraversal )        ( void *, SLINK, int));// percorre noh

EXTERN SLINK CreateNode(SBINTREE *, void *);
EXTERN BOOL AddNode( SBINTREE *, void *);
EXTERN BOOL DeleteNode( SBINTREE *, void * );
EXTERN SLINK FindNode( SBINTREE *, void * );
EXTERN BOOL DestroyBinTree( SBINTREE * );


typedef enum tagETRAVERSAL {
    INORDER,
    PREORDER,
    POSTORDER
} ETRAVERSAL;

// rotinas de teste
EXTERN BOOL TraversalTree(void *, SBINTREE *, ETRAVERSAL );
EXTERN BOOL PrintForest(FILE *, SBINTREE *);

#endif  // #define __BTGEN_H__
