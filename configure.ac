#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([libgraph], [0.1.0], [alex.ameen.tx@gmail.com])
AC_CONFIG_MACRO_DIRS([m4])
#AM_PREREQ([1.16.3])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_PREREQ([2.4.6])
LT_INIT
AC_CONFIG_SRCDIR([src/pq.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_BEAR

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([values.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 src/test/Makefile])
AC_OUTPUT
