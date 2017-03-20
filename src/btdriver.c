
/*
  Arquivo: BTDRIVER.C
  Autor: Roberto Bauer
  Observacoes:

*/

#define __BTDRIVER_C__

#include "main.h"

#define STR_SIZE		80


PRIVATE void Insert (SBINTREE *, SBINTREE *, SBINTREE *);
PRIVATE void Delete (SBINTREE *, SBINTREE *, SBINTREE *);
PRIVATE void Search (SBINTREE *, SBINTREE *, SBINTREE *);
PRIVATE void UseFile (SBINTREE *, SBINTREE *, SBINTREE *, BOOL);
PRIVATE void Display (SBINTREE *);
PRIVATE void SaveLog (SBINTREE *);
PRIVATE void Input (const char *, char *, int);
PRIVATE char *StrUpr (char *);


void
Menu (SBINTREE * BT, SBINTREE * AVL, SBINTREE * RB)
{
  char cTmp[10];
  int iChoice;
  BOOL bLoop = TRUE;

  while (bLoop)
    {
      printf ("\n( 1 ) Inserir\n");
      printf ("( 2 ) Apagar\n");
      printf ("( 3 ) Busca\n");
      printf ("( 4 ) Carrega arquivo\n");
      printf ("( 5 ) Descarrega arquivo\n");
      printf ("( 6 ) Imprime\n");
      printf ("( 7 ) Salva\n");

      printf ("( 0 ) Sair\n\n");


      Input ("Escolha umas das alternativas anteriores : ", cTmp, 10);
      if (cTmp[0] != '\0' && (isdigit (*cTmp)))
        {
          iChoice = atoi (cTmp);
          switch (iChoice)
            {
            case 1:
              Insert (BT, AVL, RB);
              break;
            case 2:
              Delete (BT, AVL, RB);
              break;
            case 3:
              Search (BT, AVL, RB);
              break;
            case 4:
              UseFile (BT, AVL, RB, TRUE);
              break;
            case 5:
              UseFile (BT, AVL, RB, FALSE);
              break;
            case 6:
              Display (BT);
              Display (AVL);
              Display (RB);
              break;
            case 7:
              SaveLog (BT);
              SaveLog (AVL);
              SaveLog (RB);
              break;
            case 0:
              bLoop = FALSE;
              break;
            }
        }
    }
}

PRIVATE void
Insert (SBINTREE * BT, SBINTREE * AVL, SBINTREE * RB)
{
  char strWord[STR_SIZE];

  for (;;)
    {
      Input ("Incluir palavra : ", strWord, STR_SIZE);
      if (strWord[0] == '\0')
        break;

      // insere na arvore
      AddNode (BT, strWord);
      AddNodeAvl (AVL, strWord);
      AddNodeRb (RB, strWord);
    }
}

PRIVATE void
Delete (SBINTREE * BT, SBINTREE * AVL, SBINTREE * RB)
{
  char strWord[STR_SIZE];

  Input ("Excluir palavra : ", strWord, STR_SIZE);

  // apaga 
  DeleteNode (BT, strWord);
  DelNodeAvl (AVL, strWord);
  DelNodeRb (RB, strWord);
}

PRIVATE void
Search (SBINTREE * BT, SBINTREE * AVL, SBINTREE * RB)
{
  char strWord[STR_SIZE];

  Input ("Procurar palavra : ", strWord, STR_SIZE);

  // procura
  if (FindNode (BT, strWord))
    printf ("\nEncontrou noh da arvore binaria.");
  else
    printf ("\nNaoh encontrou noh.");

  if (FindNode (AVL, strWord))
    printf ("\nEncontrou noh da arvore de busca binaria AVL.");
  else
    printf ("\nNaoh encontrou noh.");

  if (FindNode (RB, strWord))
    printf ("\nEncontrou noh da arvore de busca binaria RB.");
  else
    printf ("\nNaoh encontrou noh.");
}

PRIVATE void
Input (const char *pText, char *pContent, int size)
{
  fprintf (stdout, "%s", pText);
  fgets (pContent, size - 1, stdin);
  pContent[strlen (pContent) - 1] = '\0';
}

/* 
	Funcao:
			StrUpr
	
	Proposito:
			Converte todo o conteudo de uma string em maiusculas

	Entrada:
			char *str -> string com as letras a serem modificadas

	Retorno:
			str -> ponteiro com a string contendo somente maisculas.

	Descricao:
			Percorre cada ponto da string, trocando as letras por 
			equivalentes maiusculas.
*/
PRIVATE char *
StrUpr (char *str)
{
  char *ptr = str;

  while (*ptr)
    {
      *ptr = toupper (*ptr);
      ptr++;
    }

  return (str);
}


PRIVATE void
UseFile (SBINTREE * BT, SBINTREE * AVL, SBINTREE * RB, BOOL bIns)
{
  FILE *fin;                    // arquivo de entrada
  char strWord[STR_SIZE];
  char *ptrStr;
  int iChar;
  int i = 0;

  Input ("Nome do arquivo : ", strWord, STR_SIZE);


  fin = fopen (strWord, "rt");
  if (fin)
    {
      fprintf (stdout, "\nLendo arquivo\n");

      // comecah a processar o arquivo
      while (!feof (fin))
        {
          // percorre o texto pulando os espacosh e pontuncaoesh
          do
            {
              iChar = getc (fin);
            }
          while (iChar != EOF && (isspace (iChar) || ispunct (iChar)));

          // tendo um caracter valido
          // comecah a montar a palavra
          ptrStr = strWord;
          do
            {
              *ptrStr++ = iChar;
              iChar = getc (fin);
            }
          while (iChar != EOF && !isspace (iChar) && !ispunct (iChar));

          // fim do arquivo, sai do loop
          if (iChar == EOF)
            break;

          // fecha a palavra com o terminador nulo
          *ptrStr = '\0';

          // todos os caracteres em maiuscula
          ptrStr = StrUpr (strWord);

          if (bIns)
            {
              // insere na arvore
              AddNode (BT, ptrStr);
              AddNodeAvl (AVL, ptrStr);
              AddNodeRb (RB, ptrStr);
            }
          else
            {
              // remove da arvore
              DeleteNode (BT, ptrStr);
              DelNodeAvl (AVL, ptrStr);
              DelNodeRb (RB, ptrStr);
            }
          i++;
          if ((i % 500) == 0)
            fprintf (stdout, ".");

        }                       // while (!feof(fin)) 

      fclose (fin);

      fprintf (stdout, "\nFim\n");

    }
  else
    {
      fprintf (stderr, "Naoh conseguiu encontrar/abrir o arquivo %s\n",
               strWord);
    }

}

PRIVATE void
Display (SBINTREE * BT)
{
  PrintForest (stdout, BT);
}

PRIVATE void
SaveLog (SBINTREE * BT)
{
  FILE *fLog;

  fLog = fopen ("log.txt", "at");
  if (fLog)
    {
      PrintForest (fLog, BT);

      fclose (fLog);
    }
  else
    fprintf (stderr, "ERRO AO CRIAR LOG\n");
}
