#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "search.h"
#include "graph_test.h"


#define TEST_SIZE ( 199137 )


/* 1 -> Pass, 0 -> Fail */
  static int
test_dfs( void )
{
  graph_t            * g = graph_create( TEST_SIZE );
  struct search_info * s = NULL;
  int                  i = -1;

  /* Put all nodes on a long path and all nodes as children of `0' except for
   * node `n - 1' */
  for( i = 0; i < TEST_SIZE - 2; i++ )
    {
      graph_add_edge( g, i, i );          /* Red herring */
      graph_add_edge( g, i, ( i + 1 ) );  /* For `DFS' */
    };

  /* Quick check to make sure we put in the edges we thought we did */
  TEST_ASSERT( graph_vertex_count( g ) == TEST_SIZE );
  TEST_ASSERT( graph_edge_count( g ) == ( ( TEST_SIZE - 2 ) * 2 ) );

  /* Do `dfs' starting from `0' */
  s = search_info_create( g );
  dfs( s, 0 );

  /* What did we learn? */
  TEST_ASSERT( s->reached == ( TEST_SIZE - 1 ) );
  for( i = 0; i < TEST_SIZE - 1; i++ )
    {
      if ( i == 0 )
        {
          TEST_ASSERT( s->parent[i] == i );
        }
      else
        {
          TEST_ASSERT( s->parent[i] == ( i - 1 ) );
        }
      TEST_ASSERT( s->preorder[i] == i );
      TEST_ASSERT( s->time[i] == i );
      TEST_ASSERT( s->depth[i] == i );
    }

  /* Now do more dfs from `n - 1' */
  dfs( s, TEST_SIZE - 1 );

  TEST_ASSERT( s->reached == TEST_SIZE );
  for( i = 0; i < TEST_SIZE; i++ )
    {
      if ( ( i == 0 ) || ( i == TEST_SIZE - 1 ) )
        {
          TEST_ASSERT( s->parent[i] == i );
        }
      else
        {
          TEST_ASSERT( s->parent[i] == ( i - 1 ) );
        }
      TEST_ASSERT( s->preorder[i] == i );
      TEST_ASSERT( s->time[i] == i );
      if ( i == ( TEST_SIZE - 1 ) )
        {
          TEST_ASSERT( s->depth[i] == 0 );
        }
      else
        {
          TEST_ASSERT( s->depth[i] == i );
        }
    }

  search_info_destroy( s );
  s = NULL;
  graph_destroy( g );
  g = NULL;

  return 1;
}


  static int
test_bfs( void )
{
  graph_t            * g = NULL;
  struct search_info * s = NULL;
  int                  i = -1;

  g = graph_create( TEST_SIZE );

  for ( i = 1; i < TEST_SIZE - 1; i++ )
    {
      graph_add_edge( g, i, i );
      graph_add_edge( g, 0, i );
    }

  graph_add_edge(g, 1, TEST_SIZE - 1);

  TEST_ASSERT( graph_edge_count( g ) == ( 2 * ( TEST_SIZE - 2 ) + 1 ) );

  s = search_info_create( g );
  bfs( s, 0 );

  /* What did we learn? */
  TEST_ASSERT( s->reached == TEST_SIZE );
  for ( i = 0; i < TEST_SIZE - 1; i++ )
    {
      TEST_ASSERT( s->parent[i] == 0 );
      TEST_ASSERT( s->preorder[s->time[i]] == i );
      if ( i == 0 )
        {
          TEST_ASSERT( s->depth[i] == 0 );
        }
      else
        {
          TEST_ASSERT( s->depth[i] == 1 );
        }
    }

  /* `n - 1' should be hit last */
  TEST_ASSERT( s->parent[TEST_SIZE - 1] == 1 );
  TEST_ASSERT( s->preorder[TEST_SIZE - 1] == ( TEST_SIZE - 1 ) );
  TEST_ASSERT( s->time[TEST_SIZE - 1] == ( TEST_SIZE - 1 ) );
  TEST_ASSERT( s->depth[TEST_SIZE - 1] == 2 );

  search_info_destroy( s );
  s = NULL;
  graph_destroy( g );
  g = NULL;

  return 1;
}


/**
 * This shows useful properties about how `parents' are marked
 * during searches.
 * It is a precondition to `mark_reachable' working correctly.
 */
  static int
test_reachable( void )
{
  graph_t * g = graph_create( 5 );
  struct search_info * s = NULL;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  s = search_info_create( g );
  bfs( s, 0 );

  /* Vertices `0', `1', and `2' should be "reachable" from `0',
   * while vertices `3', and `4' are members of a separate "sub-graph" making
   * them "unreachable" */
  TEST_ASSERT( s->parent[0] == 0 );
  TEST_ASSERT( s->parent[1] == 0 );
  TEST_ASSERT( s->parent[2] == 1 );

  TEST_ASSERT( s->parent[3] == SEARCH_INFO_NULL );
  TEST_ASSERT( s->parent[4] == SEARCH_INFO_NULL );

  search_info_destroy( s );
  s = NULL;
  graph_destroy( g );
  g = NULL;

  return 1;
}


  static int
test_mark_reachable( void )
{
  graph_t * g = graph_create( 5 );
  char    * r = NULL;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  r = mark_reachable( g, 0 );

  TEST_ASSERT( r[0] );
  TEST_ASSERT( r[1] );
  TEST_ASSERT( r[2] );
  TEST_ASSERT( ! r[3] );
  TEST_ASSERT( ! r[4] );

  free( r );
  r = NULL;

  r = mark_reachable( g, 1 );

  TEST_ASSERT( ! r[0] );
  TEST_ASSERT( r[1] );
  TEST_ASSERT( r[2] );
  TEST_ASSERT( ! r[3] );
  TEST_ASSERT( ! r[4] );

  free( r );
  r = NULL;

  r = mark_reachable( g, 3 );

  TEST_ASSERT( ! r[0] );
  TEST_ASSERT( ! r[1] );
  TEST_ASSERT( ! r[2] );
  TEST_ASSERT( r[3] );
  TEST_ASSERT( r[4] );

  free( r );
  r = NULL;
  graph_destroy( g );
  g = NULL;

  return 1;
}


  static int
