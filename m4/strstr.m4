# strstr.m4 serial 10
dnl Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl Check that strstr works.
AC_DEFUN([gl_FUNC_STRSTR_SIMPLE],
[
  AC_REQUIRE([gl_HEADER_STRING_H_DEFAULTS])
  AC_REQUIRE([gl_FUNC_MEMCHR])
  if test "$gl_cv_func_memchr_works" != yes; then
    REPLACE_STRSTR=1
    AC_LIBOBJ([strstr])
  else
    dnl Detect http://sourceware.org/bugzilla/show_bug.cgi?id=12092.
    AC_CACHE_CHECK([whether strstr works],
      [gl_cv_func_strstr_works_always],
      [AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <string.h> /* for strstr */
#define P "_EF_BF_BD"
#define HAYSTACK "F_BD_CE_BD" P P P P "_C3_88_20" P P P "_C3_A7_20" P
#define NEEDLE P P P P P
]], [[return !!strstr (HAYSTACK, NEEDLE);
    ]])],
        [gl_cv_func_strstr_works_always=yes],
        [gl_cv_func_strstr_works_always=no],
        [dnl glibc 2.12 and cygwin 1.7.7 have a known bug.  Assume that it
         dnl works on all other platforms, even if it is not linear.
         AC_EGREP_CPP([Lucky user],
           [
#ifdef __GNU_LIBRARY__
 #include <features.h>
 #if (__GLIBC__ == 2 && __GLIBC_MINOR__ > 12) || (__GLIBC__ > 2)
  Lucky user
 #endif
#elif defined __CYGWIN__
 #include <cygwin/version.h>
 #if CYGWIN_VERSION_DLL_COMBINED > CYGWIN_VERSION_DLL_MAKE_COMBINED (1007, 7)
  Lucky user
 #endif
#else
  Lucky user
#endif
           ],
           [gl_cv_func_strstr_works_always=yes],
           [gl_cv_func_strstr_works_always="guessing no"])
        ])
      ])
    if test "$gl_cv_func_strstr_works_always" != yes; then
      REPLACE_STRSTR=1
      AC_LIBOBJ([strstr])
    fi
  fi
]) # gl_FUNC_STRSTR_SIMPLE

dnl Additionally, check that strstr is efficient.
AC_DEFUN([gl_FUNC_STRSTR],
[
  AC_REQUIRE([gl_FUNC_STRSTR_SIMPLE])
  if test $REPLACE_STRSTR = 0; then
    AC_CACHE_CHECK([whether strstr works in linear time],
      [gl_cv_func_strstr_linear],
      [AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h> /* for signal */
#include <string.h> /* for strstr */
#include <stdlib.h> /* for malloc */
#include <unistd.h> /* for alarm */
static void quit (int sig) { exit (sig + 128); }
]], [[size_t m = 1000000;
    char *haystack = (char *) malloc (2 * m + 2);
    char *needle = (char *) malloc (m + 2);
    void *result = 0;
    /* Failure to compile this test due to missing alarm is okay,
       since all such platforms (mingw) also have quadratic strstr.  */
    signal (SIGALRM, quit);
    alarm (5);
    /* Check for quadratic performance.  */
    if (haystack && needle)
      {
        memset (haystack, 'A', 2 * m);
        haystack[2 * m] = 'B';
        haystack[2 * m + 1] = 0;
        memset (needle, 'A', m);
        needle[m] = 'B';
        needle[m + 1] = 0;
        result = strstr (haystack, needle);
      }
    return !result;]])],
        [gl_cv_func_strstr_linear=yes], [gl_cv_func_strstr_linear=no],
        [dnl Only glibc > 2.12 and cygwin > 1.7.7 are known to have a
         dnl bug-free strstr that works in linear time.
         AC_EGREP_CPP([Lucky user],
           [
#include <features.h>
#ifdef __GNU_LIBRARY__
 #if (__GLIBC__ == 2 && __GLIBC_MINOR__ > 12) || (__GLIBC__ > 2)
  Lucky user
 #endif
#endif
#ifdef __CYGWIN__
 #include <cygwin/version.h>
 #if CYGWIN_VERSION_DLL_COMBINED > CYGWIN_VERSION_DLL_MAKE_COMBINED (1007, 7)
  Lucky user
 #endif
#endif
           ],
           [gl_cv_func_strstr_linear=yes],
           [gl_cv_func_strstr_linear="guessing no"])
        ])
      ])
    if test "$gl_cv_func_strstr_linear" != yes; then
      REPLACE_STRSTR=1
    fi
  fi
  if test $REPLACE_STRSTR = 1; then
    AC_LIBOBJ([strstr])
  fi
]) # gl_FUNC_STRSTR
