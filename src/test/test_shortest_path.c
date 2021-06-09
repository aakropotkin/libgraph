#include "shortest_path.h"
#include "dijkstra.h"
#include <values.h>
#include <assert.h>
#include <stdio.h>



#define TEST_SIZE ( 37 )


  int
main( int argc, char * argv[], char ** envp )
{
  graph_t * g;
  int i;
  int j;
  int dist[TEST_SIZE];
  int parent[TEST_SIZE];
  int ret;
  int * d[TEST_SIZE];
  int ddata[TEST_SIZE][TEST_SIZE];

  /* build a big line */
  g = graph_create( TEST_SIZE );

  for ( i = 0; i < ( TEST_SIZE - 2 ); i++ )
    {
      graph_add_weighted_edge( g, i, ( i + 1 ), - ( ( 2 * i ) + 1 ) );
    }

  /* and some useless junk edges */
  for ( i = 0; i < ( TEST_SIZE - 3 ); i++ )
    {
      graph_add_weighted_edge( g, i, ( i + 2 ), 1 );
    }

  /* search from 0 */
  ret = bellman_ford( g, 0, dist, parent );

  /* no negative cycles */
  assert( ret == 0 );

  /* check distances */
  for ( i = 0; i < ( TEST_SIZE - 2 ); i++ )
    {
      assert( dist[i] == ( - ( i * i ) ) );
      assert( ( i == 0 ) || ( parent[i] == ( i - 1 ) ) );
    }
  
  assert( dist[TEST_SIZE - 1] == MAXINT );
  assert( parent[TEST_SIZE - 1] == SHORTEST_PATH_NULL_PARENT );

  graph_destroy( g );

  /* build another big line to test Dijkstra's algorithm */
  /* here we have to make the edge weights positive */
  g = graph_create( TEST_SIZE );

  for ( i = 0; i < ( TEST_SIZE - 2 ); i++ )
    {
      graph_add_weighted_edge( g, i, ( i + 1 ), ( ( 2 * i ) + 1 ) );
    }

  /* and some useless junk edges */
  for ( i = 0; i < ( TEST_SIZE - 3 ); i++ )
    {
      graph_add_weighted_edge( g, i, ( i + 2 ), ( TEST_SIZE * TEST_SIZE ) );
    }

  /* run it */
  dijkstra( g, 0, dist, parent );

  /* everybody should be at ( i * i ) */
  for ( i = 0; i < ( TEST_SIZE - 2 ); i++ )
    {
      assert( dist[i] == i*i );
      assert( ( i ==  0 ) || ( parent[i] == ( i - 1 ) ) );
    }

  /* except TEST_SIZE - 1 */
  assert( dist[TEST_SIZE - 1] == MAXINT );
  assert( parent[TEST_SIZE - 1] == DIJKSTRA_NULL_PARENT );

  graph_destroy( g );

  /* build another big line, this time as an adjacency matrix */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      d[i] = ddata[i];
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          if ( i == j )
            {
              d[i][j] = 0;
            }
          else if ( j == ( i + 1 ) )
            {
              d[i][j] = 1;
            }
          else
            {
              d[i][j] = MAXINT;
            }
        }
    }

  /* run floyd-warshall */
  floyd_warshall( TEST_SIZE, d );

  /* check results */
  for ( i = 0; i < TEST_SIZE; i++ )
    {
      for ( j = 0; j < TEST_SIZE; j++ )
        {
          if ( i <= j )
            {
              assert( d[i][j] == ( j - i ) );
            }
          else
            {
              assert( d[i][j] == MAXINT );
            } 
        }
    }

  return 0;
}
