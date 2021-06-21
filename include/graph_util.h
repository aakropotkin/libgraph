#ifndef _GRAPH_UTIL_H
#define _GRAPH_UTIL_H

/* ========================================================================== */

#include "graph.h"
#include "hgraph.h"
#include <stdio.h>


/* -------------------------------------------------------------------------- */

void hgraph_fprint( FILE *, hgraph_t * );
void hgraph_write( const char * pathname, hgraph_t * );

void hgraph_fprint_dot( FILE *, hgraph_t * );
void hgraph_write_dot( const char * pathname, hgraph_t * );


/* -------------------------------------------------------------------------- */



/* ========================================================================== */
#endif /* _GRAPH_UTIL_H */
