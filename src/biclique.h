/* Enumerate biclique in bipartite graphs
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#ifndef __BICLIQUE_H
#define __BICLIQUE_H

#include <R.h>
#include <Rinternals.h>

/* ---------------------------------------- *
 * Biclique Enumeration Functions           *
 * ---------------------------------------- */
void biclique_enumerate(int **g_right, int **g_left, int *profile, BiGraph *G, vid_t *, int);

#endif

