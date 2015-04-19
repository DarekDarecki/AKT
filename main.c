#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "markov.h"
#define MAXPLIKI 10

int NPREF = 2;
int main (int argc, char **argv){

char NONWORD[] = "\n"; /* znak ktory na pewno nie bedzie slowem */

char *plik_posredni = NULL;
char *plik_out = NULL;
char *plik_staty = NULL;
char *nazwa[MAXPLIKI];
int ifstats = 0, c, nwords = 100;
int b = 0;  
//      wywołanie jako ./akt [-f 1.txt -f 2.txt -f 3.txt lub -p plik posrednie.txt][-w liczba słów] [-a liczba
// akapitów] [-g liczba] [-o plik wyjsciowy.txt] [-s statystyki.txt]
  
while ((c = getopt (argc, argv, "f:w:n:o:p:s:")) != -1){
    switch (c){
        case 'f':
                nazwa[b++] = optarg;
                break;
        case 'w':
                nwords = atoi(optarg);
                break;
        case 'n':
                NPREF = atoi(optarg);
                break;
        case 'o':
                plik_out = optarg;
                break;
        case 'p':
                plik_posredni = optarg;
                break;
        case 's':
                ifstats = 1;
                plik_staty = optarg;
                break;
        default:
                fprintf (stderr,"Unexpected error\n");
                exit (EXIT_FAILURE);
    }
}    

    FILE *st = fopen(plik_staty, "w");
    FILE *out = fopen(plik_out, "w");
    FILE *po = fopen(plik_posredni, "w");
    int i;
    char *prefix[NPREF];
    for (i = 0; i < NPREF; i++) /* ustawiamy znak "\n" jako pierwszy prfiks */
    {
        prefix[i] = NONWORD;
    }
    printf("\nPliki z tekstem zrodlowym \t= \t");
    for (i = 0; i < b; i++){
        printf("%s, ", nazwa[i]);
        build(prefix, fopen(nazwa[i], "r"));
    }
    add(prefix, NONWORD);
    generate(nwords, ifstats, st, out);
    ngrams(st, po, ifstats);
    printf("\nPlik z wygenerowanym tekstem \t= \t%s,\n", plik_out);
    printf("Plik posredni z ngramami \t= \t%s,\n", plik_posredni);
    if (ifstats)
        printf("Plik ze statystykami \t\t= \t%s,\n", plik_staty);
    printf("Wybrane ngramy \t\t\t= \t%d-gramy,\n", NPREF);
    printf("Maksymalna liczba slow \t\t= \t%d.\n\n", nwords);
    return 0;
}
