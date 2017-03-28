/* APIs of utility for undirected graphs 
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */

#ifndef __UTILITY_H
#define __UTILITY_H

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include <stdio.h>
#include <stdlib.h>


typedef unsigned short vid_t;

/** Macros **/

#define CHAR_BITS  8  /* number of bits in char */

#define XBM  31
#define RSV  5   /* >> 5 == / 32 */

#define IS_SET(vector, bit)   (vector[(bit) >> RSV] & 1 << ((bit) & XBM))
#define SET_BIT(vector, bit)  (vector[(bit) >> RSV] |= 1 << ((bit) & XBM))
#define DEL_BIT(vector, bit)  (vector[(bit) >> RSV] &= ~(1 << ((bit) & XBM)))

#define SET_VALUE(vector, index, value)  (vector[index] = value)
#define GET_VALUE(vector, index)         (vector[index])

#define MIN(a, b) (a <= b ? a : b)
#define MAX(a, b) (a >= b ? a : b)

#define MAX_NUM_CHUNK 4


/** Data structure **/

/* data structure of a set */
typedef struct set_t {
    unsigned int _set_size;     /* total number of vertices */
    unsigned int _num_elements; /* number of vertices in the set */
    unsigned int *_set;         /* bit-based vector: 1 - in set */
} Set;

/* data structure of a mapping */
typedef struct mapping_t {
    unsigned int _size;       /* total number of vertices */
    short *_mapping;   /* vector, allow number of vertices up to 32768 */
} Mapping;

/* Structure for memory management */
typedef struct memory_t {
    size_t _num_bytes;        /* number of bytes in each chunk */
    unsigned short _num_chunk;  /* number of chunks */
    unsigned short _cur_chunk;  /* current chunk id */
    unsigned char **_head;     /* head pointers of each chunk */
    unsigned char *_sbrk;     /* sbrk of the memory */
    unsigned char **_tail;     /* tail pointers of each chunk */
} Memory;


/* Macros for Set */

/* returns the size of set */
#define set_size(s)  (s->_set_size)

/* returns the number of vertices in set */
#define num_elements(s)  (s->_num_elements)

/* set the number of vertices in set */
#define set_num_elements(s, n)  (s->_num_elements = n)

/* check to see if a vertex in set or not */
#define in_set(s, u)  (IS_SET(s->_set, u))

/* add a vertex to set */
#define add_to_set(s, u) {\
    if (!IS_SET(s->_set, u)) { SET_BIT(s->_set, u); s->_num_elements++; }\
}

/* delete a vertex from set */
#define delete_from_set(s, u) {\
    if (IS_SET(s->_set, u)) { DEL_BIT(s->_set, u); s->_num_elements--; }\
}

/* Macros for Mapping */

/* returns the size of mapping */
#define mapping_size(m)  (m->_size)

/* set the mapped value of vertex u */
#define set_mapping(m, u, value)  (SET_VALUE(m->_mapping, u, value))

/* get the mapped value of vertex u */
#define get_mapping(m, u)  (GET_VALUE(m->_mapping, u))


/* Macros for bit operations */

/* return the number of 1's in a given bit string */
extern unsigned char bit_num_ones[256];
#define bit_count_ones(n, b, s) {\
    unsigned char *bits = (unsigned char *)b;\
    int my_i;\
    n = 0; \
    for (my_i = 0; my_i < s; my_i++) \
        n += bit_num_ones[bits[my_i]];\
}

/* return 1 if all bits are 0's, return 0 otherwise */
#define bit_all_zeros(b, s, yn) {\
    unsigned int *bits;\
    int my_i;\
    bits = (unsigned int *)b;\
    for (my_i = 0; my_i < s/sizeof(unsigned int); my_i++) \
        if (bits[my_i] != 0) { yn = 0; break; } \
}

/* return the intersection of two memory block of given size to dst */
#define bit_intersect(dst, src, n) {\
    int my_i, my_n=n/sizeof(unsigned int); \
    if (dst != NULL && src != NULL) {\
    for (my_i = 0; my_i < my_n; my_i++) \
            dst[my_i] &= src[my_i]; }\
}

void function_bit_intersect(void *dst, void *src, int n);

/* return the union of two memory block of given size to dst */
#define bit_union(dst, src, n) {\
    int my_i, my_n=n/sizeof(unsigned int); \
    for (my_i = 0; my_i < my_n; my_i++) \
        dst[my_i] != src[my_i]; \
}

/* return the number of unsigned ints to represent n bits */
#define bit_num_ints(n) (n / (sizeof(unsigned int)*CHAR_BITS) + \
(n % (sizeof(unsigned int)*CHAR_BITS) == 0 ? 0 : 1))

/* return the number of chars to represent n bits */
#define bit_num_chars(n) (sizeof(unsigned int) * bit_num_ints(n))

/** Function prototypes **/

/* Malloc a set, initialize it and returns a pointer to it */
Set *make_set(unsigned int set_size);

/* Free the memory of a set */
void free_set(Set *S);

/* Print out a set as 0/1 vector */
void print_set(FILE *fp, Set *S);

/* Copy set src to set dst 
     return 0 if successful, -1 if failed */ 
int copy_set(Set *dst, Set *src);

/* Return the intersection of two sets to Set dst
     return 0 if successful, -1 if failed */
int intersect_set(Set *dst, Set *src);
    
/* Return the union of two sets to set dst
     return 0 if success, -1 if failed */
int union_set(Set *dst, Set *src);

/* Malloc a mapping, initialize it and returns a pointer to it */
Mapping *make_mapping(unsigned int size);

/* Free the memory of a set */
void free_mapping(Mapping *M);

/* Print out a mapping as mapping list */
void print_mapping(FILE *fp, Mapping *M);


/** Memory operations **/

/* return the intersection of two memory block of given size to dst */
void *mem_intersect(void *dst, const void *src, int n);

/* return the union of two memory block of given size to dst */
void *mem_union(void *dst, const void *src, int n);

/* return number of integers needed for a given number of bits n */
//unsigned int bit_num_ints(int n);

/** Timer function **/

/* Returns current time in seconds as a double value
     and the precision can be as accurate as microseconds
     (10^-6 second) 
 */
double get_cur_time();


/** Functions for Memory **/
/* Malloc a memory */
Memory *memory_make(size_t num_bytes);

/* Free the memory */
void memory_free(Memory *M);

/* Reset memory */
void memory_reset(Memory *M);

/* Faked malloc to replace system call malloc() */
void *fmalloc(Memory *M, size_t size);

/* Faked calloc to replace system call calloc() */
void *fcalloc(Memory *M, size_t count, size_t size);

 
#endif  /* __UTILITY_H */

