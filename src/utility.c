/* APIs of utility for undirected graphs 
 * Implement functions defined in utility.h 
 * Author: Yuping Lu
 * Created: February 2017
 * Last Update: March 2017
 */


#include <string.h>  /* memset */
#include <sys/time.h>  /* gettimeofday */
#include "utility.h"
 
/* Malloc and initialize a Set, returns a pointer to it */
Set *make_set(unsigned int set_size)
{
    Set *S;
    int num_ints;

    S = (Set *) Calloc(1, Set);
    if (S == NULL) { 
        error("malloc");
    }
    S->_set_size = set_size;
    S->_num_elements = 0;
    num_ints = set_size / (sizeof(unsigned int) * CHAR_BITS);
    if ((set_size % (sizeof(unsigned int) * CHAR_BITS)) != 0) num_ints++;
    S->_set = (unsigned int *) Calloc(num_ints, unsigned int);
    if (S->_set == NULL) { 
        error("malloc");
    }
    memset(S->_set, 0, num_ints * sizeof(unsigned int));

    return S;
}

/* Free memory of a set */
void free_set(Set *S)
{
    if (S != NULL) {
        if (S->_set) Free(S->_set);
        Free(S);
    }
}

/* Print out a set as bit vector */
void print_set(FILE *fp, Set *S)
{
    int i;
    fprintf(fp, "%d %d\n", set_size(S), num_elements(S));
    for (i = 0; i < set_size(S); i++) {
        if (in_set(S, i)) fprintf(fp, "1 ");
    else fprintf(fp, "0 ");
        if (i % 20 == 19) fprintf(fp, "\n");
    }
}

/* Copy set src to set dst 
     return 0 if successful, -1 if failed */ 
int copy_set(Set *dst, Set *src)
{
    int n, num_ints;
    n = set_size(dst);
    if (set_size(src) != n) return -1;  
    num_ints = n / (sizeof(unsigned int) * CHAR_BITS);
    if (n % (sizeof(unsigned int) * CHAR_BITS)) num_ints++;
    memcpy(dst->_set, src->_set, num_ints * sizeof(unsigned int));
    set_num_elements(dst, num_elements(src));
    return 0;
}

/* Return the intersection of two sets to Set dst
     return 0 if successful, -1 if failed */ 
int intersect_set(Set *dst, Set *src)
{
    int n, num_chars, num_elements=0;
    n = set_size(dst);
    if (set_size(src) != n) return -1; 
    num_chars = bit_num_chars(n); 
    mem_intersect(dst->_set, src->_set, num_chars);
    bit_count_ones(num_elements, dst->_set, num_chars);
    set_num_elements(dst, num_elements); 
    return 0;
}  

/* Return the union of two sets to set dst
     return 0 if success, -1 if failed */
int union_set(Set *dst, Set *src)
{
    int n, num_chars, num_elements=0;
    n = set_size(dst);
    if (set_size(src) != n) return -1;  
    num_chars = bit_num_chars(n); 
    mem_union(dst->_set, src->_set, num_chars);
    bit_count_ones(num_elements, dst->_set, num_chars);
    set_num_elements(dst, num_elements); 
    return 0;
}  

/* Malloc and initialize a Mapping, returns a pointer to it */
Mapping *make_mapping(unsigned int size)
{
    Mapping *M;
    int i;
    M = (Mapping *) Calloc(1, Mapping);
    if (M == NULL) { 
        error("malloc");
    }
    M->_size = size;
    M->_mapping = (short *) Calloc(size, short);
    if (M == NULL) { 
        error("malloc");
    }
    for (i = 0; i < size; i++) 
        set_mapping(M, i, -1);
    return M;
}

/* Free memory of a mapping */
void free_mapping(Mapping *M)
{
    if (M != NULL) {
        if (M->_mapping) Free(M->_mapping);
        Free(M);
    }
}

/* Print out a mapping as a list of mapped indices */
void print_mapping(FILE *fp, Mapping *M)
{
    unsigned int n, i;
    n = mapping_size(M);
    fprintf(fp, "Mapping %d\n", n);
    for (i = 0; i < n; i++) {
        fprintf(fp, "%d %d\n", i, get_mapping(M, i));
    }
    return;
}

