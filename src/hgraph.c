
#include <errno.h>
#include <search.h>
#include "hgraph.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


/* ========================================================================== */

struct hgraph_s {
  struct graph_s      *  g;
  struct hsearch_data *  htab;
  char                ** keys;
};
typedef struct hgraph_s  hgraph_t;


/* -------------------------------------------------------------------------- */

  hgraph_t *
hgraph_create( char ** keys, int nkeys )
{
  assert( 0 < nkeys );
  assert( keys != NULL );

  ENTRY      e   = { NULL, NULL };
  ENTRY    * r   = NULL;
  hgraph_t * h   = malloc( sizeof( hgraph_t ) );
  int        rsl = 0;
  int        n   = 0;  /* Number of nodes, since there may be repeated keys */
  assert( h != NULL );
  h->g    = NULL;
  h->htab = NULL;
  h->keys = NULL;

  h->htab = calloc( 1, sizeof( struct hsearch_data ) );
  assert( h->htab != NULL );

  /* Duplicate keys cause waste, but whatever. */
  rsl = hcreate_r( (size_t) nkeys, h->htab );
  assert( rsl != 0 );
  rsl = 0;

  h->keys = calloc( nkeys, sizeof( char * ) );
  assert( h->keys != NULL );

  for ( int i = 0; i < nkeys; i++ )
    {
      assert( keys[i] != NULL );
      e.key = keys[i];
      /* Casting to long first helps to ensure that the size difference is
       * handled gracefully. */
      e.data = (void *) ( (long) n );

      /* See if key was already entered */
      rsl = hsearch_r( e, FIND, & r, h->htab );
      if ( rsl == 0 )  /* Key not found, add it */
        {
          assert( errno == ESRCH );  /* Make sure its not a different error */
          h->keys[n] = strdup( keys[i] );  /* Copy the key. */
          e.key = h->keys[n];
          r = NULL;
          rsl = hsearch_r( e, ENTER, & r, h->htab );
          assert( rsl != 0 );
          rsl = 0;
          assert( r != NULL );
          n++;
        }
      else  /* Key found, skip it */
        {
          assert( r != NULL );
        }
      r      = NULL;
      e.key  = NULL;
      e.data = NULL;
    }

  /* Shrink `keys' to correct size */
  if ( n != nkeys )
    {
      h->keys = realloc( h->keys, sizeof( char * ) * n );
      assert( h->keys != NULL );
    }
  h->g = graph_create( n );
  assert( h->g != NULL );

  return h;
}


/* -------------------------------------------------------------------------- */

  hgraph_t *
hgraph_clone( hgraph_t * h )
{
  assert( h != NULL );
  hgraph_t * h_new = NULL;
  int        n     = hgraph_vertex_count( h );

  h_new = hgraph_create( h->keys, n );
  assert( h_new != NULL );

  h_new->g = graph_clone( h->g );
  assert( h_new->g != NULL );

  return h_new;
}


/* -------------------------------------------------------------------------- */

  void
hgraph_destroy( hgraph_t * h )
{
  if ( h == NULL )
    {
      return;
    }
  int n = hgraph_vertex_count( h );

  for ( int i = 0; i < n; i++ )
    {
      free( h->keys[i] );
      h->keys[i] = NULL;
    }
  free( h->keys );
  h->keys = NULL;

  hdestroy_r( h->htab );
  h->htab = NULL;

  graph_destroy( h->g );
  h->g = NULL;

  free( h );
  h = NULL;
}


/* -------------------------------------------------------------------------- */

  static inline int
hlook( hgraph_t * h, char * key )
{
  assert( h != NULL );
  assert( key != NULL );
  ENTRY   e   = { key, NULL };
  ENTRY * r   = NULL;
  int     rsl = hsearch_r( e, FIND, & r, h->htab );
  assert( rsl != 0 );
  assert( r != NULL );
  rsl = (int) ( (long) r->data );
  return rsl;
}


/* -------------------------------------------------------------------------- */

  void
