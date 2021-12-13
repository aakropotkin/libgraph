AN_MAKEVAR([BEAR], [AC_PROG_BEAR])
AN_PROGRAM([bear], [AC_PROG_BEAR])
# v3: --output FILE
# v2: --o FILE
# v2: --cdb FILE
AC_DEFUN_ONCE([AC_PROG_BEAR],
[AC_ARG_VAR([BEAR],      [Bear generator command])dnl
AC_ARG_VAR([BEARFLAGS],  [Bear generator flags])dnl
AC_ARG_VAR([BEAROUTFILE], [Bear generator output file])dnl
AS_IF([test -n "$BEAR"],
[AS_IF([test -z "$BEAROUTFILE"],
       [BEAROUTFILE="\$(top_builddir)/compile_commands.json"])
 AS_IF([test -z "$BEARFLAGS"],
       [BEAR_VERSION="`$BEAR --version|cut -d\" \" -f2`"
       AS_CASE([$BEAR_VERSION],
         [2.*], [BEARFLAGS="-o \$(BEAROUTFILE)"],
         [3.*], [BEARFLAGS="--output \$(BEAROUTFILE) --"])])
])
AC_SUBST([BEAR])
AC_SUBST([BEAROUTFILE])
AC_SUBST([BEARFLAGS])
AM_CONDITIONAL([BEAR_AM], [test -n "$BEAR"])
])# AC_PROG_BEAR
