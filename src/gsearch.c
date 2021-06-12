
#include "gsearch.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


/* -------------------------------------------------------------------------- */

/** Create an array of `n' ints initialized to `SEARCH_INFO_NULL' */
  static int *
create_empty_array( int n )
{
  int * a = NULL;
  int   i = -1;

  a = malloc( sizeof( *a ) * n );
  assert( a != NULL );

  for( i = 0; i < n; i++ )
    {
      a[i] = SEARCH_INFO_NULL;
    }

  return a;
}


/* -------------------------------------------------------------------------- */

/**
 * Allocate and initialize search results structure
 * you need to do this before passing it to dfs or bfs
 */
  struct search_info *
search_info_create( graph_t * g )
{
  assert( g != NULL );
  struct search_info * s;
  const int n = graph_vertex_count( g );
  assert( 0 < n );

  s = malloc( sizeof( struct search_info ) );
  assert( s != NULL );

  s->graph   = g;
  s->reached = 0;

  s->preorder = create_empty_array( n );
  assert( s->lowlink != NULL );
  s->time = create_empty_array( n );
  s->parent = create_empty_array( n );
  s->depth = create_empty_array( n );
  assert( s->tj_stack != NULL );

  return s;
} 


/** Free `search_info' data---does NOT free graph pointer */
  void
search_info_destroy( struct search_info * s )
{
  free( s->depth );
  s->depth = NULL;
  free( s->parent );
  s->parent = NULL;
  free( s->time );
  s->time = NULL;
  free( s->preorder );
  s->preorder = NULL;
  free( s->lowlink );
  s->lowlink = NULL;
  free( s );
  s = NULL;
}


/* -------------------------------------------------------------------------- */

/** used inside search routines */
struct edge {
  int u;          /* source */
  int v;          /* sink */
};


/** stack/queue */
struct queue {
  struct edge * e;
  int           bottom;
  int           top;
};


  static inline struct queue *
queue_create( int n )
{
  struct queue * q = malloc( sizeof( struct queue ) );
  assert( q != NULL );

  q->e = malloc( sizeof( struct edge ) * n );
  assert( q->e != NULL );

  q->top = 0;
  q->bottom = 0;

  return q;
}


  static inline struct queue *
queue_create_for_graph( graph_t * g )
{
  assert( g != NULL );
  /* Start with empty `q' we need one space per edge plus one for the fake
   * `( root, root )' edge. */
  return queue_create( graph_edge_count( g ) + 1 );
}

  static inline void
queue_destroy( struct queue * q )
{
  if ( q == NULL )
    {
      return;
    }
  free( q->e );
  q->e = NULL;
  free( q );
  q = NULL;
}


  static inline int
queue_empty( struct queue * q )
{
  assert( q != NULL );
  return q->top <= q->bottom;
}


  static void
push_edge( graph_t * g, int u, int v, void * data )
{
  assert( g != NULL );
  assert( data != NULL );

  struct queue * q = data;
  const int n = graph_edge_count( g ) + 1;

  assert( q->top < n );

  q->e[q->top].u = u;
  q->e[q->top].v = v;
  q->top++;

  if ( q->top < n )
    {
      q->e[q->top].u = SEARCH_INFO_NULL;
      q->e[q->top].v = SEARCH_INFO_NULL;
    }
}


  static struct edge
stack_pop_edge( struct queue * q )
{
  assert( q != NULL );
  assert( ! queue_empty( q ) );

  q->top--;
  struct edge e = q->e[q->top];

  /* Clear the spot where `e' was stored. */
  q->e[q->top].u = SEARCH_INFO_NULL;
  q->e[q->top].v = SEARCH_INFO_NULL;

  return e;
}


  static struct edge
queue_pop_edge( struct queue * q )
{
  assert( q != NULL );
  assert( ! queue_empty( q ) );

  struct edge e = q->e[q->bottom];

  q->e[q->bottom].u = SEARCH_INFO_NULL;
  q->e[q->bottom].v = SEARCH_INFO_NULL;
  q->bottom++;

  return e;
}


/* -------------------------------------------------------------------------- */

  static inline int
_node_seen( struct search_info * s, int node )
{
  return s->parent[node] != SEARCH_INFO_NULL;
}

  int
