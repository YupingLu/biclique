/* API for Bit-Based Adjacency Matrix for undirected bipartite graphs
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#ifndef __BIGRAPH_H
#define __BIGRAPH_H

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

/** Data structure **/

/* data structure of a bipartite graph */
typedef struct bipartite_graph_t {
    unsigned int _num_v1;  /* number of vertices in V1 */
    unsigned int _num_v2;  /* number of vertices in V2 */
    unsigned int _num_edges;       /* number of active edges */
    unsigned int _num_bytes_v1;    /* number of bytes for V1 */
    unsigned int _num_bytes_v2;    /* number of bytes for V2 */
    char ** _label_v1;     /* labels of vertices in V1 */
    char ** _label_v2;     /* labels of vertices in V2 */
    unsigned int **_neighbor_v1;       /* bit-based adjacency matrix for V1 */
    unsigned int **_neighbor_v2;       /* bit-based adjacency matrix for V2 */
    unsigned short *_degree_v1;   /* number of edges each vertex have (<65536) */
    unsigned short *_degree_v2;   /* number of edges each vertex have (<65536) */
} BiGraph;

#define Strdup(X) strcpy(R_alloc(strlen(X)+1, sizeof(char)), X) 

/* Macros for BiGraph */

/* returns the total number of vertices in V1 of graph */
#define bigraph_num_v1(g)  (g->_num_v1)

/* returns the number of active vertices in V2 of graph */
#define bigraph_num_v2(g)  (g->_num_v2)

/* returns the number of active edges in graph */
#define bigraph_num_edges(g)  (g->_num_edges)

/* check to see if an edge exists or not */
#define bigraph_edge_exists(g, u, v)  (IS_SET(g->_neighbor_v2[v], u))

/* return the degree of a vertex in V1 */
#define bigraph_degree_v1(g, u)  (g->_degree_v1[u])

/* return the degree of a vertex in V1 */
#define bigraph_degree_v2(g, u)  (g->_degree_v2[u])

/* return the pointer to the bit-based neighbors of vertex u */
#define bigraph_neighbors(g, u)  (g->_neighbor_v2[u])

/* return the pointer to the bit-based neighbors of vertex u */
#define bigraph_neighbor_v1(g, u)  (g->_neighbor_v1[u])

/* add an edge to graph */
#define bigraph_add_edge(g, u, v)  { \
    if (!IS_SET(g->_neighbor_v2[v], u)) {\
        SET_BIT(g->_neighbor_v2[v], u); SET_BIT(g->_neighbor_v1[u], v);\
        g->_num_edges++; \
        g->_degree_v1[u]++; g->_degree_v2[v]++; }\
}   

/* delete an edge from graph */
#define bigraph_delete_edge(g, u, v) { \
    if (IS_SET(g->_neighbor_v2[v], u)) {\
        DEL_BIT(g->_neighbor_v2[v], u); DEL_BIT(g->_neighbor_v1[u], v); \
        g->_degree_v1[u]--; g->_degree_v2[v]--; \
        g->_num_edges--; } \
}


/** Function prototypes **/

/* Malloc a graph, initialize it and returns a pointer to it */
BiGraph *bigraph_make(unsigned int num_v1, unsigned int num_v2);

/* Free the memory of a graph */
//void bigraph_free(BiGraph *G);

/* Read in a graph from an unweighted edge-list format file */
BiGraph *bigraph_edgelist_in(FILE *fp);

/* Read in a graph from a binary matrix format file */
BiGraph *bigraph_binarymatrix_in(FILE *fp);

/* Write out the degree of each vertex in a graph to a file pointer */
void bigraph_degreelist_out(FILE *fp, BiGraph *G);

#endif  /* __BIGRAPH_H */

