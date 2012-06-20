AC_PREREQ(2.61)
LT_PREREQ([1.5.26])

AC_INIT([librb64u],[0.0.0],[jon@wroth.org])
# fails on my old system, but safely ignored
LT_INIT

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([librb64u.c])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_LIBTOOL
# autoscan complains, but not needed
#AC_PROG_RANLIB

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h stdint.h string.h])

AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T

AC_CHECK_FUNCS([memset memcpy])

AC_CONFIG_FILES([Makefile tests/Makefile])

AC_OUTPUT