node_seen( struct search_info * s, int node )
{
  assert( s != NULL );
  assert( s->graph != NULL );
  assert( node < graph_vertex_count( s->graph ) );
  return _node_seen( s, node );
}


/* -------------------------------------------------------------------------- */

enum search_kind_e {
  SK_DFS = 0,
  SK_BFS
};

/** This implements both DFS and BFS */
  static void
generic_search( struct search_info * r, int root, enum search_kind_e sk )
{
  assert( r != NULL );
  assert( r->graph != NULL );
  assert( ( sk == SK_BFS ) || ( sk == SK_DFS ) );

  const int n = graph_vertex_count( r->graph );
  struct queue * q = queue_create_for_graph( r->graph );  /* Queue/Stack */
  struct edge cur;  /* Edge we are working on */
  push_edge( r->graph, root, root, q );  /* Push the root */

  /* While q.e not empty */
  while( ! queue_empty( q ) )
    {
      cur = ( sk == SK_BFS ) ? queue_pop_edge( q ) : stack_pop_edge( q );
      /* Did we visit `sink' already? */
      if ( _node_seen( r, cur.v ) )
        {
          continue;
        }

      /* No, we did not visit `sink' yet. */
      assert( r->reached < n );
      r->parent[cur.v] = cur.u;
      r->time[cur.v] = r->reached;
      r->preorder[r->reached] = cur.v;
      r->reached++;

      /* We could avoid this if we were certain `SEARCH_INFO_NULL'
       * would never be anything but `-1' */
      r->depth[cur.v] = ( cur.u == cur.v ) ? 0 : r->depth[cur.u] + 1;

      /* Push all outgoing edges */
      graph_foreach( r->graph, cur.v, push_edge, q );
  }

  queue_destroy( q );
  q = NULL;
}


  void
dfs( struct search_info * results, int root )
{
  generic_search( results, root, SK_DFS );
}


  void
bfs( struct search_info * results, int root )
{
  generic_search( results, root, SK_BFS );
}


/* -------------------------------------------------------------------------- */

  int
count_subgraphs( graph_t * g )
{
  assert( g != NULL );
  int count = 0;
  int i     = -1;
  const int n = graph_vertex_count( g );
  struct search_info * s = search_info_create( g );

  do {
    for ( i = 0; i < n; i++ )
      {
        if ( s->parent[i] == SEARCH_INFO_NULL )
          {
            break;
          }
      }
    count++;
    bfs( s, i );
  } while( s->reached < n );

  search_info_destroy( s );
  s = NULL;

  return count;
}


/* -------------------------------------------------------------------------- */

  static void
clone_vertex( graph_t * g, int source, int sink, int weight, void * new_g )
{
  graph_add_weighted_edge( new_g, source, sink, weight );
}


  static void
clone_undirected( graph_t * g, int source, int sink, int weight, void * new_g )
{
  graph_add_weighted_edge( new_g, source, sink, weight );
  if ( ! graph_has_edge( g, sink, source ) )
    {
      graph_add_weighted_edge( new_g, sink, source, weight );
    }
}


/* -------------------------------------------------------------------------- */

  graph_t **
split_subgraphs( graph_t * g, int * num_subgraphs )
{
  assert( g != NULL );
  assert( num_subgraphs != NULL );
  *num_subgraphs = count_subgraphs( g );

  const int n = graph_vertex_count( g );
  assert( 0 < n );
  graph_t ** subgraphs = calloc( (size_t) *num_subgraphs, sizeof( graph_t * ) );
  assert( subgraphs != NULL );
  int * groups = mark_subgraph_groups( g );
  assert( groups != NULL );

  for ( int i = 0; i < *num_subgraphs; i++ )
    {
      subgraphs[i] = graph_create( n );
    }

  for ( int i = 0; i < n; i++ )
    {
      graph_foreach_weighted( g, i, clone_vertex, subgraphs[groups[i]] );
    }

  free( groups );
  groups = NULL;

  return subgraphs;
}


/* -------------------------------------------------------------------------- */

  char *
mark_reachable( graph_t * g, int source )
{
  assert( g != NULL );
  assert( 0 <= source );
  const int n = graph_vertex_count( g );
  char * r = malloc( sizeof( char ) * n );
  assert( r != NULL );
  struct search_info * s = search_info_create( g );

  bfs( s, source );

  for ( int i = 0; i < n; i++ )
    {
      r[i] = s->parent[i] != SEARCH_INFO_NULL;
    }

  search_info_destroy( s );
  s = NULL;

  return r;
}


