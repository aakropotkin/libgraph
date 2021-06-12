AN_MAKEVAR([BEAR], [AC_PROG_BEAR])
AN_PROGRAM([bear], [AC_PROG_BEAR])
AC_DEFUN_ONCE([AC_PROG_BEAR],
[AC_ARG_VAR([BEAR],      [Bear generator command])dnl
AC_ARG_VAR([BEARFLAGS],  [Bear generator flags])dnl
AC_CHECK_TOOL(BEAR, bear)
if test -z "$BEAR"; then
  if test -n "$ac_tool_prefix"; then
    AC_CHECK_PROG(BEAR, [${ac_tool_prefix}bear], [${ac_tool_prefix}bear])
  fi
fi
if test -z "$BEAR"; then
  AC_CHECK_PROG(BEAR, bear, bear, false)
fi
AC_SUBST([BEAR])
])#AC_DEFUN_ONCE
