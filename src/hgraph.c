
#define _GNU_SOURCE
#include "hgraph.h"
#include <search.h>
#include <stdlib.h>
#include <assert.h>


/* ========================================================================== */

struct hgraph_s {
  struct graph_s      *  g;
  struct hsearch_data *  htab;
  char                ** keys;
};
typedef struct hgraph_s  hgraph_t;


/* -------------------------------------------------------------------------- */

  hgraph_t *
hgraph_create( char * keys, int n )
{
  assert( 0 < n );
  assert( keys != NULL );

  int rsl = 0;
  ENTRY   e;
  ENTRY * r = NULL;
  hgraph_t * h = malloc( sizeof( hgraph_t ) );
  assert( h != NULL );
  h->g = NULL;
  h->htab = NULL;
  h->keys = NULL;

  h->g = graph_create( n );
  assert( h->g != NULL );

  h->htab = calloc( 1, sizeof( struct hsearch_data ) );
  assert( h->htab != NULL );
  rsl = hcreate_r( (size_t) n, h->htab );
  assert( rsl != 0 );

  h->keys = calloc( n, sizeof( char * ) );
  assert( h->keys != NULL );

  return h;
}



/* -------------------------------------------------------------------------- */



/* ========================================================================== */
