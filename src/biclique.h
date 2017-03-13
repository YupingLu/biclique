/* Enumerate biclique in bipartite graph
 * Author: Yun Zhang
 * Date: September 2006
 * Last modified: February 2017
 */

#ifndef __BICLIQUE_H
#define __BICLIQUE_H

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

/* ---------------------------------------- *
 * Biclique Enumeration Functions           *
 * ---------------------------------------- */
void biclique_enumerate(int **g_right, int **g_left, int *profile, BiGraph *G, vid_t *, int);

#endif