test_mark_same_subgraph( void )
{
  graph_t * g = graph_create( 5 );
  char    * r = NULL;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  r = mark_same_subgraph( g, 0 );

  TEST_ASSERT( r[0] );
  TEST_ASSERT( r[1] );
  TEST_ASSERT( r[2] );
  TEST_ASSERT( ! r[3] );
  TEST_ASSERT( ! r[4] );

  free( r );
  r = NULL;

  r = mark_same_subgraph( g, 1 );

  TEST_ASSERT( r[0] );
  TEST_ASSERT( r[1] );
  TEST_ASSERT( r[2] );
  TEST_ASSERT( ! r[3] );
  TEST_ASSERT( ! r[4] );

  free( r );
  r = NULL;

  r = mark_same_subgraph( g, 3 );

  TEST_ASSERT( ! r[0] );
  TEST_ASSERT( ! r[1] );
  TEST_ASSERT( ! r[2] );
  TEST_ASSERT( r[3] );
  TEST_ASSERT( r[4] );

  free( r );
  r = NULL;
  graph_destroy( g );
  g = NULL;

  return 1;
}


  static int
test_count_subgraphs( void )
{
  graph_t * g             = graph_create( 5 );
  int       num_subgraphs = -1;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  num_subgraphs = count_subgraphs( g );

  TEST_ASSERT( num_subgraphs == 2 );

  graph_destroy( g );
  g = NULL;

  return 1;
}


  static int
test_mark_subgraph_groups( void )
{
  graph_t * g      = graph_create( 5 );
  int     * groups = NULL;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  groups = mark_subgraph_groups( g );

  TEST_ASSERT( groups[0] == 0 );
  TEST_ASSERT( groups[1] == 0 );
  TEST_ASSERT( groups[2] == 0 );
  TEST_ASSERT( groups[3] == 1 );
  TEST_ASSERT( groups[4] == 1 );

  graph_destroy( g );
  g = NULL;
  free( groups );
  groups = NULL;

  return 1;
}


  static int
test_split_subgraphs( void )
{
  graph_t *  g             = graph_create( 5 );
  graph_t ** subgraphs     = NULL;
  int        num_subgraphs = -1;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );

  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  subgraphs = split_subgraphs( g, & num_subgraphs );

  TEST_ASSERT( num_subgraphs == 2 );

  /* Remeber, only all vertices still exist, but only relevant edges
   * are cloned */
  TEST_ASSERT( graph_vertex_count( subgraphs[0] ) == 5 );
  TEST_ASSERT( graph_vertex_count( subgraphs[1] ) == 5 );

  TEST_ASSERT( graph_edge_count( subgraphs[0] ) == 2 );
  TEST_ASSERT( graph_edge_count( subgraphs[1] ) == 2 );

  TEST_ASSERT( ! graph_has_edge( subgraphs[0], 3, 4 ) );
  TEST_ASSERT( ! graph_has_edge( subgraphs[0], 4, 3 ) );
  TEST_ASSERT( ! graph_has_edge( subgraphs[1], 0, 1 ) );
  TEST_ASSERT( ! graph_has_edge( subgraphs[1], 1, 2 ) );

  graph_destroy( g );
  g = NULL;

  if ( 1 < num_subgraphs )
    {
      for ( int i = 0; i < num_subgraphs; i++ )
        {
          graph_destroy( subgraphs[i] );
          subgraphs[i] = NULL;
        }
      free( subgraphs );
      subgraphs = NULL;
    }

  return 1;
}


  static int
test_tarjan( void )
{
  graph_t * g = graph_create( 7 );
  int     * t = NULL;
  const int n = graph_vertex_count( g );

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );
  graph_add_edge( g, 2, 3 );
  graph_add_edge( g, 3, 1 );

  graph_add_edge( g, 4, 5 );
  graph_add_edge( g, 5, 4 );

  graph_add_edge( g, 6, 0 );

  t = tarjan( g );

  /* Four groups exist of `[0], [1, 2, 3]', `[4, 5]', and `[6]' */
  TEST_ASSERT( t[0] != t[1] );
  TEST_ASSERT( t[0] != t[4] );
  TEST_ASSERT( t[0] != t[6] );

  TEST_ASSERT( t[1] == t[2] );
  TEST_ASSERT( t[2] == t[3] );
  TEST_ASSERT( t[3] != t[4] );
  TEST_ASSERT( t[3] != t[6] );

  TEST_ASSERT( t[4] == t[5] );
  TEST_ASSERT( t[5] != t[6] );

  graph_destroy( g );
  g = NULL;

  free( t );
  t = NULL;

  return 1;
}


  int
main( int argc, char ** argv )
{
  int rsl = 1;

  RUN_TEST( rsl, test_dfs );
  RUN_TEST( rsl, test_bfs );
  RUN_TEST( rsl, test_reachable );
  RUN_TEST( rsl, test_mark_reachable );
  RUN_TEST( rsl, test_mark_same_subgraph );
  RUN_TEST( rsl, test_mark_subgraph_groups );
  RUN_TEST( rsl, test_count_subgraphs );
  RUN_TEST( rsl, test_split_subgraphs );
  RUN_TEST( rsl, test_tarjan );

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}
