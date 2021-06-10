#include "shortest_path.h"
#include <assert.h>
#include <values.h>


/** data field for relax helper */
struct relax_data {
  int   improved;
  int * dist;
  int * parent;
};


  static void
relax(  graph_t * g, int source, int sink, int weight, void * data  )
{
  int len;
  struct relax_data * d;

  d = data;

  if ( ( d->dist[source] < MAXINT ) && ( weight < MAXINT ) )
    {
      len = d->dist[source] + weight;

      if ( len < d->dist[sink] )
        {
          d->dist[sink] = len;
          if ( d->parent )
            {
              d->parent[sink] = source;
            }
          d->improved = 1;
        }
    }
}


/** returns 1 if there is a negative cycle */
  int
bellman_ford( graph_t * g, int source, int *dist, int *parent )
{
  int round;
  int n;
  int i;
  struct relax_data d;

  assert( dist );

  d.dist = dist;
  d.parent = parent;
  d.improved = 1;

  n = graph_vertex_count( g );

  for ( i = 0; i < n; i++ )
    {
      d.dist[i] = MAXINT;
      if ( d.parent )
        {
          d.parent[i] = SHORTEST_PATH_NULL_PARENT;
        }
    }

  d.dist[source] = 0;
  d.parent[source] = source;

  for ( round = 0; d.improved && ( round < n ); round++ )
    {
      d.improved = 0;

      /* relax all edges */
      for ( i = 0; i < n; i++ )
        {
          graph_foreach_weighted( g, i, relax, &d );
        }
    }

  return d.improved;
}

 void
floyd_warshall( int n, int **d )
{
  int i;
  int j;
  int k;
  int newdist;

  /**
   * The algorithm:
   *
   * d( i, j, k ) = min distance from i to j with all intermediates <= k
   *
   * d( i, j, k ) = min( d( i, j, k-1 ), d( i, k, k-1 ) + d( k, j, k-1 )
   *
   * We will allow shorter paths to sneak in to d( i, j, k ) so that
   * we don't have to store anything extra.
   */

  /* initial matrix is essentially d( :,:,-1 )
   * within body of outermost loop we compute d( :,:,k ) */
  for ( k = 0; k < n; k++ )
    {
      for ( i = 0; i < n; i++ )
        {
          /* skip if we can't get to k */
          if ( d[i][k] == MAXINT )
            {

              continue;
            }
          for ( j = 0; j < n; j++ )
            {
              /* skip if we can't get from k */
              if ( d[k][j] == MAXINT )
                {
                  continue;
                }

              /* else */
              newdist = d[i][k] + d[k][j];
              if ( newdist < d[i][j] )
                {
                  d[i][j] = newdist;
                }
            }
        }
    }
}