hgraph_add_edge( hgraph_t * h, char * u, char * v )
{
  graph_add_edge( h->g, hlook( h, u ), hlook( h, v ) );
}

  void
hgraph_add_weighted_edge( hgraph_t * h, char * u, char * v, int wt )
{
  graph_add_weighted_edge( h->g, hlook( h, u ), hlook( h, v ), wt );
}

  void
hgraph_remove_edge( hgraph_t * h, char * u, char * v )
{
  graph_remove_edge( h->g, hlook( h, u ), hlook( h, v ) );
}


/* -------------------------------------------------------------------------- */

  int
hgraph_vertex_count( hgraph_t * h )
{
  assert( h != NULL );
  return graph_vertex_count( h->g );
}

  int
hgraph_edge_count( hgraph_t * h )
{
  assert( h != NULL );
  return graph_edge_count( h->g );
}


/* -------------------------------------------------------------------------- */

  int
hgraph_out_degree( hgraph_t * h, char * u )
{
  assert( h != NULL );
  assert( u != NULL );
  return graph_out_degree( h->g, hlook( h, u ) );
}

  int
hgraph_in_degree( hgraph_t * h, char * u )
{
  assert( h != NULL );
  assert( u != NULL );
  return graph_in_degree( h->g, hlook( h, u ) );
}

  int
hgraph_edge_weight( hgraph_t * h, char * u, char * v )
{
  assert( h != NULL );
  assert( u != NULL );
  assert( v != NULL );
  return graph_edge_weight( h->g, hlook( h, u ), hlook( h, v ) );
}


/* -------------------------------------------------------------------------- */

  int
hgraph_has_edge( hgraph_t * h, char * u, char * v )
{
  assert( h != NULL );
  assert( u != NULL );
  assert( v != NULL );
  return graph_has_edge( h->g, hlook( h, u ), hlook( h, v ) );
}


/* -------------------------------------------------------------------------- */

struct hg_fe_wrap {
  hgraph_t          * h;
  hgraph_foreach_fn   f;
  void              * d;  /* "real" `data' */
};

  static void
hg_foreach_wrap( graph_t * g, int u, int v, void * data )
{
  struct hg_fe_wrap * k = data;
  k->f( k->h, k->h->keys[u], k->h->keys[v], k->d );
}

  void
hgraph_foreach( hgraph_t          * h,
                char              * u,
                hgraph_foreach_fn   f,
                void              * data
              )
{
  assert( h != NULL );

  int n  = hgraph_vertex_count( h );
  int ui = hlook( h, u );
  assert( 0 <= ui );
  assert( ui < n );

  struct hg_fe_wrap k = { h, f, data };
  graph_foreach( h->g, ui, hg_foreach_wrap, & k );
}

  void
hgraph_foreach_edge( hgraph_t          * h,
                     hgraph_foreach_fn   f,
                     void              * data
                   )
{
  assert( h != NULL );
  int n  = hgraph_vertex_count( h );

  struct hg_fe_wrap k = { h, f, data };
  for ( int i = 0; i < n; i++ )
    {
      graph_foreach( h->g, i, hg_foreach_wrap, & k );
    }
}


/* -------------------------------------------------------------------------- */

/* This could probably be a `union' type with `hg_fe_wrap', but I'd rather not
 * risk any wonky undefined-behavior/portability issues that it may cause. */
struct hg_fe_wwrap {
  hgraph_t            * h;
  hgraph_w_foreach_fn   f;
  void                * d;  /* "real" `data' */
};

  static void
hg_foreach_wwrap( graph_t * g, int u, int v, int wt, void * data )
{
  struct hg_fe_wwrap * k = data;
  k->f( k->h, k->h->keys[u], k->h->keys[v], wt, k->d );
}

  void
