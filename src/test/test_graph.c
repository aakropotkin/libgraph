
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "graph_test.h"


/* ========================================================================== */

#define TEST_SIZE (37)


/** never call this */
  static void
match_sink( graph_t * g, int source, int sink, void * data )
{
  assert( data != NULL );
  assert( sink == *( (int *) data ) );
}


/* -------------------------------------------------------------------------- */

  static int
test_graph_clone( void )
{
  graph_t * g = graph_create( 5 );
  TEST_ASSERT( g != NULL );

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 0, 2 );
  graph_add_edge( g, 1, 2 );
  graph_add_edge( g, 3, 4 );
  graph_add_edge( g, 4, 3 );

  graph_t * g_new = graph_clone( g );
  TEST_ASSERT( g_new != NULL );

  TEST_ASSERT( graph_has_edge( g_new, 0, 1 ) );
  TEST_ASSERT( graph_has_edge( g_new, 0, 2 ) );
  TEST_ASSERT( graph_has_edge( g_new, 1, 2 ) );
  TEST_ASSERT( graph_has_edge( g_new, 3, 4 ) );
  TEST_ASSERT( graph_has_edge( g_new, 4, 3 ) );

  graph_destroy( g );
  g = NULL;

  TEST_ASSERT( graph_has_edge( g_new, 0, 1 ) );
  TEST_ASSERT( graph_has_edge( g_new, 0, 2 ) );
  TEST_ASSERT( graph_has_edge( g_new, 1, 2 ) );
  TEST_ASSERT( graph_has_edge( g_new, 3, 4 ) );
  TEST_ASSERT( graph_has_edge( g_new, 4, 3 ) );

  graph_destroy( g_new );
  g_new = NULL;

  return 1;
}


/* -------------------------------------------------------------------------- */

  static int
test_graph_1( void )
{
  graph_t * g = graph_create( TEST_SIZE );

  TEST_ASSERT( g != NULL );
  TEST_ASSERT( graph_vertex_count( g ) == TEST_SIZE );

  /* Check it's empty. */
  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      for ( int j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == 0 );
        }
    }

  /* Check it's empty again, to ensure `graph_has_edge' didn't modify `g'. */
  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree( g, i ) == 0 );
      graph_foreach( g, i, match_sink, 0 );
    }

  /* Check edge count. */
  TEST_ASSERT( graph_edge_count( g ) == 0 );

  /* Fill in the diagonal. */
  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      graph_add_edge( g, i, i );
    }

  /* Check */
  TEST_ASSERT( graph_edge_count( g ) == TEST_SIZE );

  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      for ( int j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == ( i == j ) );
        }
    }

  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree( g , i ) == 1 );
      graph_foreach( g, i, match_sink, & i );
    }

  /* Fill in all the entries */
  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      for ( int j = 0; j < TEST_SIZE; j++ )
        {
          if ( i != j )
            {
              graph_add_edge( g, i, j );
            }
        }
    }

  /* Test they are all there */
  TEST_ASSERT( graph_edge_count( g ) == ( TEST_SIZE * TEST_SIZE ) );

  for ( int i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree( g, i ) == TEST_SIZE );
      for ( int j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == 1 );
        }
    }

  /* Free it */
  graph_destroy( g );
  g = NULL;

  return 1;
}


/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  int rsl = 1;

  RUN_TEST( rsl, test_graph_1 );
  RUN_TEST( rsl, test_graph_clone );

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
