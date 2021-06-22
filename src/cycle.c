
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "graph.h"
#include "pq.h"
#include "gsearch.h"


struct dins_q_s {
  int  * dins;
  pq_t * q;
};


  static void
visit_vertex_detect( graph_t * g, int source, int sink, void * dq_data )
{
  assert( g != NULL );
  assert( dq_data != NULL );
  struct dins_q_s * dq = (struct dins_q_s *) dq_data;
  dq->dins[sink]--;
  if ( dq->dins[sink] == 0 )
    {
      pq_insert( dq->q, & sink );
    }
}


/**
 * The idea is to use Kahn's Algorithm for Topological Sort to detect if a
 * cycle exists in a directed graph.
 *
 * Only True/False is computed, the cycle itself is not necessarily reported.
 *
 *
 * Step 1: Compute in-degree foreach node, initialize "visited count" to 0.
 *
 * Step 2: Pick all nodes with in-degree as 0 and add them to a queue.
 *
 * Step 3: Pop queue and perform:
 *         a. Increment visited count by 1.
 *         b. Decrease in-degree by 1 forall neighboring nodes.
 *         c. If in-degree of a neighbor is reduced to 0, then add it to queue.
 *
 * Step 4: Repeat Step 3 until queue is empty.
 *
 * Step 5: If count of visited nodes is not equal to the number of nodes in the
 *         graph then a cycle exists, else no cycle exists.
 *
 *
 *  Because we cannot modify the internal `graph_t' representation of in-degree
 *  we will maintain that information in our own data structure.
 *
 *  Observations: If there are no nodes in our graph with in-degree of 0, then
 *  the graph must be cyclical.
 */
  int
graph_has_cycle( graph_t * g )
{
  assert( g != NULL );

  unsigned long visits = 0;
  int i = -1;
  const int n = graph_vertex_count( g );
  assert( 0 < n );

  struct dins_q_s dq;

  dq.dins = calloc( (size_t) n, sizeof( int ) );
  assert( dq.dins != NULL );

  dq.q = q_create( sizeof( int ) );
  assert( dq.q != NULL );

  for ( i = 0; i < n; i++  )
    {
      dq.dins[i] = graph_in_degree( g, i );
      if ( dq.dins[i] == 0 )
        {
          pq_insert( dq.q, & i );
        }
    }

  while( ! pq_is_empty( dq.q ) )
    {
      i = -1;
      pq_delete_min( dq.q, & i );
      assert( 0 <= i );
      visits++;
      /* Foreach successor decrement in-degree. */
      graph_foreach( g, i, visit_vertex_detect, & dq );
    }

  /* Cleanup */
  free( dq.dins );
  dq.dins = NULL;
  pq_destroy( dq.q );
  dq.q = NULL;

  return visits != ( (size_t) n );
}


struct dins_g_s {
  int     * dins;
  graph_t * g_new;
};

  static void
visit_clone_cycles( graph_t * g, int source, int sink, void * dg_data )
{
  assert( g != NULL );
  assert( dg_data != NULL );
  struct dins_g_s * dg = (struct dins_g_s *) dg_data;
  if ( ( 0 < dg->dins[source] ) && ( 0 < dg->dins[sink] ) )
    {
      graph_add_edge( dg->g_new, source, sink );
    }
}


  int
graph_print_cycles( graph_t * g )
{
  assert( g != NULL );

  unsigned long visits = 0;
  int i = -1;
  const int n = graph_vertex_count( g );
  assert( 0 < n );

  struct dins_q_s dq;
  struct dins_g_s dg;

  dq.dins = calloc( (size_t) n, sizeof( int ) );
  assert( dq.dins != NULL );

  dq.q = q_create( sizeof( int ) );
  assert( dq.q != NULL );

  for ( i = 0; i < n; i++  )
    {
      dq.dins[i] = graph_in_degree( g, i );
      if ( dq.dins[i] == 0 )
        {
          pq_insert( dq.q, & i );
        }
    }

  while( ! pq_is_empty( dq.q ) )
    {
      i = -1;
      pq_delete_min( dq.q, & i );
      assert( 0 <= i );
      visits++;
      /* Foreach successor decrement in-degree. */
      graph_foreach( g, i, visit_vertex_detect, & dq );
    }

  /* Create a new graph containing only cyclical vertices. */
  dg.dins = dq.dins;
  dg.g_new = graph_create( n );
  assert( dg.g_new != NULL );
  for ( i = 0; i < n; i++ )
    {
      if ( 0 < dq.dins[i] )
        {
          graph_foreach( g, i, visit_clone_cycles, & dg );
        }
    }

  //int * groups  = mark_subgraph_groups( dg.g_new );
  /* FIXME */
  int * groups = graph_tarjan( dg.g_new );
  assert( groups != NULL );
  int   ngroups = graph_count_subgraphs( dg.g_new );
  int * group_pop = calloc( (size_t) ngroups, sizeof( int ) );
  assert( group_pop != NULL );

  for ( i = 0; i < n; i++ )
    {
      printf( "%d: %d\n", i, groups[i] );
    }
  putchar( '\n' );

  for ( i = 0; i < n; i++ )
    {
      group_pop[groups[i]]++;
    }

  for ( i = 0; i < ngroups; i++ )
    {
      /* Skip singleton groups since they are not cyclcical. */
      if ( group_pop[i] <= 1 )
        {
          continue;
        }

      char sep = 0;
      for ( int j = 0; j < n; j++ )
        {
          /* Not in the current group. */
          if ( groups[j] != i )
            {
              continue;
            }

          if ( sep )
            {
              putchar( ' ' );
            }
          else
            {
              sep = 1;
            }

          printf( "%d", j );
        }

      putchar( '\n' );
    }

  /* Cleanup */
  free( groups );
  groups = NULL;
  free( group_pop );
  group_pop = NULL;
  free( dq.dins );
  dq.dins = NULL;
  dg.dins = NULL;
  pq_destroy( dq.q );
  dq.q = NULL;
  graph_destroy( dg.g_new );
  dg.g_new = NULL;

  return visits != ( (size_t) n );
}
