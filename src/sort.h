/* Header file of Sorting algorithms for Biclique
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#ifndef __SORT_H
#define __SORT_H

#include "utility.h"
#include "bigraph.h"

enum {
    BUBBLE_SORT_INC = 1,
    BUBBLE_SORT_DEC,
    COMB_SORT,
    INSERTION_SORT_OFFLINE,
    INSERTION_SORT_ONLINE,
    SELECTION_SORT,
    SHELL_SORT
};


void swap_int(int *array, int i, int j);

void swap_vid(vid_t *array, int i, int j);

void bubble_sort(vid_t *array, int start, int end, int *noc);


void comb_sort(vid_t *array, int start, int end, int *noc);


void insertion_sort_offline(vid_t *array, int start, int end, int *noc);

void insertion_sort_online(vid_t *right, int start, int end, vid_t *left, int nl, BiGraph *G);


void selection_sort(vid_t *array, int start, int end, int *noc);

void shell_sort(vid_t *array, int start, int end, int *noc);

void candidate_sort(vid_t *right, int start, int end, vid_t *left, int nl, BiGraph *G);


#endif
