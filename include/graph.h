
#ifndef _GRAPH_H
#define _GRAPH_H

/* ========================================================================== */

/** basic directed graph type */
typedef struct graph_s  graph_t;

/** Create a new graph with n vertices labeled 0..n-1 and no edges */
graph_t * graph_create( int n );

/** Deep copy a `graph_t' */
graph_t * graph_clone( graph_t * );

/** free all space used by graph_t */
void graph_destroy( graph_t * );


/* -------------------------------------------------------------------------- */

/**
 * add an edge to an existing graph
 * doing this more than once may have unpredictable results
 */
#ifndef GRAPH_DEFAULT_EDGE_WEIGHT
#  define GRAPH_DEFAULT_EDGE_WEIGHT  ( 1 )
#endif  /* GRAPH_DEFAULT_EDGE_WEIGHT */
void graph_add_edge( graph_t *, int source, int sink );
void graph_add_weighted_edge( graph_t *, int source, int sink, int wt );
void graph_add_edge_safe( graph_t *, int source, int sink );
void graph_add_weighted_edge_safe( graph_t *, int source, int sink, int wt );
void graph_remove_edge( graph_t *, int source, int sink );


/* -------------------------------------------------------------------------- */

/** return the number of vertices/edges in the graph_t */
int graph_vertex_count( graph_t * );
int graph_edge_count( graph_t * );


/* -------------------------------------------------------------------------- */

/** return the out-degree of a vertex */
int graph_out_degree( graph_t *, int source );

/** return the out-degree of a vertex */
int graph_in_degree( graph_t *, int source );

/** return weight of edge, or MAXINT ( from values.h ) if edge does not exist */
int graph_edge_weight( graph_t *, int source, int sink );


/* -------------------------------------------------------------------------- */

/** return 1 if edge ( source, sink ) exists, 0 otherwise */
int graph_has_edge( graph_t *, int source, int sink );


/* -------------------------------------------------------------------------- */

typedef void (*graph_foreach_fn)( graph_t *,
                                  int       source,
                                  int       sink,
                                  void    * data
                                );

typedef void (*graph_w_foreach_fn)( graph_t *,
                                    int        source,
                                    int        sink,
                                    int        weight,
                                    void     * data
                                  );

/**
 * Invoke f on all edges ( u,v ) with source u
 * supplying data as final parameter to f
 * no particular order is guaranteed
 */
void graph_foreach( graph_t          * g,
                    int                source,
                    graph_foreach_fn   f,
                    void             * data
                  );

void graph_foreach_weighted( graph_t            * g,
                             int                  source,
                             graph_w_foreach_fn   f,
                             void               * data
                           );


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
#endif /* graph.h */
