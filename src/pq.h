#ifndef _PQ_H
#define _PQ_H

/** generic priority queue */
typedef struct pq_s  pq_t;

/**
 * Create a new empty priority queue
 * `element_length' is the size of an element in bytes
 * compare is a comparision function that returns
 * < 0 if its first argument is less than its second
 *   0 if they are equal
 * > 0 if the first argument is greater than the second.
 */
 pq_t * pq_create( int element_length,
                   int (*compare)( const void *, const void * )
                 );

/**
 * Create a new empty queue.
 * This uses the same underlying priority queue but the comparator treats all
 * elements as equal.
 */
pq_t * q_create( int element_length );

/** free a priority queue */
void pq_destroy(  pq_t * );

/**
 * Add an element to the priority queue
 * the contents of the element are COPIED from elt
 */
void pq_insert(  pq_t *, const void * elt );

/** Returns nonzero if  pq * is empty */
int pq_is_empty(  pq_t * );

/**
 * Delete the minimum element of the priority queue
 * and COPY its contents to retval
 * it is an error to call this on an empty queue
 */
void pq_delete_min(  pq_t *, void * retval );

/** Utility function: blows up if heap invariant is violated */
void pq_sanity_check(  pq_t * );


#endif /* pq.h */
