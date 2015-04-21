#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "markov.h"
#define MAXPLIKI 10

int NPREF = 2;
int main (int argc, char **argv){

char NONWORD[] = "\n"; /* znak ktory na pewno nie bedzie slowem */
FILE *st,*out,*po,*in;
char *plik_posredni = NULL;
char *plik_out = NULL;
char *plik_staty = NULL;
char *nazwa[MAXPLIKI];
int ifstats = 0, c, nwords = 100;
int b = 0, o = 0, p = 0;  
//      wywołanie jako ./akt [-f 1.txt -f 2.txt -f 3.txt] [-w liczba słów] [-n liczba] [-o plik wyjsciowy.txt] [-p posredni.txt] [-s statystyki.txt]
  
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
                o++;
                break;
        case 'p':
                p++;
                plik_posredni = optarg;
                break;
        case 's':
                ifstats = 1;
                plik_staty = optarg;
                break;
        default:
                fprintf (stderr, "Nieznany argument\n");
                exit (EXIT_FAILURE);
    }
}    
    if (ifstats)
        st = fopen(plik_staty, "w");
    if (o){
        out = fopen(plik_out, "w");
    } else {
         fprintf( stderr, "\nProsze podac plik do ktorego ma byc generowany tekst!\n" );
         exit( EXIT_FAILURE );
 
    }
    if (p){
        po = fopen(plik_posredni, "w");
    } else {
        fprintf( stderr, "\nProsze podac plik do ktorego maja byc zapisywane dane posrednie!\n" );
        exit( EXIT_FAILURE );
    }
    int i;
    char *prefix[NPREF];
    for (i = 0; i < NPREF; i++) /* ustawiamy znak "\n" jako pierwszy prfiks */
    {
        prefix[i] = NONWORD;
    }
    if (b == 0){
        fprintf( stderr, "\nProsze podac co najmniej jeden plik z tekstem zrodlowym!\n" );
        exit( EXIT_FAILURE );
    }
    for (i = 0; i < b; i++){
        in = fopen(nazwa[i], "r");
        if (in == NULL){
            fprintf( stderr, "Nie udalo sie otworzyc pliku %s, upewnij sie czy taki plik istnieje!\n", nazwa[i]);
            exit( EXIT_FAILURE );
        }
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
    if (ifstats)
        fclose(st);
    fclose(out);
    fclose(po);
    return 0;
}
