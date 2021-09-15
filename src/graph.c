
#include <string.h>
#include <assert.h>
#include <values.h>
#include <stdlib.h>


/* -------------------------------------------------------------------------- */

/**
 * basic directed graph type
 * the implementation uses adjacency lists
 * represented as variable-length arrays
 * these arrays may or may not be sorted: if one gets long enough
 * and you call graph_has_edge on its source, it will be
 */
struct graph_s {
  int n;               /* number of vertices */
  int m;               /* number of edges */
  struct successors {
    int  dout;         /* number of successors */
    int  din;          /* number of predecessors */
    int  len;          /* number of slots in array */
    char is_sorted;    /* true if list is already sorted */
    struct edge {
      int sink;
      int weight;
    } list[1];      /* actual list of successors */
  } * alist[1];
};

#include "graph.h"


/* -------------------------------------------------------------------------- */

/** Create a new graph with n vertices labeled 0..n-1 and no edges */
  graph_t *
graph_create( int n )
{
  graph_t * g = NULL;
  int       i = -1;

  g = malloc( sizeof( struct graph_s ) +
              ( sizeof( struct successors * ) * ( n - 1 ) )
            );
  assert( g != NULL );

  g->n = n;
  g->m = 0;

  for( i = 0; i < n; i++ )
    {
      g->alist[i] = malloc( sizeof( struct successors ) );
      assert( g->alist[i] != NULL );

      g->alist[i]->dout      = 0;
      g->alist[i]->din       = 0;
      g->alist[i]->len       = 1;
      g->alist[i]->is_sorted = 1;
    }

  return g;
}


/* -------------------------------------------------------------------------- */

/** Deep copy a `graph_t'. */
  graph_t *
graph_clone( graph_t * g )
{
  assert( g != NULL );
  int i = -1;
  int n = graph_vertex_count( g );
  assert( 0 < n );
  graph_t * g_new = malloc( sizeof( struct graph_s ) +
                            ( sizeof( struct successors * ) * ( n - 1 ) )
                          );
  assert( g_new != NULL );
  g_new->n = n;
  g_new->m = graph_edge_count( g );

  for ( i = 0; i < n; i++ )
    {
      g_new->alist[i] =
        malloc( sizeof( struct successors ) +
                sizeof( struct edge ) * ( g->alist[i]->len - 1 )
              );
      assert( g_new->alist[i] != NULL );
      memcpy( g_new->alist[i],
              g->alist[i],
              ( sizeof( struct successors ) +
                sizeof( struct edge ) * ( g->alist[i]->len -1 )
              )
            );
    }
  return g_new;
}


/* -------------------------------------------------------------------------- */

/** Free all space used by `graph_t' */
  void
graph_destroy( graph_t * g )
{
  assert( g != NULL );
  for( int i = 0; i < g->n; i++ )
    {
      if ( g->alist[i] != NULL )
        {
          free( g->alist[i] );
          g->alist[i] = NULL;
        }
    }
  free( g );
}


/* -------------------------------------------------------------------------- */

/** add an edge to an existing graph_t */
  void
graph_add_weighted_edge( graph_t * g, int u, int v, int wt )
{
  assert( g != NULL );
  assert( 0 <= u );
  assert( u < g->n );
  assert( 0 <= v );
  assert( v < g->n );

  /* do we need to grow the list? */
  while( g->alist[u]->len <= g->alist[u]->dout )
    {
      g->alist[u]->len *= 2;
      g->alist[u] = realloc( g->alist[u],
                             sizeof( struct successors ) +
                             sizeof( struct edge ) * ( g->alist[u]->len - 1 )
                           );
    }

  /* now add the new sink */
  g->alist[u]->list[g->alist[u]->dout].sink   = v;
  g->alist[u]->list[g->alist[u]->dout].weight = wt;
  g->alist[u]->dout++;
  g->alist[v]->din++;
  g->alist[u]->is_sorted = 0;

  /* bump edge count */
  g->m++;
}


  void
graph_add_edge_weighted_safe( graph_t * g, int u, int v, int w )
{
  if ( ! graph_has_edge( g, u, v ) )
    {
      graph_add_weighted_edge( g, u, v, w );
    }
}


  void
graph_add_edge_safe( graph_t * g, int u, int v )
{
  if ( ! graph_has_edge( g, u, v ) )
    {
      graph_add_edge( g, u, v );
    }
}


/** Remove an edge */
  void
graph_remove_edge( graph_t * g, int u, int v )
{
  assert( g != NULL );
  assert( 0 <= u );
  assert( u < g->n );
  assert( 0 <= v );
  assert( v < g->n );

  if ( ! graph_has_edge( g, u, v ) )
    {
      return;
    }

  for ( int i = 0; i < g->alist[u]->dout; i++ )
    {
      if ( g->alist[u]->list[i].sink != v )
        {
          continue;
        }

      /* Shift edge data to fill removed entry, unless it was the last entry. */
      if ( i != ( g->alist[u]->dout - 1 ) )
        {
          memmove( g->alist[u]->list + i,
                   g->alist[u]->list + ( i + 1 ),
                   ( sizeof( int ) * 2 ) * ( g->alist[u]->dout - i - 1 )
                 );
        }

      break;
    }

  /* `sorted' property shouldn't change for a removal. */
  g->alist[u]->dout--;
  assert( 0 <= g->alist[u]->dout );
  g->alist[v]->din--;
  assert( 0 <= g->alist[v]->din );
  g->m--;  /* Decrement total edge count. */
  assert( 0 <= g->m );
}


  void
