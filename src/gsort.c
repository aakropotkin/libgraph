
#include <stdlib.h>
#include <stdio.h>
#include "hgraph.h"
#include "gsearch.h"
#include "graph_util.h"
#include <limits.h>
#include <assert.h>


/* ========================================================================== */

  static inline int
ilist_max( int arr[], int n )
{
  assert( arr != NULL );
  int m = INT_MIN;
  for ( int i = 0; i < n; i++ )
    {
      if ( m < arr[i] )
        {
          m = arr[i];
        }
    }
  return m;
}


/* -------------------------------------------------------------------------- */

struct hgraph_scss {
  char ** keys;
  int  *  scss;
  int     nkeys;
  int     nscss;
};


  static struct hgraph_scss *
hgraph_tarjan( hgraph_t * h )
{
  assert( h != NULL );
  struct hgraph_scss * hscss = malloc( sizeof( struct hgraph_scss ) );
  assert( hscss != NULL );
  hscss->keys = hgraph_get_keys( h );
  hscss->nkeys = hgraph_vertex_count( h );
  /* The first member of `hgraph_t' is `graph_t', but I admit this is kind
   * of evil. */
  hscss->scss = tarjan( (graph_t *) h );
  hscss->nscss = ilist_max( hscss->scss, hscss->nkeys ) + 1;
  return hscss;
}


/* -------------------------------------------------------------------------- */

int
main( int argc, char * argv[], char ** envp )
{
  char     * pathname = NULL;
  hgraph_t * h        = NULL;
  struct hgraph_scss * hscss = NULL;

  if ( argc <= 1 )
    {
      fprintf( stderr, "You must provide a filename with edge pairs!\n" );
      return EXIT_FAILURE;
    }
  pathname = argv[1];

  h = hgraph_fread( pathname );
  hscss = hgraph_tarjan( h );




  /* Cleanup */
  for ( int i = 0; i < hscss->nkeys; i++ )
    {
      free( hscss->keys[i] );
      hscss->keys = NULL;
    }
  free( hscss->keys );
  hscss->keys = NULL;
  free( hscss->scss );
  hscss->scss = NULL;
  free( hscss );
  hscss = NULL;

  return EXIT_SUCCESS;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
