/* Enumerate biclique in bipartite graphs
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: August 2017
 */

#ifndef __BICLIQUE_H
#define __BICLIQUE_H

#include <R.h>
#include <Rinternals.h>

typedef unsigned int num_t;

/* ---------------------------------------- *
 * Biclique Enumeration Functions           *
 * ---------------------------------------- */
void biclique_enumerate(num_t **g_right, num_t **g_left, num_t *profile, BiGraph *G, vid_t *, int);

#endif

