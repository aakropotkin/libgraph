
#include <stdlib.h>
#include <assert.h>

struct pq_s {
  int element_length;
  int (*compare)( const void *, const void * );
  int    n;            /* number of elements */
  int    size;         /* number of slots in data */
  void * swap_space;   /* element_length bytes used for swapping */
  void * data;
};

#include <string.h>
#include "pq.h"


#define PQ_INITIAL_SIZE ( 128 )


  pq_t *
pq_create( int element_length, int (*compare)( const void *, const void * ) )
{
  pq_t * pq = NULL;

  pq = malloc( sizeof( pq_t ) );
  assert( pq );

  pq->element_length = element_length;
  pq->compare = compare;
  pq->n = 0;
  pq->size = PQ_INITIAL_SIZE;

  pq->swap_space = malloc( pq->element_length );
  assert( pq->swap_space );

  pq->data = malloc( pq->element_length * pq->size );
  assert( pq->data );

  return pq;
}

  static int
cmp_always_eq( const void * a, const void * b )
{
  return 0;
}

  pq_t *
q_create( int element_length )
{
  assert( 0 < element_length );
  return pq_create( element_length, cmp_always_eq );
}


  void
pq_destroy( pq_t * pq )
{
  if ( pq == NULL ) return;
  free( pq->data );
  pq->data = NULL;
  free( pq->swap_space );
  pq->swap_space = NULL;
  free( pq );
  pq = NULL;
}


  int
pq_is_empty( pq_t * pq )
{
  return pq->n == 0;
}


/**
 * `Child( i, 0 )' and `Child( i, 1 )' are children of `i'
 * `Parent( i )' is parent of `i'
 */
#define Child( i, x )  ( 2 * ( i ) + 1 + ( x ) )
#define Parent( i )    ( ( ( i ) - 1 ) / 2 )


#define NUM_CHILDREN  ( 2 )


/** Compute the address of position `i' in the data field */
#define REF( pq, i )                                                       \
  ( (void *) ( ( (char *) ( pq )->data ) + ( pq )->element_length * i ) )


/** Swap elements at indexes `i1' and `i2' */
  static void
pq_swap( pq_t * pq, int i1, int i2 )
{
  memcpy( pq->swap_space, REF( pq, i1 ), pq->element_length );
  memcpy( REF( pq, i1 ), REF( pq, i2 ), pq->element_length );
  memcpy( REF( pq, i2 ), pq->swap_space, pq->element_length );
}


  void
pq_insert( pq_t * pq, const void * elt )
{
  int floater = -1;  /* new element */

  while ( pq->n + 1 > pq->size )
    {
      pq->size *= 2;
      pq->data = realloc( pq->data, pq->element_length * pq->size );
      assert( pq->data );
    }

  /* copy the new element in */
  floater = pq->n++;
  memcpy( REF( pq, floater ), elt, pq->element_length );

  /* float it up until it is at the top */
  /* or it is no smaller than its parent */
  while ( ( 0 < floater ) &&
          ( pq->compare( REF( pq, floater ),
                         REF( pq, Parent( floater ) )
                       ) <= 0
          )
        )
    {
      /* it's smaller than its parent */
      pq_swap( pq, floater, Parent( floater ) );
      floater = Parent( floater );
    }
}


  void
pq_delete_min( pq_t * pq, void * retval )
{
  int floater     = -1;    /* previous loser floating down */
  int small_child = -1;    /* smaller child of floater */

  assert( ! pq_is_empty( pq ) );

  /* first copy out the winner */
  memcpy( retval, REF( pq, 0 ), pq->element_length );

  --( pq->n );

  if ( pq_is_empty( pq ) )
    {
      /* pq empty, nothing to do */
      return;
    }

  /* else */
  memcpy( REF( pq, 0 ), REF( pq, pq->n ), pq->element_length );

  floater = 0;

  for ( ;; )
    {
      /* find smaller child of floater */
      if ( pq->n <= Child( floater, 0 ) )
        {
          return;     /* no children, bail out */
        }
      else if ( pq->n <= Child( floater, 1 ) )
        {
          small_child = Child( floater, 0 );
        }
      else if( pq->compare( REF( pq, Child( floater, 0 ) ),
                            REF( pq, Child( floater, 1 ) )
                          ) < 0
             )
        {
          small_child = Child( floater, 0 );
        }
      else
        {
          small_child = Child( floater, 1 );
        }

      /* is floater <= small_child? */
      if ( pq->compare( REF( pq, floater ), REF( pq, small_child ) ) <= 0 )
        {
          /* yes, we are done */
          return;
        }
      else
        {
          /* no, swap and continue floating down */
          pq_swap( pq, floater, small_child );
          floater = small_child;
        }
    }
}


  void
pq_sanity_check( pq_t * pq )
{
  int i = -1;
  int j = -1;

  assert( pq->n >= 0 );
  assert( pq->n <= pq->size );

  for ( i = 0; i < pq->n; i++ )
    {
      for( j = 0; j < NUM_CHILDREN; j++ )
        {
          if ( Child( i, j ) < pq->n )
            {
              assert( pq->compare( REF( pq, i ), REF( pq, Child( i, j ) ) )
                      <= 0
                    );
            }
        }
    }
}
