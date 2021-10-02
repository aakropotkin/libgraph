# -*- mode: autoconf; -*-
# ============================================================================ #
#
#
# ---------------------------------------------------------------------------- #

#serial 1

# AK_PROG_PATH(VARIABLE, PROGRAM, [VALUE-IF-NOT-SET], [PATH = $PATH])
# -------------------------------------------------------------------
AC_DEFUN([AK_PROG_PATH],
[AC_PREREQ([2.61])
AC_ARG_VAR([$1], [Absolute path to $2 executable])
AS_VAR_SET_IF([$1],
              [AC_MSG_CHECKING([for $2])
              AS_VAR_COPY([ak_USER_VAR], [$1])
              AS_VAR_SET([$1], ["`AK_REALPATH_S([$ak_USER_VAR])`"])
              AS_IF([AS_EXECUTABLE_P([$[]$1])],
                    [AC_SUBST([$1])
                    AC_MSG_RESULT([$[]$1])],
                    [AC_MSG_RESULT([no])
                    AC_MSG_ERROR([invalid path for $2: $[]$1])])],
              [AC_PATH_PROG([$1], [$2], [$3], [$4])])
])# AK_PROG_PATH


# ---------------------------------------------------------------------------- #



# ============================================================================ #
# vim: set filetype=config :
