
noinst_PROGRAMS = test_graph test_search test_shortest_path test_cycle

TESTS = test_graph test_search test_shortest_path test_cycle

TEST_CFLAGS = -ggdb3

test_graph_CFLAGS = $(TEST_CFLAGS)
test_graph_CPPFLAGS = -I$(top_srcdir)/include
test_graph_SOURCES = test_graph.c $(top_srcdir)/include/graph_test.h
test_graph_LDADD = $(top_builddir)/src/libgraph.la

test_search_CFLAGS = $(TEST_CFLAGS)
test_search_CPPFLAGS = -I$(top_srcdir)/include
test_search_SOURCES = test_search.c $(top_srcdir)/include/graph_test.h
#test_search_SOURCES += ../search.c ../graph.c
test_search_LDADD = $(top_builddir)/src/libgraph.la

test_shortest_path_CFLAGS = $(TEST_CFLAGS)
test_shortest_path_CPPFLAGS = -I$(top_srcdir)/include
test_shortest_path_SOURCES = test_shortest_path.c
test_shortest_path_SOURCES += $(top_srcdir)/include/graph_test.h
test_shortest_path_LDADD = $(top_builddir)/src/libgraph.la

test_cycle_CFLAGS = $(TEST_CFLAGS)
test_cycle_CPPFLAGS = -I$(top_srcdir)/include
test_cycle_SOURCES = test_cycle.c $(top_srcdir)/include/graph_test.h
test_cycle_LDADD = $(top_builddir)/src/libgraph.la


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
