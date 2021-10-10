# -*- mode: autoconf; -*-
# ============================================================================ #
#
#
# ---------------------------------------------------------------------------- #

#serial 1

# AK_CHECK_NIX
# ------------
AC_DEFUN([AK_CHECK_NIX],
[AC_CACHE_CHECK([whether CC is a nix wrapper], [ak_cv_nix_cc_wrapper],
  [AC_REQUIRE([AC_PROG_CC])
  AS_IF([file -Lb `which $CC` 2>/dev/null|[]dnl
        $GREP -q "^a /nix/store/[[^ ]]*/bash script, ASCII text executable\$"],
        [ak_cv_nix_cc_wrapper=yes],
        [ak_cv_nix_cc_wrapper=no])
  ])
AM_CONDITIONAL([NIX_CC_WRAPPER], [test "x$ak_cv_nix_cc_wrapper" = xyes])
])# AK_CHECK_NIX


# ---------------------------------------------------------------------------- #



# ============================================================================ #
# vim: set filetype=config :
