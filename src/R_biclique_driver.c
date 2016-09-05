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

FILE *fp;
char infn[100];
char *outfn;
int DEGREE;
char INPUT;

void maximal_biclique(char *fn, BiGraph *G)
{
    FILE *fp1=NULL;
    int *profile = (int *) malloc ((3*(G->_num_v1*G->_num_v2) + 9) * sizeof (int));

    int n2 = G->_num_v2;
    vid_t cand[n2];
    int i;

    if (PRINT) { 
        if (fn != NULL) { fp1 = fopen(fn, "w"); }
        else { fp1 = stdout; }
    }

    for (i = 0; i < n2; i++) cand[i] = i;
    biclique_enumerate(fp1, profile, G, cand, n2);

    if (fp1 != NULL) fclose(fp1);
    for(i = 0; i < profile[0]; i++)
        Rprintf("%d\n", profile[i]);

    Free(profile);
}

/**
 * Read input file and do the bicliuqe enumeration.
 * Two formats are supported: bel and bmat
 * If start and count is not specified, a whole list is returned.
 */
SEXP R_biclique(SEXP R_file)
{
    BiGraph *G;

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
    PRINT = 0;
    outfn = NULL;
    SORT_TYPE = 1;
    INPUT = 0;  // default = edge list = 0

    if (INPUT==0) G = bigraph_edgelist_in(fp);
    else if (INPUT==1) G = bigraph_binarymatrix_in(fp);
    fclose(fp);

    if (DEGREE) {
        bigraph_degreelist_out(stdout, G);
    }
    else maximal_biclique(outfn, G);

    bigraph_free(G);

    return R_NilValue;
}
