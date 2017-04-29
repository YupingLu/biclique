/* Enumerate biclique in bipartite graphs
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "bigraph.h"
#include "biclique.h"
#include "sort.h"

#define LMIN 2

/* Global Variables */
int LLEAST, RLEAST;
int VERSION;
int PRINT;

int *nnr;
int *nnl;

typedef unsigned long num_t;

/* ------------------------------------------------------------- *
 * Function: biclique_profile_out()                              *
 *   Print out the profile, no. of left/right vertices, no. of   *
 *   edges, no.of biclique, size of vertex/edge maximum biclique *
 * ------------------------------------------------------------- */
void biclique_profile_out(int *profile, BiGraph *G, num_t *nclique)
{
    unsigned int n1 = G->_num_v1;
    unsigned int n2 = G->_num_v2;
    num_t num, sum=0;
    int ei=0, ej=0;
    int vi=0, vj=0;
    int i, j, length;

    length = 0;
    profile[length++] = 0;
 
    for (i = 1; i <= n2; i++) {
        for (j = 1; j <= n1; j++) {
            num = nclique[(i-1)*n1+(j-1)];
            if (num > 0) {
                profile[length++] = i;
                profile[length++] = j;
                profile[length++] = num;

                sum += num;
                if (i*j > ei*ej) { ei = i; ej = j; }
                if (i+j > vi+vj) { vi = i; vj = j; }
            }
        }
    }
    
    profile[length++] = n1;
    profile[length++] = n2;
    profile[length++] = G->_num_edges;
    profile[length++] = sum;
    profile[length++] = ei;
    profile[length++] = ej;
    profile[length++] = vi;
    profile[length++] = vj;
    profile[0] = length;
}


/* ------------------------------------------------------------- *
 * Function: biclique_out()                                      *
 *   Print out a biclique: left and right vertices on two lines  *
 * ------------------------------------------------------------- */
void biclique_out(int **g_right, int **g_left, BiGraph *G, vid_t *right, \
                int nr, vid_t *left, int nl)
{
    int i;

    //*g_right = (int *) R_alloc(nr, sizeof(int));
    *g_right = (int *) Calloc(nr, int);
    for (i = 0; i < nr; i++) {
        (*g_right)[i] = right[i];
    }

    //*g_left = (int *) R_alloc(nl, sizeof(int));
    *g_left = (int *) Calloc(nl, int);
     for (i = 0; i < nl; i++) {
        (*g_left)[i] = left[i];
    }

    nnr[0]++;
    nnr[nnr[0]] = nr;

    nnl[0]++;
    nnl[nnl[0]] = nl;
}

/* ------------------------------------------------------------- *
 * Function: searchtreenode_out()                                *
 *   Print out the elements in biclique, candidates, and former  *
 *   candidates, in both left and right.                         *
 * ------------------------------------------------------------- */
void searchtreenode_out(BiGraph *G, vid_t *clique, vid_t *right, \
                vid_t *left, int nc, int ne, int ce, int nl) 
{
    int i;
    for (i = 0; i < nc; i++) Rprintf(" %s", G->_label_v2[clique[i]]);
    Rprintf("\t|");
    for (i = 0; i < ne; i++) Rprintf(" %s", G->_label_v2[right[i]]);
    Rprintf("\t|");
    for (i = ne; i < ce; i++) Rprintf(" %s", G->_label_v2[right[i]]);
    Rprintf("\t|");
    for (i = 0; i < nl; i++) Rprintf(" %s", G->_label_v1[left[i]]);
    Rprintf("\n");
}

void searchtreenode_out2(BiGraph *G, vid_t *clique, vid_t *right, \
                vid_t *left, vid_t *old_right, vid_t w, int nc, int ne, \
                int ce, int nl, int old_ne) 
{
    int i;
    for (i = 0; i < nc; i++) Rprintf(" %s", G->_label_v2[clique[i]]);
    Rprintf("\t|");
    for (i = 0; i < ne; i++) Rprintf(" %s", G->_label_v2[right[i]]);
    Rprintf(" %s", G->_label_v2[w]); 
    Rprintf("\t|");
    for (i = old_ne+1; i < ce; i++) Rprintf(" %s", G->_label_v2[old_right[i]]);
    Rprintf("\t|");
    for (i = 0; i < nl; i++) Rprintf(" %s", G->_label_v1[left[i]]);
    Rprintf("\n");
}



/* --------------------------------------------------- *
 *  Function: biclique_find_basic()
 *    The basic version of biclique_find().
 * --------------------------------------------------- */