hgraph_foreach_weighted( hgraph_t            * h,
                         char                * u,
                         hgraph_w_foreach_fn   f,
                         void                * data
                       )
{
  assert( h != NULL );

  int n  = hgraph_vertex_count( h );
  int ui = hlook( h, u );
  assert( 0 <= ui );
  assert( ui < n );

  struct hg_fe_wwrap k = { h, f, data };
  graph_foreach_weighted( h->g, ui, hg_foreach_wwrap, & k );
}

  void
hgraph_w_foreach_edge( hgraph_t            * h,
                       hgraph_w_foreach_fn   f,
                       void                * data
                     )
{
  assert( h != NULL );
  int n  = hgraph_vertex_count( h );

  struct hg_fe_wwrap k = { h, f, data };
  for ( int i = 0; i < n; i++ )
    {
      graph_foreach_weighted( h->g, i, hg_foreach_wwrap, & k );
    }
}


/* -------------------------------------------------------------------------- */

  void
hgraph_foreach_vertex( hgraph_t * h, hgraph_foreach_vert_fn f, void * data )
{
  assert( h != NULL );
  int n = hgraph_vertex_count( h );
  for ( int i = 0; i < n; i++ )
    {
      f( h, (const char *) h->keys[i], data );
    }
}


/* -------------------------------------------------------------------------- */

  char **
hgraph_get_keys( hgraph_t * h )
{
  assert( h != NULL );
  int n = hgraph_vertex_count( h );
  char ** keys = malloc( sizeof( char * ) * n );
  assert( keys != NULL );
  for ( int i = 0; i < n; i++ )
    {
      keys[i] = strdup( h->keys[i] );
      assert( keys[i] != NULL );
    }
  return keys;
}


/* -------------------------------------------------------------------------- */

  static inline char *
strpcbrk( const char * s, const char * reject )
{
  return ( (char *) s ) + strspn( s, reject );
}


/* -------------------------------------------------------------------------- */

enum parse_edges_style {
  PES_SPACE,
  PES_DOT
};

static const char * hgraph_edge_delim     = " \t\n'\"";
static const char * hgraph_edge_delim_dot = " \t\n;:<>-{}[]()='\"";

  static inline const char *
get_pes_delim( enum parse_edges_style style )
{
  switch( style )
    {
      case PES_SPACE:  return hgraph_edge_delim;     break;
      case PES_DOT:    return hgraph_edge_delim_dot; break;
      default:         assert( 0 ); return NULL;     break;
    }
  assert( 0 );
  return NULL;
}

struct edge_pair_s {
  char * u;
  char * v;
};


  static struct edge_pair_s
parse_edge_pair( const char * line, enum parse_edges_style style )
{
  assert( line != NULL );
  struct edge_pair_s e = { NULL, NULL };
  const char * delim = get_pes_delim( style );

  char * i = strpcbrk( line, delim );
  assert( i != NULL );
  size_t len = strcspn( i, delim );

  e.u = strndup( i, len );
  assert( e.u != NULL );

  i = strpcbrk( i + len + 1, delim );
  assert( i != NULL );
  len = strcspn( i, delim );

  e.v = strndup( i, len );
  assert( e.v != NULL );

  return e;
}


/* -------------------------------------------------------------------------- */

  void
print_parsed_edges( const char * const lines[], int nlines )
{
  assert( lines != NULL );
  struct edge_pair_s e = { NULL, NULL };
  for ( int i = 0; i < nlines; i ++ )
    {
      //e = parse_edge_pair( lines[i], PES_SPACE );
      e = parse_edge_pair( lines[i], PES_DOT );
      printf( "( %s, %s )\n", e.u, e.v );
      free( e.u );
      e.u = NULL;
      free( e.v );
      e.v = NULL;
    }
}


/* -------------------------------------------------------------------------- */

  static size_t
fcount_lines( FILE * stream )
{
  size_t n = 0;
  int    c = '\0';
  assert( stream != NULL );

  while ( ( c = getc( stream ) ) != EOF )
    {
      if ( c == '\n' )
        {
          n++;
        }
    }

  rewind( stream );
  return n;
}


/* -------------------------------------------------------------------------- */