/* -------------------------------------------------------------------------- */

  char *
mark_same_subgraph( graph_t * g, int source )
{
  assert( g != NULL );
  assert( 0 <= source );
  const int n = graph_vertex_count( g );
  assert( 0 < n );
  graph_t * g_new = graph_create( n );  /* Clone `g' since we write to it. */
  assert( g_new != NULL );
  char * r = malloc( sizeof( char ) * n );
  assert( r != NULL );
  struct search_info * s = NULL;

  /* Make graph undirected and run search from `source'.
   * Because adding edges modifies the out-degree of vertices we should not
   * rely on `graph_foreach' here. */
  for ( int i = 0; i < n; i++ )
    {
      graph_foreach_weighted( g, i, clone_undirected, g_new );
    }

  s = search_info_create( g_new );
  assert( s != NULL );

  bfs( s, source );

  for ( int i = 0; i < n; i++ )
    {
      r[i] = s->parent[i] != SEARCH_INFO_NULL;
    }

  search_info_destroy( s );
  s = NULL;
  graph_destroy( g_new );
  g_new = NULL;

  return r;
}


/* -------------------------------------------------------------------------- */

  int *
mark_subgraph_groups( graph_t * g )
{
  assert( g != NULL );
  const int n = graph_vertex_count( g );
  int    grp    = 0;  /* Current subgraph generation */
  int  * groups = create_empty_array( n );
  char * group  = NULL;

  assert( SEARCH_INFO_NULL < 0 );

  /* As ungrouped vertices are encountered find their shared subgraph members
   * and mark them in "generations". */
  for ( int i = 0; i < n; i++ )
    {
      /* Skip any assigned vertices. */
      if ( groups[i] != SEARCH_INFO_NULL )
        {
          continue;
        }

      /* Find other subgraph members. */
      group = mark_same_subgraph( g, i );
      assert( group != NULL );

      /* Mark generation */
      for ( int j = i; j < n; j++ )
        {
          if ( group[j] )
            {
              groups[j] = grp;
            }
        }

      /* Increment the generation counter. */
      grp++;

      free( group );
      group = NULL;
    }

  return groups;
}


/* -------------------------------------------------------------------------- */

  int
graph_is_connected( graph_t * g )
{
  assert( g != NULL );
  const int n = graph_vertex_count( g );
  assert( 0 < n );
  struct search_info * s = search_info_create( g );
  assert( s != NULL );

  for ( int i = 0; i < n; i++ )
    {
      if ( s->time[i] == SEARCH_INFO_NULL )
        {
          dfs( s, i );
          if ( n <= s->reached )
            {
              search_info_destroy( s );
              s = NULL;
              return 1;
            }
        }
    }

  search_info_destroy( s );
  s = NULL;

  return 0;
}


/* -------------------------------------------------------------------------- */

  int
graph_is_strongly_connected( graph_t * g )
{
  assert( g != NULL );
  const int n = graph_vertex_count( g );
  assert( 0 < n );
  int * scss = tarjan( g );
  for ( int i = 1; i < n; i++ )
    {
      if ( scss[i - 1] != scss[i] )
        {
          free( scss );
          scss = NULL;
          return 0;
        }
    }
  free( scss );
  scss = NULL;
  return 1;
}


/* -------------------------------------------------------------------------- */

  static int
array_contains( int * hay, int needle, int len )
{
  assert( hay != NULL );
  for ( int i = 0; i < len; i++ )
    {
      if ( hay[i] == needle )
        {
          return 1;
        }
    }
  return 0;
}


/* -------------------------------------------------------------------------- */

/**
 * Assign Strongly Connected Component labels to each vertex.
 * Tarjan in a nushell:
 *
 * 1. Mark the id of each node as unvisited.
 *
 * 2. Start DFS. Upon visiting a node assign it an id ( time ) and
 *    low-link value.
 *    Also mark the current node as visitied and add them to a "seen stack".
 *
 * 3. On DFS callback, if the previous node is on the stack then `min' the
 *    current node's low-link value with the last node's low-link value.
 *    By "DFS" callback what we mean is that because DFS was performed
 *    recursively we perform this step after returning from performing DFS on a
 *    neighboring node.
 *
 * 4. After visiting all neighbors, if the current node started a connected
 *    component ( `time[u] == lowlink[u]' ) then pop nodes off the stack until
 *    the current node is removed.
 */


