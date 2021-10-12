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


# `autoreconf' is not used so that we can patch `libtool.m4' before
# running `autoconf'.

##aclocal && autoreconf -if;

declare -a UB_PATCHES=(
  m4/libtool.m4
  build-aux/ltmain.sh
);

set -e;
aclocal --force --install;
libtoolize -c;
echo "Patching '/usr/bin/' usage in autotools";
chmod +w ${UB_PATCHES[*]};
sed -i 's:/usr/bin/::g' ${UB_PATCHES[*]};
aclocal --force --install;
autoconf -f;
autoheader -f;
autoheader -f;
automake -acf;
set +e;


echo "Done";
