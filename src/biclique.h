/* Enumerate biclique in bipartite graph
 * Author: Yun Zhang
 * Date: September 2006
 */

#ifndef __BICLIQUE_H
#define __BICLIQUE_H


/* ---------------------------------------- *
 * Biclique Enumeration Functions           *
 * ---------------------------------------- */
void biclique_enumerate(FILE *fp, FILE *fp2, BiGraph *G, vid_t *, int);

#endif

