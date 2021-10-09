#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([libgraph],[0.1.0],[alex.ameen.tx@gmail.com])
AC_CONFIG_MACRO_DIRS([m4 gnulib/m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/pq.c])
AC_CONFIG_HEADERS([include/config.h])
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_CC
gl_EARLY
AM_PROG_AR

LT_PREREQ([2.4.6])
LT_INIT

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_BEAR

# Checks for libraries.

# For gnulib.
gl_INIT

# Checks for header files.
AC_CHECK_HEADERS([values.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