void biclique_find_basic(int **g_right, int **g_left, BiGraph *G, num_t *nclique, \
        vid_t *clique, int nc, vid_t *left, int nl, vid_t *right, int ne, int ce)
{
    unsigned int n1 = G->_num_v1;
    vid_t new_left[nl];
    vid_t new_right[ce];
    vid_t u, v, w, j, k;
    int new_nc, new_nl, new_ne, new_ce;
    int count, is_maximal=1;

    while (ne < ce) {

        /* Choose one vertex from candidate set */
        v = right[ne];

        /* Set right vertices in clique */
        new_nc = nc;
        clique[new_nc++] = v;
        
        /* Set neighbors on left */
        memset(new_left, -1, nl*sizeof(vid_t));
        new_nl = 0;
        for (j = 0; j < nl; j++) {
            u = left[j];
            if (bigraph_edge_exists(G, u, v)) new_left[new_nl++] = u;
        }

        /* Set right vertices in not */
        memset(new_right, -1, ce*sizeof(vid_t));
        new_ne = 0;
        is_maximal = 1;
        for (j = 0; j < ne; j++) {
            w = right[j];
            count = 0;
            for (k = 0; k < new_nl; k++) {
                u = new_left[k];
                if (bigraph_edge_exists(G, u, w)) count++;
            }
            if (count == new_nl) { is_maximal = 0; break; }
            else if (count > 0) new_right[new_ne++] = w;
        }

        /* Stop this branch if it is not maximal */
        if (!is_maximal) { 
            ne++; continue;
        }

        /* Set right vertices in cand */
        new_ce = new_ne;
        for (j = ne+1; j < ce; j++) {
            w = right[j];
            count = 0;
            for (k = 0; k < new_nl; k++) {
                u = new_left[k];
                if (bigraph_edge_exists(G, u, w)) count++;
            }
            if (count == new_nl) { 
                clique[new_nc++] = w; 
            }
            else if (count > 0)  { 
                new_right[new_ce++] = w;
            }
        }

        /* Print out the found maximal biclique */
        if (new_nc >= RLEAST && new_nl >= LLEAST) {
            nclique[(new_nc-1)*n1+(new_nl-1)]++;
            if (PRINT) {
                biclique_out(&(g_right[nnr[0]]), &(g_left[nnl[0]]), G, clique, new_nc, new_left, new_nl);
            }
        }
        
        /* Recursively find bicliques */
        if ((new_ne < new_ce) && (new_nc+(new_ce-new_ne) >= RLEAST)) {
            biclique_find_basic(g_right, g_left, G, nclique, clique, new_nc, \
                new_left, new_nl, new_right, new_ne, new_ce);
        }

        /* Move v to former candidate set */
        ne++;

    }
    
    return;
}


/* --------------------------------------------------- *
 *  Function: biclique_find_improve()
 *    The Improved Version of biclique_find().
 * --------------------------------------------------- */
