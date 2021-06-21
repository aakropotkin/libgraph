
#include "graph_util.h"
#include <assert.h>

/* ========================================================================== */


/* -------------------------------------------------------------------------- */

  static void
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


  static void
hgraph_edge_fprint( hgraph_t * h, char * u, char * v, void * data )
{
  FILE * stream = data;
  fprintf( stream, "  %s %s\n", u, v );
}


  void
hgraph_fprint( FILE * stream, hgraph_t * h )
{
  assert( stream != NULL );
  assert( h != NULL );
  hgraph_foreach_edge( h, hgraph_edge_fprint, (void *) stream );
}


/* -------------------------------------------------------------------------- */

  void
hgraph_write( const char * pathname, hgraph_t * h )
{
  assert( pathname != NULL );
  assert( h != NULL );
  FILE * stream = fopen( pathname, "w" );
  int rsl = 0;
  assert( stream != NULL );
  hgraph_fprint( stream, h );
  rsl = fclose( stream );
  assert( rsl == 0 );
}


  void
hgraph_write_dot( const char * pathname, hgraph_t * h )
{
  assert( pathname != NULL );
  assert( h != NULL );
  FILE * stream = fopen( pathname, "w" );
  int rsl = 0;
  assert( stream != NULL );
  hgraph_write_dot( pathname, h );
  rsl = fclose( stream );
  assert( rsl == 0 );
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