/* return the intersection of two memory block of given size to dst */
void function_bit_intersect(void *dst, void *src, int n) {
    int my_i; 
    int my_n = n/sizeof(unsigned int); 
    unsigned int *d = (unsigned int *)dst;
    unsigned int *s = (unsigned int *)src;
    if (d != NULL && s != NULL) {
        for (my_i = 0; my_i < my_n; my_i++)
            d[my_i] &= s[my_i]; 
    }
}

/* return the intersection of two memory block of given size to dst */
void *mem_intersect(void *dst, const void *src, int n)
{
    int i;
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;
    for (i = 0; i < n; i++) d[i] &= s[i];
    return (void *)d;
}

/* return the union of two memory block of given size to dst */
void *mem_union(void *dst, const void *src, int n)
{
    int i;
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;
    for (i = 0; i < n; i++) d[i] |= s[i];
    return (void *)d;
}


/* Returns current time in seconds as a double value
     and the precision can be as accurate as microseconds
     (10^-6 second) 
 */
double get_cur_time() {
    struct timeval   tv;
    struct timezone  tz;
    double cur_time;
    gettimeofday(&tv, &tz);
    cur_time = tv.tv_sec + tv.tv_usec / 1000000.0;
    return cur_time;
}


/** Functions for Memory **/

/* Malloc a memory */
Memory *memory_make(size_t num_bytes)
{
    Memory *M;
    M = (Memory *) Calloc(1, Memory);
    if (M == NULL) { 
        error("malloc");
    }
    M->_num_bytes = num_bytes;
    M->_num_chunk = 1;
    M->_cur_chunk = 0;
    M->_head = (unsigned char **) Calloc(MAX_NUM_CHUNK, unsigned char*);
    if (M->_head == NULL) { 
        error("calloc");
    }
    M->_head[0] = (unsigned char *) Calloc(num_bytes, unsigned char);
    if (M->_head[0] == NULL) { 
        error("malloc");
    }
    M->_sbrk = M->_head[0];
    M->_tail = (unsigned char **) Calloc(MAX_NUM_CHUNK, unsigned char*);
    if (M->_tail == NULL) { 
        error("calloc");
    }
    M->_tail[0] = M->_head[0] + num_bytes;
    return M;
}

/* Free the memory */
void memory_free(Memory *M)
{
    int i;  
    if (M == NULL) return;
    for (i = 0; i < M->_num_chunk; i++)
        if (M->_head[i]) Free(M->_head[i]);
    Free(M->_head);
    Free(M->_tail);
    Free(M);
}

/* Malloc one more memory chunk */
int memory_malloc_chunk(Memory *M)
{
    if (M->_num_chunk == MAX_NUM_CHUNK) return -1;
    M->_num_chunk++;
    M->_cur_chunk++;
    M->_head[M->_cur_chunk] = (unsigned char *) Calloc(M->_num_bytes, unsigned char);
    if (M->_head[M->_cur_chunk] == NULL) { 
        error("malloc");
    }
    M->_tail[M->_cur_chunk] = M->_head[M->_cur_chunk] + M->_num_bytes;
    M->_sbrk = M->_head[M->_cur_chunk];
    return 0;
}

/* Reset a memory */
void memory_reset(Memory *M)
{
    int i;
    for (i = M->_cur_chunk+1; i < M->_num_chunk; i++) {
    if (M->_head[i]) {
        Free(M->_head[i]);
        M->_head[i] = NULL;
        M->_tail[i] = NULL;
        }
    }   
    M->_num_chunk = M->_cur_chunk + 1;
    M->_cur_chunk = 0;
    M->_sbrk = M->_head[0];
}

/* Faked malloc to replace system call malloc() */
void *fmalloc(Memory *M, size_t size)
{
    void *ptr;
    if (M->_sbrk + size > M->_tail[M->_cur_chunk]) {
    if (M->_cur_chunk == M->_num_chunk - 1) {
        if (memory_malloc_chunk(M) < 0) return NULL;
        }
    else if (M->_cur_chunk < M->_num_chunk - 1) {
            M->_cur_chunk++;
        M->_sbrk = M->_head[M->_cur_chunk];
        }
    }
    ptr = M->_sbrk;
    M->_sbrk += size;
    if (size%4) M->_sbrk += 4 - size%4;
    return ptr;
}

/* Faked calloc to replace system call calloc() */
void *fcalloc(Memory *M, size_t count, size_t size)
{
    void *ptr;
    size_t nbytes = count * size;
    ptr = fmalloc(M, nbytes);
    memset(ptr, 0, nbytes);
    return ptr;
}


