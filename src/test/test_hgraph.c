
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hgraph.h"
#include "graph_test.h"
#include <string.h>


/* ========================================================================== */

/** never call this */
  static void
match_sink( hgraph_t * h, char * v, char * u, void * data )
{
  assert( data != NULL );
  assert( u != NULL );
  assert( strcmp( u, (char *) data ) == 0 );
}


/* -------------------------------------------------------------------------- */

/* The `0[ARR]' looks wrong but have faith.
 * It takes advantage of a very wonky part of the language spec which states
 * that `0[ARR]' is equivalent to `ARR[0]'/`*ARR' ONLY on arrays, but will
 * fail to compile if `ARR' was declared as a pointer. */
#define ARRAY_SIZE( ARR )                                      \
  ( ( sizeof( ARR ) / sizeof( 0[ARR] ) ) /                     \
    ( (size_t) ( ! ( sizeof( ARR ) % sizeof( 0[ARR] ) ) ) ) )


/* -------------------------------------------------------------------------- */

static char * KEYS[] = { "foo", "bar", "baz", "quux", "stuz" };
static int    NKEYS  = (int) ARRAY_SIZE( KEYS );

  static int
test_hgraph_1( void )
{
  hgraph_t * h = hgraph_create( KEYS, NKEYS );

  TEST_ASSERT( h != NULL );
  TEST_ASSERT( hgraph_vertex_count( h ) == NKEYS );

  /* Check it's empty. */
  for ( int i = 0; i < NKEYS; i++ )
    {
      for ( int j = 0; j < NKEYS; j++ )
        {
          TEST_ASSERT( hgraph_has_edge( h, KEYS[i], KEYS[j] ) == 0 );
        }
    }

  /* Check it's empty again, to ensure `graph_has_edge' didn't modify `g'. */
  for ( int i = 0; i < NKEYS; i++ )
    {
      TEST_ASSERT( hgraph_out_degree( h, KEYS[i] ) == 0 );
      /* The `NULL' check at the top of `match_sink' will crash if
       * `hgraph_foreach' actually invokes `match_sink' on any edge. */
      hgraph_foreach( h, KEYS[i], match_sink, NULL );
    }

  /* Check edge count. */
  TEST_ASSERT( hgraph_edge_count( h ) == 0 );

  /* Fill in the diagonal. */
  for ( int i = 0; i < NKEYS; i++ )
    {
      hgraph_add_edge( h, KEYS[i], KEYS[i] );
    }

  /* Check */
  TEST_ASSERT( hgraph_edge_count( h ) == NKEYS );

  for ( int i = 0; i < NKEYS; i++ )
    {
      for ( int j = 0; j < NKEYS; j++ )
        {
          TEST_ASSERT( hgraph_has_edge( h, KEYS[i], KEYS[j] ) == ( i == j ) );
        }
    }

  for ( int i = 0; i < NKEYS; i++ )
    {
      TEST_ASSERT( hgraph_out_degree( h , KEYS[i] ) == 1 );
      hgraph_foreach( h, KEYS[i], match_sink, KEYS[i] );
    }

  /* Fill in all the entries */
  for ( int i = 0; i < NKEYS; i++ )
    {
      for ( int j = 0; j < NKEYS; j++ )
        {
          if ( i != j )
            {
              hgraph_add_edge( h, KEYS[i], KEYS[j] );
            }
        }
    }

  /* Test they are all there */
  TEST_ASSERT( hgraph_edge_count( h ) == ( NKEYS * NKEYS ) );

  for ( int i = 0; i < NKEYS; i++ )
    {
      TEST_ASSERT( hgraph_out_degree( h, KEYS[i] ) == NKEYS );
      for ( int j = 0; j < NKEYS; j++ )
        {
          TEST_ASSERT( hgraph_has_edge( h, KEYS[i], KEYS[j] ) == 1 );
        }
    }

  /* Free it */
  hgraph_destroy( h );
  h = NULL;

  return 1;
}


/* -------------------------------------------------------------------------- */

  static int
test_hgraph_clone( void )
{
  hgraph_t * h = hgraph_create( KEYS, NKEYS );
  TEST_ASSERT( h != NULL );

  hgraph_add_edge( h, KEYS[0], KEYS[1] );
  hgraph_add_edge( h, KEYS[0], KEYS[2] );
  hgraph_add_edge( h, KEYS[1], KEYS[2] );
  hgraph_add_edge( h, KEYS[3], KEYS[4] );
  hgraph_add_edge( h, KEYS[4], KEYS[3] );

  hgraph_t * h_new = hgraph_clone( h );
  TEST_ASSERT( h_new != NULL );

  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[0], KEYS[1] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[0], KEYS[2] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[1], KEYS[2] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[3], KEYS[4] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[4], KEYS[3] ) );

  hgraph_destroy( h );
  h = NULL;

  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[0], KEYS[1] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[0], KEYS[2] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[1], KEYS[2] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[3], KEYS[4] ) );
  TEST_ASSERT( hgraph_has_edge( h_new, KEYS[4], KEYS[3] ) );

  /* Free it */
  hgraph_destroy( h_new );
  h_new = NULL;

  return 1;
}


/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  int rsl = 1;

  RUN_TEST( rsl, test_hgraph_1 );
  RUN_TEST( rsl, test_hgraph_clone );

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
