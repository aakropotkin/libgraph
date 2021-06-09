#ifndef _SHORTEST_PATH_H
#define _SHORTEST_PATH_H

#include "graph.h"


/** various algorithms for shortest paths */
#define SHORTEST_PATH_NULL_PARENT ( -1 )

/**
 * Computes distance of each node from starting node
 * and stores results in dist (length n, allocated by the caller)
 * unreachable nodes get distance MAXINT
 * If parent argument is non-null, also stores parent pointers in parent
 * Allows negative-weight edges and runs in O(nm) time.
 * returns 1 if there is a negative cycle, 0 otherwise
 */
int bellman_ford( graph_t * g, int source, int * dist, int * parent );

/**
 * computes all-pairs shortest paths using Floyd-Warshall given
 * an adjacency matrix
 * answer is returned in the provided matrix!
 * assumes matrix is n pointers to rows of n ints each
 */
void floyd_warshall( int n, int ** matrix );


#endif  /* shortest_path.h */
