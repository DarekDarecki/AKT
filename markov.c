#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "markov.h"

char NONWORD[] = "\n"; /* symbol, ktory na pewno nie jest slowem*/
int tabh[NHASH]; /*tablica przechowujaca wartosci "haszu"*/
int xx = 0; /*licznik*/

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
/*elimination: eliminuje powtarzajace sie elementy w tablicy*/
int elimination(int *array, int size){
int i,j,k;
int *p = array;
  for(i=0;i<size;i++){
    for(j=0;j<size;j++){
         if(i==j){
             continue;
         }
         else if(*(p+i)==*(p+j)){
             k=j;
             size--;
             while(k < size){
                 *(p+k)=*(p+k+1);
                 k++;
              }
              j=0;
          }
      }
  }
return size;
}

int find(int *numbers, int n)
{
    int qMax = 0;
    int i,j,temp, compare, max;
    for (i=0;i<n;i++)
    {
        temp = 0;
        compare = numbers[i];
        for (j=0;j<n;j++)
        {
            if (numbers[j] == compare)
            {
                temp++;
            }
        }
        if (temp > qMax)
        {
            max = compare;
            qMax = temp;
        }
    }
    return max;
}
/*funkcja generujaca plik posredni z prefiksami i ich sufiksami*/
void ngrams(FILE *out){
    int i,h,k,j,n,x;
    State *sp;
    Suffix *suf;
    char *prefix[NPREF],*w;
    for (i = 0; i < NPREF; i++) 
    {
        prefix[i] = NONWORD;
    }
    for (i = 0; i < xx; i++)
        printf("%d ", tabh[i]); 
    x = find(tabh, xx);
    printf("\n--%d--\n",x); 
   
    for (sp = statetab[x]; sp != NULL; sp = sp->next){
        printf("\"");
        for (j = 0; j < NPREF; j++)
            printf("%s ", sp->pref[j]);
        printf("\":\n");
        for (suf = sp->suf; suf != NULL; suf = suf->next)
        {
                w = suf->word;
                printf("\t%s\n", w);
        }  
    }


 
    n = elimination(tabh, xx); /*eliminujemy powtarzajace sie wartosci*/
    printf("\n\n");
    for (i = 0; i < n; i++)
        printf("%d ", tabh[i]); 
    for (i = NPREF; i < n; i++){ 
    k = tabh[i];
    for (sp = statetab[k]; sp != NULL; sp = sp->next){
        fprintf(out,"\"");
        for (j = 0; j < NPREF; j++)
            fprintf(out, "%s ", sp->pref[j]);
        fprintf(out, "\":\n");
        for (suf = sp->suf; suf != NULL; suf = suf->next)
        {
                w = suf->word;
                fprintf(out, "\t%s\n", w);
        }  
    }
    }
}

