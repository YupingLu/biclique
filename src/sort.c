/* Sorting algorithms for Biclique
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "bigraph.h"
#include "sort.h"

// global variable
int SORT_TYPE;

inline void swap_int(int *array, int i, int j)
{
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

inline void swap_vid(vid_t *array, int i, int j)
{
    vid_t tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void bubble_sort_inc(vid_t *array, int start, int end, int *noc)
{
    int i, j;
    for (i = 0; i < end-start; i++) {
    for (j = i+1; j < end-start; j++) {
        if (noc[j] < noc[i]) {
        swap_int(noc, i, j);
        swap_vid(array, i+start, j+start);
        }
    }
    }
}


void bubble_sort_dec(vid_t *array, int start, int end, int *noc)
{
    int i, j;
    for (i = 0; i < end-start; i++) {
    for (j = i+1; j < end-start; j++) {
        if (noc[j] > noc[i]) {
        swap_int(noc, i, j);
        swap_vid(array, i+start, j+start);
        }
    }
    }
}

void comb_sort(vid_t *array, int start, int end, int *noc)
{
    int gap, swaps, i;
    swaps = 0; 
    gap = end-start;  // initialize gap size
    while (gap > 1 || swaps != 0) {
    if (gap > 1) {
        gap = gap / 1.3;
        if (gap == 10 || gap == 9) gap = 11;
    }
        i = 0;
    swaps = 0;  // bubblesort
    // a single "comb" over the input list
    while (i + gap < end-start) {
        if (noc[i] > noc[i+gap]) {
        swap_int(noc, i, i+gap);
        swap_vid(array, i+start, i+start+gap);
            swaps++;
        }
        i++;
    }
    }
}


void insertion_sort_offline(vid_t *array, int start, int end, int *noc)
{
    int i, j, tmp, tmpnoc;
    for (i = 1; i < end-start; i++) {
    tmpnoc = noc[i]; tmp = array[i];
    j = i - 1;
    while (j >= 0 && noc[j] > tmpnoc) {
        noc[j+1] = noc[j]; 
        array[j+1] = array[j];
        j--;
    }
    noc[j+1] = tmpnoc; array[j+1] = tmp;
    }
}


void insertion_sort_online(vid_t *right, int start, int end, \
        vid_t *left, int nl, BiGraph *G)
{
    int noc[end-start];
    int tmpnoc, i, j;
    vid_t right_sort[end-start];
    vid_t u, v;
    memset(noc, 0, (end-start)*sizeof(int));
    memset(right_sort, -1, (end-start)*sizeof(vid_t));
    for (i = 0; i < end-start; i++) {
    v = right[i+start];
    // count the connections to L
    tmpnoc = 0;
    for (j = 0; j < nl; j++) {
        u = left[j];
        if (bigraph_edge_exists(G, u, v)) tmpnoc++;
    }
    j = i - 1;
    while (j >= 0 && noc[j] > tmpnoc) {
        noc[j+1] = noc[j];
        right_sort[j+1] = right_sort[j];
        j--;
    }
    noc[j+1] = tmpnoc;
    right_sort[j+1] = v;
    }
    memcpy(right+start, right_sort, (end-start)*sizeof(vid_t));
}


void selection_sort(vid_t *array, int start, int end, int *noc)
{
    int i, j, min;
    for (i = 0; i < end-start-1; i++) {
    min = i;
    for (j = i+1; j < end-start; j++) {
        if (noc[j] < noc[min]) min = j;
    }
        swap_int(noc, i, min);
    swap_vid(array, i+start, min+start);
    }
}

void shell_sort(vid_t *array, int start, int end, int *noc)
{
    int i, j, inc, tmp;
    inc = (end-start) / 2;
    while (inc > 0) {
    for (i = inc; i < end-start; i++) {
        j = i;
        tmp = noc[i];
        while (j >= inc && noc[j-inc] > tmp) {
        noc[j] = noc[j-inc];
        j = j - inc;
        }
        noc[j] = tmp;
        swap_vid(array, i+start, j+start);
    }
    if (inc == 2) inc = 1;
    else inc = (int) (inc/2.2);
    }
}


void candidate_sort(vid_t *right, int start, int end, \
        vid_t *left, int nl, BiGraph *G)
{
    int noc[end-start];  // number of connections for each candidate in P
    vid_t u, v;
    int i, j;

    if (SORT_TYPE == INSERTION_SORT_ONLINE) {
    insertion_sort_online(right, start, end, left, nl, G);
        return;
    }
    
    // For each candidate, compute its connections to L
    memset(noc, 0, (end-start)*sizeof(int));
    for (i = 0; i < end-start; i++) {
    v = right[i+start];
    // count the connections to L
    for (j = 0; j < nl; j++) {
        u = left[j];
        if (bigraph_edge_exists(G, u, v)) noc[i]++;
    }
    }
    
    // Sort the candidates by their connections in increasing order
    switch (SORT_TYPE) {
    case BUBBLE_SORT_INC : bubble_sort_inc(right, start, end, noc); break;
    case BUBBLE_SORT_DEC : bubble_sort_dec(right, start, end, noc); break;
    case COMB_SORT : comb_sort(right, start, end, noc); break;
        case INSERTION_SORT_OFFLINE : insertion_sort_offline(right, start, end, noc); break;
        case SELECTION_SORT : selection_sort(right, start, end, noc); break;
        case SHELL_SORT : shell_sort(right, start, end, noc); break;
    default : break;
    }
    
}