graph_add_edge( graph_t * g, int u, int v )
{
  graph_add_weighted_edge( g, u, v, GRAPH_DEFAULT_EDGE_WEIGHT );
}


/* -------------------------------------------------------------------------- */

/** return the number of vertices in the graph_t */
  int
graph_vertex_count( graph_t * g )
{
  return g->n;
}


/** return the number of vertices in the graph_t */
  int
graph_edge_count( graph_t * g )
{
  return g->m;
}


/** return the out-degree of a vertex */
  int
graph_out_degree( graph_t * g, int source )
{
  assert( 0 <= source );
  assert( source < g->n );
  return g->alist[source]->dout;
}


/** return the out-degree of a vertex */
  int
graph_in_degree( graph_t * g, int source )
{
  assert( 0 <= source );
  assert( source < g->n );
  return g->alist[source]->din;
}


/* -------------------------------------------------------------------------- */

/** when we are willing to call bsearch */
#define BSEARCH_THRESHOLD  ( 10 )


  static int
edgecmp( const void * a, const void * b )
{
  assert( a != NULL );
  assert( b != NULL );
  return ( (const struct edge *) a )->sink - ( (const struct edge *) b )->sink;
}


/* return 1 if edge (source, sink) exists), 0 otherwise */
  static int
graph_has_edge_internal( graph_t * g, int source, int sink, int * weight )
{
  int i = -1;
  struct edge   sinkedge;
  struct edge * bsearch_result = NULL;

  assert( g != NULL );
  assert( 0 <= source );
  assert( source < g->n );
  assert( 0 <= sink );
  assert( sink < g->n );

  /* Default if not found */
  *weight = MAXINT;

  if ( BSEARCH_THRESHOLD <= graph_out_degree( g, source ) )
    {
      /* make sure it is sorted */
      if ( ! g->alist[source]->is_sorted )
        {
          qsort( g->alist[source]->list,
                 g->alist[source]->dout,
                 sizeof( struct edge ),
                 edgecmp
               );
        }

      /* call bsearch to do binary search for us */
      sinkedge.sink   = sink;
      sinkedge.weight = GRAPH_DEFAULT_EDGE_WEIGHT;

      bsearch_result = bsearch( & sinkedge,
                                g->alist[source]->list,
                                g->alist[source]->dout,
                                sizeof( struct edge ),
                                edgecmp
                              );
      if ( bsearch_result != 0 )
        {
          *weight = bsearch_result->weight;
          return 1;
        }
      else
        {
          return 0;
        }
    } else {
      /* just do a simple linear search
       * we could call lfind for this, but why bother? */
      for( i = 0; i < g->alist[source]->dout; i++ )
        {
          if ( g->alist[source]->list[i].sink == sink )
            {
              *weight = g->alist[source]->list[i].weight;
              return 1;
            }
        }
      /* else */
      return 0;
    }
}


/* -------------------------------------------------------------------------- */

  int
graph_has_edge( graph_t * g, int source, int sink )
{
  assert( g != NULL );
  assert( source < g->n );
  int dummy = -1;
  return graph_has_edge_internal( g, source, sink, & dummy );
}


/* -------------------------------------------------------------------------- */

  int
graph_edge_weight( graph_t * g, int source, int sink )
{
  assert( g != NULL );
  assert( source < g->n );
  assert( sink < g->n );
  int weight = -1;
  graph_has_edge_internal( g, source, sink, & weight );
  return weight;
}


/* -------------------------------------------------------------------------- */

/**
 * invoke f on all edges (u,v) with source u
 * supplying data as final parameter to f
 */
  void
graph_foreach( graph_t * g,
               int       source,
               void (*f)( graph_t * g, int source, int sink, void * data ),
               void    * data
             )
{
  assert( g != NULL );
  assert( 0 <= source );
  assert( source < g->n );

  for ( int i = 0; i < g->alist[source]->dout; i++ )
    {
      f( g, source, g->alist[source]->list[i].sink, data );
    }
}


  void
graph_foreach_weighted(
    graph_t * g,
    int       source,
    void (*f)( graph_t * g, int source, int sink, int weight, void * data ),
    void    * data
  )
{
  assert( g != NULL );
  assert( 0 <= source );
  assert( source < g->n );

  for ( int i = 0; i < g->alist[source]->dout; i++ )
    {
      f( g,
         source,
         g->alist[source]->list[i].sink,
         g->alist[source]->list[i].weight,
         data
       );
    }
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
