#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "markov.h"
#define N 100
char NONWORD[] = "\n"; /* symbol, ktory na pewno nie jest slowem*/
int tabh[NHASH];
int xx = 0;
/* funkcja haszujaca - oblicza indeks tablicy stanow */
unsigned int hash(char *s[])
{
    unsigned int h;
    unsigned char *p;
    int i;
    h = 0;
    for (i = 0; i < NPREF; i++) {
        for (p = (unsigned char *) s[i]; *p != '\0'; p++) {
            h = 31 * h + *p;
        }
    }
    tabh[xx] = h % NHASH;
    xx++;
    return h % NHASH;
}
/* lookup: wyszukuje prefiks, jesli potrzeba tworzy go  */
State* lookup(char *prefix[], int create)
{
    int i, h;
    State *sp;
    h = hash(prefix);
    for (sp = statetab[h]; sp != NULL; sp = sp->next) {
        for (i = 0; i < NPREF; i++)
        {
            if (strcmp(prefix[i], sp->pref[i]) != 0)
            {
                break;
            }
        }
        if ( i == NPREF )
        {
            return sp;
        }
    }
    if (create) {
        sp = (State *) malloc(sizeof(State));
        if (sp == NULL)
        {
            exit(2);
        }
        for (i = 0; i < NPREF; i++)
        {
            sp->pref[i] = prefix[i];
        }
        sp->suf = NULL;
        sp->next = statetab[h];
        statetab[h] = sp;
    }
    return sp;
}
/* add_suffix: dodaje sufiks do stanu */
void add_suffix(State *sp, char *suffix)
{
    Suffix *suf;
    suf = (Suffix *) malloc(sizeof(Suffix));
    if (suf == NULL)
    {
        exit(2);
    }
    suf->word = suffix;
    suf->next = sp->suf;
    sp->suf = suf;
}
/* add: dodaje slowo do listy sufiksow, aktualizuje stan przesuwajac prefiks */
void add(char *prefix[NPREF], char *suffix)
{
    State *sp;
    sp = lookup(prefix, 1); /* jesli nie ma takiego prefiksu tworzy go */
    add_suffix(sp, suffix);
    /* przesuwa slowa w prefiksie (zastepuje ostatnie slowo prefiksu sufiksem) */
    memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
    prefix[NPREF-1] = suffix;
}
/* build: wczytuje plik, tworzy tablice prefiksow */
void build(char *prefix[NPREF], FILE *f)
{
    char buf[100]; /* zakladamy ze slowo bedzie krotsze niz 100 znakow */
    while (fscanf(f, "%s", buf) == 1) {
        add(prefix, strdup(buf));
    }
}
/* generate: generuje tekst wyjsciowy */
void generate(int nwords)
{
    srand(time(NULL));
    State *sp;
    Suffix *suf;
    char *prefix[NPREF], *w;
    int i, nmatch;
    for (i = 0; i < NPREF; i++) /* ustawiamy pierwsze slowo jako symbol "\n" */
    {
        prefix[i] = NONWORD;
    }
    for (i = 0; i< nwords; i++)
    {
        sp = lookup(prefix, 0);
        nmatch = 0;
        for (suf = sp->suf; suf != NULL; suf = suf->next)
        {
            if (rand() % ++nmatch == 0) /* prob = 1/nmatch */
            {
                w = suf->word;
            }
        }
        if (strcmp(w, NONWORD) == 0) break;
        printf("%s ", w);
        memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
        prefix[NPREF-1] = w;
    }
}

void rmdup(int *array, int length)
{
    int *current , *end = array + length - 1;

    for ( current = array + 1; array < end; array++, current = array + 1 )
    {
        while ( current <= end )
        {
            if ( *current == *array )
            {
                *current = *end--;
            }
            else
            {
                current++;
            }
        }
    }
}

void drukuj(){
    int i,h,k;
    State *sp;
    Suffix *suf;
    char *prefix[NPREF],*w;
    for (i = 0; i < NPREF; i++) 
    {
        prefix[i] = NONWORD;
    }
    rmdup(tabh, xx);
    for (i = 0; i < xx; i++){ 
    k=tabh[i];
    for (sp = statetab[k]; sp != NULL; sp = sp->next){
        printf("\"%s ", sp->pref[0]);
        printf("%s\":\n", sp->pref[1]);
        for (suf = sp->suf; suf != NULL; suf = suf->next)
        {
                w = suf->word;
                printf("\t%s\n", w);
        }  
    }
    }
}
/* 

void drukuj(int nwords){
    State *sp;
    Suffix *suf;
    char *prefix[NPREF],*w;
    int i,j = 0,h, k = 0,tab[NHASH];
    
    for (i = 0; i < xx; i++)
        printf("%d  ", tabh[i]); 

    for (i = 0; i < NPREF; i++) 
    {
        prefix[i] = NONWORD;
    }
    for (i = 0; i< nwords; i++){
        h = hash(prefix);
        tab[k] = h;
        for (j = 0; j < k; j++){
            if(tab[j] == h){
                memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
                prefix[NPREF-1] = w;
            }
        }
        sp = lookup(prefix, 0);
        k++;
        printf("%d\n", h);
        printf("%s ", sp->pref[0]);
        printf("%s:\n", sp->pref[1]);
        for (suf = sp->suf; suf != NULL; suf = suf->next)
        {
                w = suf->word;
                printf("%s\n", w);
        }
        if (strcmp(w, NONWORD) == 0) break;
        //printf("%s: ", w);
        memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
        prefix[NPREF-1] = w;
    } 
}
*/
