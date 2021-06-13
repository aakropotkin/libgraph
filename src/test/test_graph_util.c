
#include <stdlib.h>
#include "graph_util.h"
#include "graph_test.h"

/* ========================================================================== */

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

  int
test_hgraph_fprint_dot( void )
{
  hgraph_t * h = hgraph_create( KEYS, NKEYS );

  TEST_ASSERT( h != NULL );
  TEST_ASSERT( hgraph_vertex_count( h ) == NKEYS );

  hgraph_add_edge( h, KEYS[0], KEYS[1] );
  hgraph_add_edge( h, KEYS[1], KEYS[0] );
  hgraph_add_edge( h, KEYS[2], KEYS[3] );
  hgraph_add_edge( h, KEYS[3], KEYS[4] );
  hgraph_add_edge( h, KEYS[4], KEYS[2] );

  hgraph_fprint_dot( stdout, h );

  /* Free it */
  hgraph_destroy( h );
  h = NULL;

  return 1;
}


/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  int rsl = 1;

  RUN_TEST( rsl, test_hgraph_fprint_dot );

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* -------------------------------------------------------------------------- */



/* ========================================================================== */
