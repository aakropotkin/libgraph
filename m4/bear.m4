AN_MAKEVAR([BEAR], [AC_PROG_BEAR])
AN_PROGRAM([bear], [AC_PROG_BEAR])
# Indicating the output file differs with versions:
# v3: --output FILE
# v2: --cdb FILE
AC_DEFUN_ONCE([AC_PROG_BEAR],
[AC_ARG_VAR([BEAR],      [Bear generator command])dnl
AC_ARG_VAR([BEARFLAGS],  [Bear generator flags])dnl
AC_ARG_VAR([BEAROUTFILE], [Bear generator output file])dnl
if test -z "$BEAR"; then
  AC_CHECK_PROG(BEAR, bear, bear)
fi
if test -n "$BEAR"; then
  BEAR=`which bear`;
  if test -z "$BEAROUTFILE"; then
    BEAROUTFILE='$(top_builddir)/compile_commands.json';
  fi
  if test -z "$BEARFLAGS"; then
    BEARFLAGS="--output $BEAROUTFILE --";
  fi
fi
AC_SUBST([BEAR])
AC_SUBST([BEARFLAGS])
AC_SUBST([BEAROUTFILE])
AM_CONDITIONAL([AM_BEAR], [test -n $BEAR])
])#AC_DEFUN_ONCE