/**
 * Tarjan helper function to determine if a node is "on the stack" from the
 * phrasing of the actual algorithm.
 * This should not be confused to mean that a node is LITERALLY in the queue `q'
 * which often temporarily holds nodes which already have a completely solved
 * `lowlink' value.
 *
 * "On the stack" really means "the low-link value is not finalized".
 */
  static inline int
source_on_stack( struct search_info * s, int source )
{
  assert( s != NULL );
  return s->tj_stack[source] != SEARCH_INFO_NULL;
}


/**
 * Adopt the lowest low-link value reachable from a node, but ( imporant )
 * ignore any node which is "not on the stack".
 */
  static void
update_lowlink( graph_t * g, int u, int v, void * data )
{
  assert( data != NULL );
  struct search_info * s = data;
  assert( node_seen( s, v ) );
  if ( ( s->lowlink[v] < s->lowlink[u] ) && source_on_stack( s, v ) )
    {
      s->lowlink[u] = s->lowlink[v];
    }
}


/**
 * This does most of the heavy lifting.
 * It considers updating low-link values both when a previously discovered node
 * is encountered, and after all neighbors have "finalized" their low-link.
 */
  static void
tarjan_dfs( struct search_info * s, int root, struct queue * q )
{
  assert( s != NULL );
  assert( s->graph != NULL );
  assert( q != NULL );

  /* Peek, don't pop. We have to "pop" after processing all outgoing edges. */
  int cur_top = q->top - 1;
  struct edge cur = q->e[cur_top];

  if ( _node_seen( s, cur.v ) )
    {
      /* Node was already visited, but consider adopting its lowlink. */
      if ( source_on_stack( s, cur.v )                &&
           ( s->lowlink[cur.v] < s->lowlink[cur.u] )
         )
        {
          s->lowlink[cur.u] = s->lowlink[cur.v];
        }
    }
  else
    {
      s->parent[cur.v] = cur.u;
      s->time[cur.v] = s->reached;
      s->lowlink[cur.v] = s->reached;
      s->reached++;
      s->tj_stack[cur.v] = 1;

      /* Recursively descend into neighbors. */
      graph_foreach( s->graph, cur.v, push_edge, q );
      while ( cur_top < ( q->top - 1 ) )
        {
          tarjan_dfs( s, root, q );
        }
      /* Now the neighbors are finalized, consider updates. */
      graph_foreach( s->graph, cur.v, update_lowlink, s );
    }

  /* Okay now you can actually "pop" since we've processed outgoing edges. */
  if ( s->time[cur.u] == s->lowlink[cur.u] )
    {
      /* We "completed" marking an SCC, pop until we get back to the
        * root of THAT component ( not necessarily `root' argument ). */
      while ( ( ! queue_empty( q ) ) && ( q->e[q->top - 1].u == cur.u ) )
        {
          stack_pop_edge( q );
        }
      s->tj_stack[cur.u] = SEARCH_INFO_NULL;
    }
  else
    {
      stack_pop_edge( q );
    }
}


  int *
tarjan( graph_t * g )
{
  assert( g != NULL );
  const int n = graph_vertex_count( g );
  assert( 0 < n );
  struct search_info * s = search_info_create( g );
  assert( s != NULL );
  struct queue * q = NULL;

  /* Start DFS.
   * Upon visiting a node assign it an `id' ( `time' ) and a low-link value.
   * Also mark the current source as visited and add them to the stack. */
  for ( int i = 0; i < n; i++ )
    {
      if ( ! node_seen( s, i ) )
        {
          q = queue_create_for_graph( g );
          /* Push a fake edge `( i, i )' as a starting point. */
          push_edge( g, i, i, q );
          tarjan_dfs( s, i, q );
          queue_destroy( q );
          q = NULL;
          free( s->tj_stack );
          s->tj_stack = create_empty_array( n );
        }
    }

  /* Copy out low-link values so they may be returned. */
  int * scss = malloc( sizeof( int ) * n );
  assert( scss != NULL );
  memcpy( scss, s->lowlink, sizeof( int ) * n );

  search_info_destroy( s );
  s = NULL;

  return scss;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
