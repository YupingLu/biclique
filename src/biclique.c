/* Enumerate biclique in bipartite graphs
 * Author: Yun Zhang
 * Created: September 2006
 * Last Update: August 2007
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
/*void biclique_out(FILE *fp, BiGraph *G, vid_t *right, \
                int nr, vid_t *left, int nl)
{
    int i;

    for (i = 0; i < nr-1; i++) {
        fprintf(fp, "%s\t", G->_label_v2[right[i]]);
    }
    fprintf(fp, "%s\n", G->_label_v2[right[i]]);
    for (i = 0; i < nl-1; i++) {
        fprintf(fp, "%s\t", G->_label_v1[left[i]]);
    }
    fprintf(fp, "%s\n", G->_label_v1[left[i]]);
    fprintf(fp, "\n");
}*/

void biclique_out(int **g_right, int **g_left, FILE *fp, BiGraph *G, vid_t *right, \
                int nr, vid_t *left, int nl, int *nnr, int *nnl)
{
    int i;

    printf("Test %d \n", nnr[0]);

    *g_right = (int *) malloc (nr * sizeof (int));
    for (i = 0; i < nr; i++) {
        (*g_right)[i] = right[i];
        printf("%d \n", (*g_right)[i]);
    }

    *g_left = (int *) malloc (nl * sizeof (int));
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
    for (i = 0; i < nc; i++) printf(" %s", G->_label_v2[clique[i]]);
    printf("\t|");
    for (i = 0; i < ne; i++) printf(" %s", G->_label_v2[right[i]]);
    printf("\t|");
    for (i = ne; i < ce; i++) printf(" %s", G->_label_v2[right[i]]);
    printf("\t|");
    for (i = 0; i < nl; i++) printf(" %s", G->_label_v1[left[i]]);
    printf("\n");
}

void searchtreenode_out2(BiGraph *G, vid_t *clique, vid_t *right, \
                vid_t *left, vid_t *old_right, vid_t w, int nc, int ne, \
                int ce, int nl, int old_ne) 
{
    int i;
    for (i = 0; i < nc; i++) printf(" %s", G->_label_v2[clique[i]]);
    printf("\t|");
    for (i = 0; i < ne; i++) printf(" %s", G->_label_v2[right[i]]);
    printf(" %s", G->_label_v2[w]); 
    printf("\t|");
    for (i = old_ne+1; i < ce; i++) printf(" %s", G->_label_v2[old_right[i]]);
    printf("\t|");
    for (i = 0; i < nl; i++) printf(" %s", G->_label_v1[left[i]]);
    printf("\n");
}



/* --------------------------------------------------- *
 *  Function: biclique_find_basic()
 *    The basic version of biclique_find().
 * --------------------------------------------------- */
void biclique_find_basic(int **g_right, int **g_left, FILE *fp, BiGraph *G, num_t *nclique, \
        vid_t *clique, int nc, vid_t *left, int nl, vid_t *right, int ne, int ce, int *nnr, int *nnl)
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
                biclique_out(&(g_right[nnr[0]]), &(g_left[nnl[0]]), fp, G, clique, new_nc, new_left, new_nl, nnr, nnl);

                printf("Test1 %d \n", nnr[0]);
                int i;
                for (i = 0; i < nnr[nnr[0]]; i++) {
                    printf("%d \n", g_right[nnr[0]-1][i]);
                }
            }
        }
        
        /* Recursively find bicliques */
        if ((new_ne < new_ce) && (new_nc+(new_ce-new_ne) >= RLEAST)) {
            biclique_find_basic(g_right, g_left, fp, G, nclique, clique, new_nc, \
                new_left, new_nl, new_right, new_ne, new_ce, nnr, nnl);
        }
        else {
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


/* --------------------------------------------------- *
 *  Biclique Enumerating Main Function 
 * --------------------------------------------------- */
void biclique_enumerate(int **g_right, int **g_left, FILE *fp1, int *profile, BiGraph *G, \
                vid_t *cand, int lcand, int *nnr, int *nnl)
{
    unsigned int n1 = G->_num_v1;
    unsigned int n2 = G->_num_v2;
    num_t *nclique;
    vid_t left[n1], right[n2], clique[n2];
    vid_t u, v;
    int noc[n2], tmpnoc, x;
    
    /* Initialization */
    nclique = (num_t *) calloc(n1*n2, sizeof(num_t));
    if (!nclique) { perror("malloc nclique\n"); exit(-1); }
    memset(clique, -1, n2*sizeof(vid_t));  // initially Clique is empty
    for (u = 0; u < n1; u++) left[u] = u;  // every left vertex is candidate 
    for (v = 0; v < n2; v++) right[v] = v; // every right vertex is candidate
 
    /* Call the recursive function to find maximal bicliques */
    if (VERSION == 1) {
        biclique_find_basic(g_right, g_left, fp1, G, nclique, clique, 0, left, n1, right, 0, n2, nnr, nnl);
    }

    printf("Test2 %d \n", nnr[0]);
    int i;
    for (i = 0; i < nnr[nnr[0]]; i++) {
        printf("%d \n", g_right[nnr[0]-1][i]);
    }
    
    /* Print out the profile of maximal bicliques */
    biclique_profile_out(profile, G, nclique);

    /* Free memory */
    free(nclique);

    return;
}

