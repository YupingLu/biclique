/* API for Bit-Based Adjacency Matrix for undirected graphs 
 * Implement functions defined in graph.h 
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#include <string.h>
#include "bigraph.h"

#define LINE_LENGTH 1000

// trie node
struct TrieNode {
    struct TrieNode *children[256];
    int isLeaf;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void) {
    struct TrieNode *pNode = (struct TrieNode *)R_alloc(1, sizeof(struct TrieNode));

    if (pNode) {
        int i;
        pNode->isLeaf = -1;
        for (i = 0; i < 256; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

void insert(struct TrieNode *root, const char *key, int id) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = key[level];
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isLeaf = id;
}

// Returns id if key presents in trie
int search(struct TrieNode *root, const char *key) {
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++) {
        index = key[level];

        if (!pCrawl->children[index])
            return -1;

        pCrawl = pCrawl->children[index];
    }

    if(pCrawl == NULL) return -1;
    else return pCrawl->isLeaf;
}

/* Malloc and initialize a graph, returns a pointer to it */
BiGraph *bigraph_make(unsigned int num_v1, unsigned int num_v2)
{
    BiGraph *G;
    int i;
    int num_ints_v1 = bit_num_ints(num_v2);
    int num_ints_v2 = bit_num_ints(num_v1);
    
    G = (BiGraph *) R_alloc(1, sizeof(BiGraph));
    if (G == NULL) { 
        error("malloc");
    }
    G->_num_v1 = num_v1;
    G->_num_v2 = num_v2;
    G->_num_edges = 0;
    G->_num_bytes_v1 = num_ints_v1 * sizeof(int);
    G->_num_bytes_v2 = num_ints_v2 * sizeof(int);

    G->_label_v1 = (char **) R_alloc(num_v1, sizeof(char *));
    G->_label_v2 = (char **) R_alloc(num_v2, sizeof(char *));

    if (G->_label_v1 == NULL || G->_label_v2 == NULL) {
        error("bigraph_make: malloc label");
    }

    G->_neighbor_v1 = (unsigned int **) R_alloc(num_v1, sizeof(unsigned int *));
    if (G->_neighbor_v1 == NULL) { 
        error("malloc");
    }

    G->_neighbor_v1[0] = (unsigned int *) R_alloc(num_ints_v1 * num_v1, sizeof(int));
    if (G->_neighbor_v1[0] == NULL) { 
        error("malloc");
    }
    for (i = 0; i < num_v1; i++)
        G->_neighbor_v1[i] = G->_neighbor_v1[0] + i * num_ints_v1;

    G->_neighbor_v2 = (unsigned int **) R_alloc(num_v2, sizeof(unsigned int *));
    if (G->_neighbor_v2 == NULL) { 
        error("malloc");
    }

    G->_neighbor_v2[0] = (unsigned int *) R_alloc(num_ints_v2 * num_v2, sizeof(int));
    if (G->_neighbor_v2[0] == NULL) { 
        error("malloc"); 
    }
    for (i = 0; i < num_v2; i++)
        G->_neighbor_v2[i] = G->_neighbor_v2[0] + i * num_ints_v2;
    
    G->_degree_v1 = (unsigned short *) R_alloc(num_v1, sizeof(unsigned short));
    G->_degree_v2 = (unsigned short *) R_alloc(num_v2, sizeof(unsigned short));
    if (!G->_degree_v1 | !G->_degree_v2) { 
        error("malloc degree");
    }
    
    memset(G->_neighbor_v1[0], 0, G->_num_bytes_v1 * num_v1);
    memset(G->_neighbor_v2[0], 0, G->_num_bytes_v2 * num_v2);
    memset(G->_degree_v1, 0, num_v1 * sizeof(unsigned short));
    memset(G->_degree_v2, 0, num_v2 * sizeof(unsigned short));
    
    return G;
}

/** I/O functions for Graph **/

