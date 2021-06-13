
#include "graph_util.h"
#include <assert.h>

/* ========================================================================== */


/* -------------------------------------------------------------------------- */

  void
hgraph_edge_fprint_dot( hgraph_t * h, char * u, char * v, void * data )
{
  FILE * stream = data;
  fprintf( stream, "  %s -> %s;\n", u, v );
}

  void
hgraph_fprint_dot( FILE * stream, hgraph_t * h )
{
  assert( stream != NULL );
  assert( h != NULL );
  fputs( "digraph {\n", stream );
  hgraph_foreach_edge( h, hgraph_edge_fprint_dot, (void *) stream );
  fputs( "};\n", stream );
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
