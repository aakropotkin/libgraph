#ifndef _SEARCH_H
#define _SEARCH_H

#include "graph.h"


/* ========================================================================== */

/**
 * Typical usage:
 *
 *    struct search_info *s;
 *    int n;
 *
 *    s = search_info_create( g );
 *
 *    n = graph_vertices( g );
 *    for( i = 0; i < n; i++ )
 *      {
 *        dfs( s, i );
 *      }
 *
 *    // ... use results in s ...
 *
 *    search_info_destroy( s );
 */


/* -------------------------------------------------------------------------- */

/**
 * Summary information per node for dfs and bfs
 * this is not intended to be opaque---user can read it
 * ( but should not write it! )
 */
#define SEARCH_INFO_NULL ( -1 )  /* for empty slots */

struct search_info {
  int       reached;  /* count of reached nodes */
  graph_t * graph;
  union {
    int * preorder;   /* list of nodes in order first reached */
    int * lowlink;    /* lowest reachable `time' value when performing Tarjan */
  };
  int * time;         /* `time[i] == position' of node `i' in preorder list */
  int * parent;       /* parent in DFS or BFS forest */
  union {
    int * depth;      /* distance from root */
    int * tj_stack;   /* Node is currently "on the stack" for Tarjan's Algo */
  };
};


/* -------------------------------------------------------------------------- */

/**
 * allocate and initialize search results structure
 * you need to do this before passing it to dfs or bfs
 */
struct search_info * search_info_create( graph_t * g );

/** free search_info data---does NOT free graph pointer */
void search_info_destroy( struct search_info * );


/* -------------------------------------------------------------------------- */

/** perform depth-first search starting at root, updating results */
void dfs( struct search_info * results, int root );

/** perform breadth-first search starting at root, updating results */
void bfs( struct search_info * results, int root );


/* -------------------------------------------------------------------------- */

/** It is possible to reach all vertices from at least one vertex. */
int graph_is_connected( graph_t * );

/** It is possible to reach all vertices from all vertices. */
int graph_is_strongly_connected( graph_t * );


/* -------------------------------------------------------------------------- */

/** Mark vertices which are reachable from vertex `source' */
char * mark_reachable( graph_t *, int source );


/* -------------------------------------------------------------------------- */

/** Return the number of disconnected subgraphs in a given graph. */
int count_subgraphs( graph_t * );

/**
 * Mark vertices which are members of the same subgraph as vertex `source'.
 * This might also be phrased as:
 * "Mark all vertices reachable from vertices reachable from `source'"
 */
char * mark_same_subgraph( graph_t *, int source );


/** Map vertices to subgraph groups ( unordered ). */
int * mark_subgraph_groups( graph_t * );

/**
 * Split a graph into subgraphs returned as an array.
 * The length of the resulting array will be stored in `num_subgraphs' */
graph_t ** split_subgraphs( graph_t *, int * num_subgraphs );


/* -------------------------------------------------------------------------- */

/** Run Tarjan's algorithm returning a list of `SCC' groups. */
int * tarjan( graph_t * );


/* -------------------------------------------------------------------------- */

int node_seen( struct search_info *, int node );


/* -------------------------------------------------------------------------- */



/* ========================================================================== */

#endif /* search.h */
