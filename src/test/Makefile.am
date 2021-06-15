
noinst_PROGRAMS =  test_graph test_gsearch test_shortest_path test_cycle
noinst_PROGRAMS += test_hgraph test_graph_util

TESTS =  test_graph test_gsearch test_shortest_path test_cycle
TESTS += test_hgraph test_graph_util

TEST_CFLAGS = -ggdb3
TEST_LD_FLAGS = 

test_graph_CFLAGS = $(TEST_CFLAGS)
test_graph_CPPFLAGS = -I$(top_srcdir)/include
test_graph_SOURCES = test_graph.c $(top_srcdir)/include/graph_test.h
test_graph_LDADD = $(top_builddir)/src/libgraph.la
test_graph_LDFLAGS = $(TEST_LD_FLAGS)

test_gsearch_CFLAGS = $(TEST_CFLAGS)
test_gsearch_CPPFLAGS = -I$(top_srcdir)/include
test_gsearch_SOURCES = test_gsearch.c $(top_srcdir)/include/graph_test.h
test_gsearch_LDADD = $(top_builddir)/src/libgraph.la
test_gsearch_LDFLAGS = $(TEST_LD_FLAGS)

test_shortest_path_CFLAGS = $(TEST_CFLAGS)
test_shortest_path_CPPFLAGS = -I$(top_srcdir)/include
test_shortest_path_SOURCES = test_shortest_path.c
test_shortest_path_SOURCES += $(top_srcdir)/include/graph_test.h
test_shortest_path_LDADD = $(top_builddir)/src/libgraph.la
test_shortest_path_LDFLAGS = $(TEST_LD_FLAGS)

test_cycle_CFLAGS = $(TEST_CFLAGS)
test_cycle_CPPFLAGS = -I$(top_srcdir)/include
test_cycle_SOURCES = test_cycle.c $(top_srcdir)/include/graph_test.h
test_cycle_LDADD = $(top_builddir)/src/libgraph.la
test_cycle_LDFLAGS = $(TEST_LD_FLAGS)

test_hgraph_CFLAGS = $(TEST_CFLAGS)
test_hgraph_CPPFLAGS = -I$(top_srcdir)/include
test_hgraph_SOURCES = test_hgraph.c $(top_srcdir)/include/graph_test.h
test_hgraph_LDADD = $(top_builddir)/src/libgraph.la
test_hgraph_LDFLAGS = $(TEST_LD_FLAGS)

test_graph_util_CFLAGS = $(TEST_CFLAGS)
test_graph_util_CPPFLAGS = -I$(top_srcdir)/include
test_graph_util_SOURCES = test_graph_util.c $(top_srcdir)/include/graph_test.h
test_graph_util_LDADD = $(top_builddir)/src/libgraph.la
test_graph_util_LDFLAGS = $(TEST_LD_FLAGS)


#VALGRIND = valgrind
#VGMINFLAGS = --tool=memcheck --leak-check=yes
#VGFULLFLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all -s
#VGFLAGS = $(VGFULLFLAGS)
#
#test: test_graph test_search test_shortest_path
#	./test_shortest_path
#	$(VALGRIND) $(VGFLAGS) ./test_shortest_path
#	./test_graph
#	$(VALGRIND) $(VGFLAGS) ./test_graph
#	./test_search
#	$(VALGRIND) $(VGFLAGS) ./test_search
