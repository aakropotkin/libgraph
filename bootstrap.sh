#! /usr/bin/env sh
mkdir -p m4;

declare -a GNULIB_MODULES=(
  array-list
  linked-list
  linkedhash-list
  array-map
  hash-map
  realloc-gnu
  hash-set
  strdup-posix
  strndup
  stddef
  stdlib
  stdio
  stdbool
  stdint
);

gnulib-tool                      \
  --libtool                      \
  --with-tests                   \
  --no-conditional-dependencies  \
  --vc-files                     \
  --macro-prefix=gl              \
  --source-base=gnulib           \
  --m4-base=gnulib/m4            \
  --doc-base=gnulib/doc          \
  --tests-base=gnulib/tests      \
  --aux-dir=build-aux            \
  --import                       \
  ${GNULIB_MODULES[*]}           \
  ;

aclocal && autoreconf -if;
