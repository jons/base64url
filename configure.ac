# specify the binary interface here and allow tags to follow this schema
m4_define([rb64u_cur],[0])
m4_define([rb64u_rev],[1])
m4_define([rb64u_age],[0])

AC_PREREQ(2.61)
LT_PREREQ([1.5.26])

AC_INIT([librb64u],[rb64u_cur.rb64u_rev.rb64u_age],[jon@wroth.org])
# fails on my old system, but safely ignored
LT_INIT

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([librb64u.c])
AM_INIT_AUTOMAKE([foreign])

base64url_ltver="rb64u_cur:rb64u_rev:rb64u_age"
AC_SUBST([base64url_ltver])

AC_PROG_CC
AC_PROG_LIBTOOL
# autoscan complains, but not needed
#AC_PROG_RANLIB

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h stdint.h string.h assert.h])

AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T

AC_CHECK_FUNCS([memset memcpy])

AC_CONFIG_FILES([Makefile tests/Makefile codec/Makefile])

AC_OUTPUT
