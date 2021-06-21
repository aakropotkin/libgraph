
#ifndef _HGRAPH_H
#define _HGRAPH_H

/* ========================================================================== */

#include "graph.h"
#include <stdio.h>


/* -------------------------------------------------------------------------- */

/**
 * Extends `graph_t' to reference nodes by hashed keys.
 * These are assumed to be strings for now, but it may be useful to add new
 * keys later.
 */
typedef struct hgraph_s  hgraph_t;

/** Create a new hgraph with n vertices labeled 0..n-1 and no edges */
hgraph_t * hgraph_create( char ** keys, int n );

/** Deep copy a `hgraph_t' */
hgraph_t * hgraph_clone( hgraph_t * );

/** free all space used by hgraph_t */
void hgraph_destroy( hgraph_t * );


/* -------------------------------------------------------------------------- */

void hgraph_add_edge( hgraph_t *, char * source, char * sink );
void hgraph_add_weighted_edge( hgraph_t *, char * source, char * sink, int wt );
void hgraph_remove_edge( hgraph_t *, char * source, char * sink );


/* -------------------------------------------------------------------------- */

/** return the number of vertices/edges in the graph_t */
int hgraph_vertex_count( hgraph_t * );
int hgraph_edge_count( hgraph_t * );


/* -------------------------------------------------------------------------- */

/** return the out-degree of a vertex */
int hgraph_out_degree( hgraph_t *, char * source );

/** return the out-degree of a vertex */
int hgraph_in_degree( hgraph_t *, char * source );

/** return weight of edge, or MAXINT ( from values.h ) if edge does not exist */
int hgraph_edge_weight( hgraph_t *, char * source, char * sink );


/* -------------------------------------------------------------------------- */

/** return 1 if edge ( source, sink ) exists, 0 otherwise */
int hgraph_has_edge( hgraph_t *, char * source, char * sink );


/* -------------------------------------------------------------------------- */

typedef void (*hgraph_foreach_fn)( hgraph_t *,
                                   char     * source,
                                   char     * sink,
                                   void     * data
                                 );

typedef void (*hgraph_w_foreach_fn)( hgraph_t *,
                                     char     * source,
                                     char     * sink,
                                     int        weight,
                                     void     * data
                                   );

/**
 * Invoke f on all edges ( u,v ) with source u
 * supplying data as final parameter to f
 * no particular order is guaranteed
 */
void hgraph_foreach( hgraph_t          *,
                     char              * source,
                     hgraph_foreach_fn   f,
                     void              * data
                   );
void hgraph_foreach_edge( hgraph_t *, hgraph_foreach_fn f, void * data );


void hgraph_foreach_weighted( hgraph_t            *,
                              char                * source,
                              hgraph_w_foreach_fn   f,
                              void                * data
                            );
void hgraph_w_foreach_edge( hgraph_t *, hgraph_w_foreach_fn f, void * data );


/* -------------------------------------------------------------------------- */

typedef void (*hgraph_foreach_vert_fn)( hgraph_t   *,
                                        const char * v,
                                        void       * data
                                      );

void hgraph_foreach_vertex( hgraph_t *, hgraph_foreach_vert_fn f, void * data );


/* -------------------------------------------------------------------------- */

/**
 * This duplicates the `hgraph_t' internal key list in the exact order of their
 * associated `graph_t' indexes.
 * This should be used sparingly, perhaps for extracting the pruned list of keys
 * created by `hgraph_parse', or for adding `hgraph_t' support to functions that
 * normally only support `graph_t'.
 *
 * The caller must `free' each key, as well as the list of strings itself.
 * The number of elements may be found with `hgraph_vertex_count'.
 */
char ** hgraph_get_keys( hgraph_t * h );


/* -------------------------------------------------------------------------- */

void print_parsed_edges( const char * const lines[], int nlines );


/* -------------------------------------------------------------------------- */

hgraph_t * hgraph_parse( FILE * stream );
hgraph_t * hgraph_fread( const char * pathname );


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
#endif  /* _HGRAPH_H */
