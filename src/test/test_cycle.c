#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "cycle.h"


/**
 * Has Cycle:
 * A -> B
 * B -> C
 * C -> A
 */
  int
test_graph_has_cycle_1( void )
{
  graph_t * g = graph_create( 3 );
  int       has_cycle = 0;
  if ( g == NULL ) return EXIT_FAILURE;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );
  graph_add_edge( g, 2, 0 );

  has_cycle = graph_has_cycle( g );
  graph_destroy( g );
  g = NULL;

  return has_cycle;
}


/**
 * No Cycle:
 * A -> B
 * B -> C
 * A -> C
 */
  int
test_graph_has_cycle_2( void )
{
  graph_t * g         = graph_create( 3 );
  int       has_cycle = 0;
  if ( g == NULL ) return EXIT_FAILURE;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );
  graph_add_edge( g, 0, 2 );

  has_cycle = graph_has_cycle( g );
  graph_destroy( g );
  g = NULL;

  return ! has_cycle;
}


/**
 * Has two cycles, but `A' is not in that cycle:
 *
 * A -> B
 * B -> C
 * B -> D
 * C -> D
 * D -> B
 *
 * F -> A
 * E -> F
 * F -> E
 *
 * Notably `A' does not cause the two cycles to become one group.
 *
 * FIXME: `F -> A' does incorrectly cause the groups to merge because it causes
 *        all vertices to have `0 < in-degree'.
 *        Ideally I want these two cycles to still be output distinctly because
 *        no members of the `B C D' cycle are able to reach `E' or `F'.
 */
  int
test_graph_print_cycles_1( void )
{
  graph_t * g = graph_create( 6 );
  int       has_cycle = 0;
  if ( g == NULL ) return EXIT_FAILURE;

  graph_add_edge( g, 0, 1 );
  graph_add_edge( g, 1, 2 );
  graph_add_edge( g, 1, 3 );
  graph_add_edge( g, 2, 3 );
  graph_add_edge( g, 3, 1 );

  graph_add_edge( g, 4, 5 );
  graph_add_edge( g, 5, 4 );
  graph_add_edge( g, 5, 0 );

  has_cycle = graph_print_cycles( g );
  graph_destroy( g );
  g = NULL;

  return has_cycle;
}


  int
main( int argc, char * argv[], char ** envp )
{
  int rsl = 1;

  rsl &= test_graph_has_cycle_1();
  rsl &= test_graph_has_cycle_2();
  //rsl &= test_graph_print_cycles_1();

  return rsl ? EXIT_SUCCESS : EXIT_FAILURE;
}
