
#include <stdlib.h>
#include <stdio.h>
#include "hgraph.h"
#include "gsearch.h"
#include "graph_util.h"
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <search.h>
#include <errno.h>


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

struct htarjan_info {
  char                ** keys;
  int                 *  scss;
  struct hsearch_data *  htab;
  int                    nkeys;
  int                    nscss;
};


  static struct htarjan_info *
htarjan_info_create( hgraph_t * h )
{
  assert( h != NULL );

  struct htarjan_info * hi = malloc( sizeof( struct htarjan_info ) );
  assert( hi != NULL );

  int rsl = 0;
  ENTRY e = { NULL, NULL };
  ENTRY * r = NULL;

  hi->keys = hgraph_get_keys( h );
  hi->nkeys = hgraph_vertex_count( h );

  /* The first member of `hgraph_t' is `graph_t', but I admit this is kind
   * of evil. */
  hi->scss = tarjan( (graph_t *) h );
  hi->nscss = ilist_max( hi->scss, hi->nkeys ) + 1;

  hi->htab = calloc( 1, sizeof( struct hsearch_data ) );
  assert( hi->htab != NULL );
  rsl = hcreate_r( (size_t) hi->nkeys, hi->htab );
  assert( rsl != 0 );
  rsl = 0;

  for ( int i = 0; i < hi->nkeys; i++ )
    {
      assert( hi->keys[i] != NULL );
      e.key = hi->keys[i];
      /* Casting to long first helps to ensure that the size difference is
       * handled gracefully. */
      e.data = (void *) ( (long) hi->scss[i] );

      rsl = hsearch_r( e, ENTER, & r, hi->htab );
      assert( rsl != 0 );
      rsl = 0;
      assert( r != NULL );
      r      = NULL;
      e.key  = NULL;
      e.data = NULL;
    }

  return hi;
}


  static void
htarjan_info_destroy( struct htarjan_info * hi )
{
  assert( hi != NULL );
  for ( int i = 0; i < hi->nkeys; i++ )
    {
      free( hi->keys[i] );
      hi->keys = NULL;
    }
  free( hi->keys );
  hi->keys = NULL;
  free( hi->scss );
  hi->scss = NULL;
  hdestroy_r( hi->htab );
  hi->htab = NULL;
  free( hi );
  hi = NULL;
}


  static int
htarjan_get_scss( struct htarjan_info * hi, const char * u )
{
  assert( hi != NULL );
  assert( u != NULL );
  ENTRY e = { u, NULL };
  ENTRY * r = NULL;
  int rsl = hsearch_r( e, FIND, & r, hi->htab );
  assert( rsl == 0 );
  assert( r != NULL );
  return (int) ( (long) r->data );
}


/* -------------------------------------------------------------------------- */

typedef void (*htj_scss_foreach_fn)( hgraph_t *,
                                     int        scss,
                                     char     * u,
                                     void     * data
                                   );

  static void
htarjan_scss_all_foreach( hgraph_t            * h,
                          htj_scss_foreach_fn   f,
                          struct htarjan_info * hi,
                          void                * data
                        )
{
  assert( hi != NULL );
  for ( int s = 0; s < hi->nscss; s++ )
    {
      for ( int i = 0; i < hi->nkeys; i++ )
        {
          if ( hi->scss[i] == s )
            {
              f( h, s, hi->keys[i], data );
            }
        }
    }
}


  static void
htarjan_scss_foreach( hgraph_t            * h,
                      int                   scss,
                      htj_scss_foreach_fn   f,
                      struct htarjan_info * hi,
                      void                * data
                     )
{
  assert( hi != NULL );
  for ( int i = 0; i < hi->nkeys; i++ )
    {
      if ( hi->scss[i] == scss )
        {
            f( h, scss, hi->keys[i], data );
        }
    }
}


/* -------------------------------------------------------------------------- */

struct htj_fe_cloner {
  struct htarjan_info * hi;
  graph_t             * g;
};


  static void
htj_clone_fe_edge( hgraph_t * h, char * u, char * v, void * data )
{
  struct htj_fe_cloner * fec = data;
  int uscss = htarjan_get_scss( fec->hi, u );
  int vscss = htarjan_get_scss( fec->hi, v );
  if ( uscss != vscss )
    {
      graph_add_edge_safe( fec->g, uscss, vscss );
    }
}


  static graph_t *
htarjan_scss_graph_create( hgraph_t * h, struct htarjan_info * hi )
{
  assert( h != NULL );
  assert( hi != NULL );
  graph_t * g = graph_create( hi->nscss );
  assert( g != NULL );
  struct htj_fe_cloner fec = { hi, g };
  hgraph_foreach_edge( h, htj_clone_fe_edge, (void *) & fec );
  return g;
}



/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  char     * pathname = NULL;
  hgraph_t * h        = NULL;
  graph_t  * g        = NULL;
  struct htarjan_info * hi = NULL;

  if ( argc <= 1 )
    {
      fprintf( stderr, "You must provide a filename with edge pairs!\n" );
      return EXIT_FAILURE;
    }
  pathname = argv[1];

  h = hgraph_fread( pathname );
  hi = htarjan_info_create( h );
  g = htarjan_scss_graph_create( h, hi );

  /* Cleanup */
  htarjan_info_destroy( hi );
  hi = NULL;
  hgraph_destroy( h );
  h = NULL;
  graph_destroy( g );
  g = NULL;

  return EXIT_SUCCESS;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
