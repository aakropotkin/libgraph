#! /usr/bin/env sh
mkdir -p m4;

##declare -a GNULIB_MODULES=(
##  list
##  map
##  realloc-gnu
##  set
##  strdup-posix
##  strndup
##);
##
##gnulib-tool                      \
##  --libtool                      \
##  --with-tests                   \
##  --no-conditional-dependencies  \
##  --vc-files                     \
##  --macro-prefix=gl              \
##  --source-base=gnulib           \
##  --m4-base=gnulib/m4            \
##  --doc-base=gnulib/doc          \
##  --tests-base=gnulib/tests      \
##  --aux-dir=build-aux            \
##  --import                       \
##  ${GNULIB_MODULES[*]}           \
##  ;

aclocal && autoreconf -if;
