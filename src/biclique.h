/* Enumerate biclique in bipartite graph
 * Author: Yun Zhang
 * Date: September 2006
 */

#ifndef __BICLIQUE_H
#define __BICLIQUE_H


/* ---------------------------------------- *
 * Biclique Enumeration Functions           *
 * ---------------------------------------- */
void biclique_enumerate(int **g_right, int **g_left, FILE *fp, int *profile, BiGraph *G, vid_t *, int, int *nnr, int *nnl);

#endif

