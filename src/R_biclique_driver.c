/* 
 * R wrapper of Biclique Enumerator
 *
 * Author: Yuping Lu, yupinglu@utk.edu
 * Created: September 2016
 *
 * Copyright 2016-2017
 * Department of Computer Science, University of Tennessee, Knoxville
 *
 * Last modified: September 2016
 *
 */

#include "R_biclique_driver.h"

extern int LLEAST, RLEAST;
extern int CACHE_SIZE;
extern int VERSION;
extern int PRINT;
extern long long node_num;
extern int SORT_TYPE;

// Global variables to store the bicliques
extern int *nnr;
extern int *nnl;
extern int **g_right;
extern int **g_left;

FILE *fp;
char infn[100];
char *outfn;
int DEGREE;
char INPUT;

/** 
 *  Finalizer that only clears R pointer
 */
static void finalizer0(SEXP Rptr)
{
    void *ptr = (void *) R_ExternalPtrAddr(Rptr);
    if (NULL == ptr) {
        return;
    } else {
        R_ClearExternalPtr(Rptr);
    }
}

void maximal_biclique(char *fn, BiGraph *G, int *profile)
{
    FILE *fp1=NULL;
    // number of nodes in each...
    nnr = (int *) calloc ((G->_num_v2 + 1), sizeof (int));
    nnl = (int *) calloc ((G->_num_v2 + 1), sizeof (int));

    g_right = (int **) calloc (G->_num_v2 * sizeof (int*));
    g_left = (int **) calloc (G->_num_v2 * sizeof (int*));
    
    int n2 = G->_num_v2;
    vid_t cand[n2];
    int i;

    for (i = 0; i < n2; i++) cand[i] = i;
    biclique_enumerate(fp1, profile, G, cand, n2);

    // print the bicliques
    int j;
    for(j = 0; j < nnr[0]; j++) {
        for (i = 0; i < nnr[j+1]-1; i++) {
            Rprintf("%s\t", G->_label_v2[g_right[j][i]]);
        }
        Rprintf("%s\n", G->_label_v2[g_right[j][nnr[j+1]]]);
        for (i = 0; i < nnl[j+1]-1; i++) {
            Rprintf("%s\t", G->_label_v1[g_left[j][i]]);
        }
        Rprintf("%s\n", G->_label_v1[g_left[j][nnl[j+1]]]);
        
        Rprintf("\n");
    }
    
    // free memory
    for (i = 0; i < nnr[0]; i++) {
        Free(g_right[i]);
    }
    for (i = 0; i < nnl[0]; i++) {
        Free(g_left[i]);
    }
    Free(g_right);
    Free(g_left);
    Free(nnr);
    Free(nnl);
    
}

/**
 * Read input file and do the bicliuqe enumeration.
 * Two formats are supported: bel and bmat
 * If start and count is not specified, a whole list is returned.
 */
SEXP R_biclique(SEXP R_file)
{
    BiGraph *G;
    SEXP R_data;

    const char *filepath = CHARPT(R_file, 0);
    strcpy(infn, filepath);
    if ((fp = fopen(infn, "r")) == NULL) {
        REprintf("Can't open file %s\n", infn);
        return R_NilValue;
    }

    LLEAST = 1;
    RLEAST = 1;
    DEGREE = 0;
    VERSION = 1;
    PRINT = 1;
    outfn = NULL;
    SORT_TYPE = 1;
    INPUT = 0;  // default = edge list = 0

    if (INPUT==0) G = bigraph_edgelist_in(fp);
    else if (INPUT==1) G = bigraph_binarymatrix_in(fp);
    fclose(fp);

    if (DEGREE) {
        bigraph_degreelist_out(stdout, G);
    }
    else {
        SEXP profile_data;
        int *profile = (int *) malloc ((3*(G->_num_v1*G->_num_v2) + 9) * sizeof (int));
        maximal_biclique(outfn, G, profile);

        newRptr(profile, profile_data, finalizer0);
        R_data = copy_data (profile_data);

        UNPROTECT(1);
        Free(profile);
    }

    bigraph_free(G);

    return R_data;
}


/** 
 * Copy data from C memory to R memory, return is R_data
 */
SEXP copy_data (SEXP C_data)
{
    SEXP R_data;  
    int *data = R_ExternalPtrAddr(C_data);
    int nelems = data[0];  /* get the number of elements to copy */
    int pos;

    R_data = PROTECT(allocVector(INTSXP, nelems));
    for(pos = 0; pos < nelems; pos++)
        INTEGER(R_data)[pos] = data[pos];

    UNPROTECT(1);
    return R_data;
}