/* Read in an unweighted edge-list file, return a pointer to the graph */
BiGraph * bigraph_edgelist_in(FILE *fp)
{
    int u, v;
    unsigned int n1, n2, e;
    int k1=0, k2=0, edges=0, r;
    char word1[100], word2[100];
    BiGraph *G;
    int id;
    
    if (fscanf(fp, "%d %d %d", &n1, &n2, &e) != 3) {
        error("Bad file format: n1 n2 e incorrect");
    }
    
    G = bigraph_make(n1, n2);

    /* create a trietree */
    struct TrieNode *root = getNode();

    while ((r = fscanf(fp, "%s\t%s", word1, word2)) != EOF) {
        if (r != 2) {
            error("Bad file format: label1 label2 incorrect");
        }

        if((id = search(root, word1)) != -1) {
            u = id;
        } else {
            u = k1;
            G->_label_v1[k1++] = Strdup(word1);
            insert(root, word1, u);
        }

        if((id = search(root, word2)) != -1) {
            v = id;
        } else {
            v = k2;
            G->_label_v2[k2++] = Strdup(word2);
            insert(root, word2, v);
        }        

        if (k1 > n1) {
            error("Bad file format: too many left vertex labels");
        }
        if (k2 > n2) {
            error("Bad file format: too many right vertex labels");
        }
        
        bigraph_add_edge(G, u, v);
        edges++;
    }
    
    if (edges != e) {
        error("edgelist_in: number of edges incorrect\n");
    }
    if (k1 != n1) {
        error("edgelist_in: number of left vertices incorrect\n");
    }
    if (k2 != n2) {
        error("edgelist_in: number of right vertices incorrect\n");
    }

    return G;
}


/* Read in a bigraph from a binary matrix format file */
BiGraph * bigraph_binarymatrix_in(FILE *fp)
{
    BiGraph *G;
    char line[LINE_LENGTH];
    char delims[] = " \t\n";
    char *a = NULL;
    int n1, n2, k1, k2, i, j;

    if(fgets(line, LINE_LENGTH, fp) == NULL) return NULL;
    //fgets(line, LINE_LENGTH, fp);
    a = strtok(line, delims);
    n1 = atoi(a);
    a = strtok(NULL, delims);
    n2 = atoi(a);
    
    G = bigraph_make(n1, n2);
    
    if(fgets(line, LINE_LENGTH, fp) == NULL) return NULL;
    //fgets(line, LINE_LENGTH, fp);
    a = strtok(line, delims);
    k2 = 0;
    G->_label_v2[k2++] = Strdup(a);
    while ((a = strtok(NULL, delims)) != NULL) {
        G->_label_v2[k2++] = Strdup(a);
    }

    k1 = 0;
    while (fgets(line, LINE_LENGTH, fp) != NULL) {
        a = strtok(line, delims);
        G->_label_v1[k1] = Strdup(a);
        i = 0;
        while ((a = strtok(NULL, delims)) != NULL) {
            j = atoi(a);
                if (j==1) { bigraph_add_edge(G,k1,i); }
            i++;
        }
        k1++;
    }

    if (k1 != n1) {
        error("binarymatrix_in: # left vertices incorret\n");
    }
    if (k2 != n2) {
        error("binarymatrix_in: # right vertices incorret\n");
    }

    return G;
}


/* Print out the edgelist of a bigraph to a file */
void bigraph_edgelist_out(FILE *fp, BiGraph *G)
{
    unsigned int n1 = bigraph_num_v1(G);
    unsigned int n2 = bigraph_num_v2(G);
    unsigned int e = bigraph_num_edges(G);
    unsigned int i, j;
    fprintf(fp, "EdgeList %d %d %d\n", n1, n2, e);
    for (i = 0; i < n1; i++) {
        for (j = 0; j < n2; j++) {
            if (bigraph_edge_exists(G, i, j)) {
            fprintf(fp, "%s\t%s\n", G->_label_v1[i], G->_label_v2[j]);
            }
        }
    }
    return;
}


/* Write out the degree of each vertex in a graph to a file pointer */
void bigraph_degreelist_out(FILE *fp, BiGraph *G)
{
    unsigned int i, n1, n2;
    n1 = bigraph_num_v1(G);
    n2 = bigraph_num_v2(G);
    fprintf(fp, "DegreeList %d %d\n", n1, n2);
    for (i = 0; i < n1; i++)
        fprintf(fp, "%s\t%d\n", G->_label_v1[i], bigraph_degree_v1(G, i));
    for (i = 0; i < n2; i++)
        fprintf(fp, "%s\t%d\n", G->_label_v2[i], bigraph_degree_v2(G, i));
    return;
}
 

