/* Driver file for Biclique Enumerator
 *
 * Author: Yun Zhang, yzhang@cs.utk.edu
 * Created: April 2006
 *
 * Copyright 2005-2006
 * Department of Computer Science, University of Tennessee, Knoxville
 * Computer Science and Mathematics Division, Oak Ridge National Laboratory
 *
 * Last modified: September 2008
 *
 * Changes: This version has added the choice to take binary matrix 
 * as input file.
 */

#include <string.h>
#include "bit.h"
#include "bigraph.h"
#include "utility.h"
#include "biclique.h"


extern int LLEAST, RLEAST;
extern int CACHE_SIZE;
extern int VERSION;
extern int PRINT;
extern long long node_num;
extern double time_check, time_expand, time_out, time_sort;
extern int SORT_TYPE;
double time_in, stime;
FILE *fp;
char infn[100];
char *outfn;
int DEGREE;
char INPUT;

void print_options(void)
{
  fprintf(stderr, "\n Options: \n");
  fprintf(stderr, "  -i [e|m] input file format <default = e>\n");
  fprintf(stderr, "       e - edge list\n");
  fprintf(stderr, "       m - binary matrix\n");
  fprintf(stderr, "  -m [c|d] output type <default = c>\n");
  fprintf(stderr, "       c - biclique list\n");
  fprintf(stderr, "       d - degree list\n");
  fprintf(stderr, "  -v [1|2] algorithm version <default = 1>\n");
  fprintf(stderr, "  -p       print out bicliques <default = no print out>\n");
  fprintf(stderr, "  -o <filename>  \n");
  fprintf(stderr, "           output filename to store bicliques if choose to print\n");
  fprintf(stderr, "                     <default = STDOUT>\n");
  fprintf(stderr, "  -l <value>         least number of genes <default = 1>\n");
  fprintf(stderr, "  -r <value>         least number of phenotypes <default = 1>\n");
  fprintf(stderr, "\n");
}

void argument_parse(int argc, char **argv)
{
  int i;
  
  if (argc < 2) {
    fprintf(stderr, "Usage: %s Graph <options>\n", argv[0]);
    print_options();
	exit(1);
  }
  
  LLEAST = 1;
  RLEAST = 1;
  DEGREE = 0;
  VERSION = 1;
  PRINT = 0;
  outfn = NULL;
  SORT_TYPE = 1;
  INPUT = 0;  // default = edge list = 0
  
  for (i = 2; i < argc; i++) {
	if (!strcmp(argv[i], "-i")) {
	  i++;
	  if (!strcmp(argv[i], "e")) INPUT = 0;
	  else if (!strcmp(argv[i], "m")) INPUT = 1;
	}
	if (!strcmp(argv[i], "-o")) {
	  outfn = strdup(argv[++i]);
	}
	if (!strcmp(argv[i], "-l")) {
	  LLEAST = atoi(argv[++i]);
	}
	if (!strcmp(argv[i], "-r")) {
	  RLEAST = atoi(argv[++i]);
	}
	if (!strcmp(argv[i], "-v")) {
	  VERSION = atoi(argv[++i]);
	}
	if (!strcmp(argv[i], "-p")) {
	  PRINT = 1;
	}
	if (!strcmp(argv[i], "-s")) {
	  SORT_TYPE = atoi(argv[++i]);
	}
	if (!strcmp(argv[i], "-m")) {
	  i++;
	  if (!strcmp(argv[i], "c")) DEGREE = 0;
	  else if (!strcmp(argv[i], "d")) DEGREE = 1;
	}
  }

  strcpy(infn, argv[1]);
  if ((fp = fopen(infn, "r")) == NULL) {
    fprintf(stderr, "Can't open file %s\n", infn);
    exit(-1);
  }

  return;
}


void maximal_biclique(char *fn, BiGraph *G)
{
  FILE *fp1=NULL, *fp2;
  char fname[100];
  double utime;
  int n2 = G->_num_v2;
  vid_t cand[n2];
  int i;

  if (PRINT) { 
	if (fn != NULL) { fp1 = fopen64(fn, "w"); }
    else { fp1 = stdout; }
  }
  sprintf(fname, "%s.profile", infn);
  fp2 = fopen(fname, "w");

#ifdef PERFORMANCE
  node_num = 0;
  time_check = 0.0;
  time_expand = 0.0;
  time_out = 0.0;
  time_sort = 0.0;
#endif

  for (i = 0; i < n2; i++) cand[i] = i;
  biclique_enumerate(fp1, fp2, G, cand, n2);
  utime = get_cur_time() - stime;

#ifdef VERBOSE 
  printf("find biclique %f seconds\n", utime);
#endif

#ifdef PERFORMANCE
  fprintf(fp2, "\n");
  fprintf(fp2, "%lld tree nodes\n", node_num);
  fprintf(fp2, "%f readin\n", time_in);
  fprintf(fp2, "%f expand\n", time_expand);
  fprintf(fp2, "%f check\n", time_check);
  fprintf(fp2, "%f output\n", time_out);
  fprintf(fp2, "%f sorting\n", time_sort);
#endif
  fprintf(fp2, "%f total time\n", utime);

  if (fp1 != NULL) fclose(fp1);
  fclose(fp2);

}


int main(int argc, char  **argv)
{
  BiGraph *G;

  argument_parse(argc, argv);
  
  stime = get_cur_time();
  if (INPUT==0) G = bigraph_edgelist_in(fp);
  else if (INPUT==1) G = bigraph_binarymatrix_in(fp);
  fclose(fp);

  time_in = get_cur_time() - stime;

  if (DEGREE) {
	bigraph_degreelist_out(stdout, G);
  }
  else maximal_biclique(outfn, G);

  bigraph_free(G);

  exit(0);
}

