#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "markov.h"
#define MAXPLIKI 10

const int NPREF = 2;
const int MAXGEN = 90;
int main (int argc, char **argv){

char NONWORD[] = "\n"; /* znak ktory na pewno nie bedzie slowem */

char *liczba_slow = NULL;
char *liczba_akapitow = NULL;
char *liczba = NULL;
char *plik_out = NULL;
char *staty = NULL;
char *nazwa[MAXPLIKI];
int index;
int c;

  
// 		wywołanie jako ./akt [-f 1.txt -f 2.txt -f 3.txt lub -p plik posrednie.txt][-w liczba słów] [-a liczba
// akapitów] [-g liczba] [-o plik wyjsciowy.txt] [-s statystyki.txt]
/*
  
while ((c = getopt (argc, argv, "f:p:w:a:g:o:s")) != -1)
	switch (c){
		case 'f':
			nazwa[i++] = optarg;
		        break;
		case 'p':
			if (i != 0){
				fprintf(stderr, "Mozna podac tylko 1 plik posredni\n");
				exit(EXIT_FAILURE);
			}	
		        nazwa[i++] = optarg;
		        break;
		case 'w':
		        liczba_slow = atoi(optarg);
		        break;
		case 'a':
		        liczba_akapitow = atoi(optarg);
		        break;
		case 'g':
		        liczba = atoi(optarg);
		        :break;
		case 'o':
		        plik_out = optarg;
		        break;
		case 's':
		        staty = optarg;
		        break;

	}
*/
    int i,nwords = MAXGEN;
    char *prefix[NPREF];
    for (i = 0; i < NPREF; i++) /* ustawiamy znak "\n" jako pierwszy prfiks */
    {
        prefix[i] = NONWORD;
    }
    build(prefix, fopen("raz.txt", "r"));
    add(prefix, NONWORD);
    generate(nwords);
    //drukuj();
    printf("\n");
    return 0;
}

