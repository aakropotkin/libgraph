#ifndef GRAPH_TEST_H_
#define GRAPH_TEST_H_

#define TEST_ASSERT( EXP )                      \
  if ( ! ( EXP ) )                              \
  {                                             \
    fprintf( stderr,                            \
             "Failure in %s at line %d: %s\n",  \
             __func__, __LINE__, # EXP          \
           );                                   \
    return 0;                                   \
  }


#define RUN_TEST( RSL, TEST_NAME )                              \
  if ( ! ( TEST_NAME() ) )                                      \
    {                                                           \
      fprintf( stderr, "Failed sub-test: %s\n", # TEST_NAME );  \
      RSL = 0;                                                  \
    }


#endif  /* GRAPH_TEST_H_ */