struct edge {
  int u;
  int v;
};

  hgraph_t *
hgraph_parse( FILE * stream )
{
  assert( stream != NULL );
  char         * line   = NULL;
  size_t         len    = 0;
  ssize_t        nread  = 0;
  const size_t   nlines = fcount_lines( stream );
  ENTRY          e      = { NULL, NULL };
  ENTRY        * r      = NULL;
  hgraph_t     * h      = malloc( sizeof( hgraph_t ) );
  int            n      = 0;  /* Key count */
  int            rsl    = 0;
  struct edge_pair_s tedge = { NULL, NULL };
  struct edge * iedges = calloc( nlines, sizeof( struct edge ) );

  assert( h != NULL );
  h->g    = NULL;
  h->htab = NULL;
  h->keys = NULL;

  h->htab = calloc( 1, sizeof( struct hsearch_data ) );
  assert( h->htab != NULL );

  /* This is going to be super wasteful on space, but making multiple scans
   * probably isn't worth the time. */
  rsl = hcreate_r( nlines * 2, h->htab );
  assert( rsl != 0 );
  rsl = 0;

  h->keys = calloc( nlines * 2, sizeof( char * ) );
  assert( h->keys != NULL );

  for ( int i = 0; i < nlines; i++ )
    {
      nread = getline( & line, & len, stream );
      assert( 0 < nread );
      assert( line != NULL );
      tedge = parse_edge_pair( line, PES_SPACE );

      e.key = tedge.u;
      e.data = (void *) ( (long) n );

      /* See if the key was already entered */
      rsl = hsearch_r( e, FIND, & r, h->htab );
      if ( rsl == 0 )  /* Key not found, add it */
        {
          assert( errno == ESRCH );  /* Make sure its not a different error */
          h->keys[n] = e.key;
          r = NULL;
          rsl = hsearch_r( e, ENTER, & r, h->htab );
          assert( rsl != 0 );
          assert( r != NULL );
          n++;
        }
      else
        {
          assert( r != NULL );
          free( tedge.u );
        }
      tedge.u = NULL;
      iedges[i].u = (int) ( (long) r->data );
      rsl = 0;
      r   = NULL;

      e.key = tedge.v;
      e.data = (void *) ( (long) n );

      /* See if the key was already entered */
      rsl = hsearch_r( e, FIND, & r, h->htab );
      if ( rsl == 0 )  /* Key not found, add it */
        {
          assert( errno == ESRCH );  /* Make sure its not a different error */
          h->keys[n] = e.key;
          r = NULL;
          rsl = hsearch_r( e, ENTER, & r, h->htab );
          assert( rsl != 0 );
          assert( r != NULL );
          n++;
        }
      else
        {
          assert( r != NULL );
          free( tedge.v );
        }
      tedge.v = NULL;
      iedges[i].v = (int) ( (long) r->data );
      rsl    = 0;
      r      = NULL;
      e.key  = NULL;
      e.data = NULL;

      free( line );
      line = NULL;
      len = 0;
      nread = 0;
    }

  /* Shrink `keys' to correct size */
  if ( n != ( nlines * 2 ) )
    {
      h->keys = realloc( h->keys, sizeof( char * ) * n );
      assert( h->keys != NULL );
    }
  h->g = graph_create( n );
  assert( h->g != NULL );

  for ( int i = 0; i < nlines; i++ )
    {
      graph_add_edge_safe( h->g, iedges[i].u, iedges[i].v );
    }

  free( iedges );
  iedges = NULL;

  return h;
}


/* -------------------------------------------------------------------------- */

  hgraph_t *
hgraph_fread( const char * pathname )
{
  assert( pathname != NULL );
  hgraph_t * h      = NULL;
  FILE     * stream = fopen( pathname, "r" );
  int        rsl    = 0;
  assert( stream != NULL );
  h = hgraph_parse( stream );
  rsl = fclose( stream );
  stream = NULL;
  assert( rsl == 0 );
  return h;
}


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
