SUBDIRS = . tests

lib_LTLIBRARIES = librb64u.la
include_HEADERS = librb64u.h
librb64u_la_SOURCES = librb64u.c librb64u.h

ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -ansi -pedantic -Wall

librb64u_la_LDFLAGS = -version-info ${base64url_ltver}
