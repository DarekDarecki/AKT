#ifndef __MARKOV_H
#define __MARKOV_H
#define NHASH 4093 /*rozmiar tablicy haszujacej */
#define N 100 /* zakladamy, ze ngramy nie beda wieksze niz 100*/
extern const int NPREF;
extern const int MAXGEN;

typedef struct State State;
typedef struct Suffix Suffix;
struct State { /* lista par: prefiks + sufiks */
    char *pref[N]; /* wyrazy prefiksow */
    Suffix *suf; /* lista sufiksow */
    State *next; 
};
struct Suffix { /* lista sufiksow */
    char *word; 
    Suffix *next;
};
State *statetab[NHASH]; /* tablica haszujaca zawierajaca stany */

#endif