void biclique_find_improve(int **g_right, int **g_left, BiGraph *G, num_t *nclique, \
        vid_t *clique, int nc, vid_t *left, int nl, vid_t *right, int ne, int ce)
{
    unsigned int n1 = G->_num_v1;
    vid_t new_left[nl];
    vid_t new_right[ce];
    vid_t u, v, w, j, k;
    int new_nc, new_nl, new_ne, new_ce;
    int count, is_maximal=1;
    int x, noc[ce-ne];

    // Improvement II
    // divide new_left to two parts: in L' and not in L'
    // ----------------------------
    // |  L'          |   not L'  |
    // ----------------------------
    // ^              ^           ^
    // new_nl ->          <- not_nl  
    // L'=new_left[0..new_nl-1], ~L'=new_left[new_nl,nl-1]
    int not_nl;  // position of not_left
    int nn;      // number of vertices directly go to not
    // End
 
    /* Same operations as v2 on each candidate in order */
    while (ne < ce) {
    
        /* Choose the next candidate in P */
        v = right[ne];

        /* Choose one vertex from candidate set */
        new_nc = nc;
        clique[new_nc++] = v;
        
        /* Set right vertices in clique */
        memset(new_left, -1, nl*sizeof(vid_t));
        new_nl = 0; not_nl = nl;
        for (j = 0; j < nl; j++) {
            u = left[j];
            if (bigraph_edge_exists(G, u, v))
                new_left[new_nl++] = u;
            else new_left[not_nl--] = u;
        }

        /* Set right vertices in not */
        memset(new_right, -1, ce*sizeof(vid_t));
        new_ne = 0;
        is_maximal = 1;
        for (j = 0; j < ne; j++) {
            w = right[j];
            count = 0;
            for (k = 0; k < new_nl; k++) {
                u = new_left[k];
                if (bigraph_edge_exists(G, u, w)) count++;
            }
            if (count == new_nl) { is_maximal = 0; break; }
            else if (count > 0) new_right[new_ne++] = w;
        }

        /* Stop this branch if it is not maximal */
        if (!is_maximal) { 
            ne++; continue;
        }

        /* Set right vertices in cand */
        memset(noc, 0, (ce-ne)*sizeof(int));
        new_ce = new_ne;
        nn = 1; // number of vertice will be put in not when backtracking
        for (j = ne+1; j < ce; j++) {
            w = right[j];
            /* count the connections to L */
            count = 0;
            for (k = 0; k < new_nl; k++) {
                u = new_left[k];
                if (bigraph_edge_exists(G, u, w)) count++;
            }
            if (count == new_nl) {
                clique[new_nc++] = w;
                // Improvement II
                for (k = nl; k > not_nl; k--) {
                        u = new_left[k];
                        if (bigraph_edge_exists(G,u,w)) count++;
                }
                // switch such vertex with the one next to 
                // the last picked vertex to biclique
                if (count == new_nl) {
                        right[j] = right[ne+nn]; 
                        right[ne+nn] = w;
                        nn++;
                }
            }
            else if (count > 0)  { 
                /* Improvement I: Sorting candidates in new_right */
                x = new_ce-1;
                while (x >= new_ne && noc[x-new_ne] > count) {
                    noc[x+1-new_ne] = noc[x-new_ne];
                    new_right[x+1] = new_right[x];
                    x--;
                }
                noc[x+1-new_ne] = count;
                new_right[x+1] = w;
                new_ce++;
            }
        }

        /* Print out the found maximal biclique */
        if (new_nc >= RLEAST && new_nl >= LLEAST) {
            nclique[(new_nc-1)*n1+(new_nl-1)]++;
            if (PRINT) biclique_out(&(g_right[nnr[0]]), &(g_left[nnl[0]]), G, clique, new_nc, new_left, new_nl);
        }
        
        /* Recursively find bicliques */
        if ((new_ne < new_ce) && (new_nc+(new_ce-new_ne) >= RLEAST)) {
            biclique_find_basic(g_right, g_left, G, nclique, clique, new_nc, \
                new_left, new_nl, new_right, new_ne, new_ce);
        }
        
        /* Move v and other qualified vertics to former candidate set */
        ne += nn;

    }
    
    return;
}


/* --------------------------------------------------- *
 *  Biclique Enumerating Main Function 
 * --------------------------------------------------- */
void biclique_enumerate(int **g_right, int **g_left, int *profile, BiGraph *G, \
                vid_t *cand, int lcand)
{
    unsigned int n1 = G->_num_v1;
    unsigned int n2 = G->_num_v2;
    num_t *nclique;
    vid_t left[n1], right[n2], clique[n2];
    vid_t u, v;
    int noc[n2], tmpnoc, x;
    
    /* Initialization */
    nclique = (num_t *) Calloc(n1*n2, num_t);
    if (!nclique) { 
        error("malloc nclique");
    }
    memset(clique, -1, n2*sizeof(vid_t));  // initially Clique is empty
    for (u = 0; u < n1; u++) left[u] = u;  // every left vertex is candidate 
    for (v = 0; v < n2; v++) right[v] = v; // every right vertex is candidate
 
    /* Call the recursive function to find maximal bicliques */
    if (VERSION == 1) {
        biclique_find_basic(g_right, g_left, G, nclique, clique, 0, left, n1, right, 0, n2);
    }else if (VERSION == 2) {
        /* Sort the candidate right vertices */
        memset(noc, 0, n2*sizeof(int));
        for (v = 0; v < n2; v++) {
            tmpnoc = 0;
            for (u = 0; u < n1; u++)
                if (bigraph_edge_exists(G, u, v)) tmpnoc++;
            /* Sorting candidates in new_right */
            x = v - 1;
            while (x >= 0 && noc[x] > tmpnoc) {
                    noc[x+1] = noc[x];
                    right[x+1] = right[x];
                    x--;
            }
            noc[x+1] = tmpnoc;
            right[x+1] = v;
        }       
        
        biclique_find_improve(g_right, g_left, G, nclique, clique, 0, left, n1, right, 0, n2);
    }
    
    /* Print out the profile of maximal bicliques */
    biclique_profile_out(profile, G, nclique);

    /* Free memory */
    Free(nclique);

    return;
}

