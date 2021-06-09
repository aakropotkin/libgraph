
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "graph_test.h"


#define TEST_SIZE (37)


/** never call this */
  static void
match_sink( graph_t * g, int source, int sink, void * data )
{
  assert( data && ( sink == *( (int *) data ) ) );
}


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


  static int
test_graph_1( void )
{
  graph_t * g;
  int i;
  int j;

  g = graph_create( TEST_SIZE );

  TEST_ASSERT( graph_vertex_count( g ) == TEST_SIZE );

  /* check it's empty */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == 0 );
        }
    }

  /* check it's empty again */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree( g, i ) == 0 );
      graph_foreach( g, i, match_sink, 0 );
    }

  /* check edge count */
  TEST_ASSERT( graph_edge_count( g ) == 0 );

  /* fill in the diagonal */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      graph_add_edge( g, i, i );
    }

  /* check */
  TEST_ASSERT( graph_edge_count( g ) == TEST_SIZE );

  for ( i = 0; i < TEST_SIZE; i++ )
    {
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == ( i == j ) );
        }
    }

  for (i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree(g , i ) == 1 );
      graph_foreach( g, i, match_sink, & i );
    }


  /* fill in all the entries */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          if ( i != j )
            {
              graph_add_edge( g, i, j );
            }
        }
    }

  /* test they are all there */
  TEST_ASSERT( graph_edge_count( g ) == ( TEST_SIZE * TEST_SIZE ) );

  for ( i = 0; i < TEST_SIZE; i++ )
    {
      TEST_ASSERT( graph_out_degree( g, i ) == TEST_SIZE );
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          TEST_ASSERT( graph_has_edge( g, i, j ) == 1 );
        }
    }

  /* free it */
  graph_destroy( g );

  return 1;
}


  int
main( int argc, char * argv[], char ** envp )
{
  int rsl = 1;

  RUN_TEST( rsl, test_graph_1 );
  RUN_TEST( rsl, test_graph_clone );

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}
