#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.65)
AC_INIT(listserv-utils, 1.0, listserv-utils@aegee.org)
AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 subdir-objects silent-rules])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/generate-listowners-l.c])
AC_CONFIG_MACRO_DIR([m4])
# Checks for programs.
AC_PROG_CC
gl_EARLY
AM_PROG_CC_C_O
AM_PROG_AR
LT_INIT([disable-shared, dlopen])
LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT([nonrecursive])
AC_CHECK_HEADERS_ONCE(signal.h, errno.h, sys/time.h, sys/stat.h, sys/types.h, libmilter/mfapi.h)
# Checks for libraries.
AC_CHECK_LIB([milter], [smfi_main],,,[-lpthread])
AC_CHECK_LIB([ltdl], [lt_dlsym])
dnl AC_CHECK_LIB([pthread],[])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
PKG_PROG_PKG_CONFIG(0.25)
PKG_CHECK_MODULES([glib], [glib-2.0 >= 2.26], [check_glib=yes], [check_glib=no])
PKG_CHECK_MODULES([liblistserv], [liblistserv >= 0.1], [check_liblistserv=yes], [check_liblistserv=no])
dnl AM_GNU_GETTEXT_VERSION([0.18.1])
dnl AM_GNU_GETTEXT
gl_INIT
AC_FUNC_MALLOC
AC_HEADER_STDC
AC_CONFIG_FILES([Makefile lib/Makefile])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT
