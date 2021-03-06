dnl Process this file with autoconf to produce a configure script.
AC_INIT([vilistextum], [2.8.0-devel])
AC_CONFIG_SRCDIR([src/debug.h])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

dnl Set current date
AC_DEFINE_UNQUOTED(RELEASEDATE, "??.??.???", "Release date of the program")

dnl Checks for programs.
AC_PROG_CC

AC_C_CHAR_UNSIGNED

dnl check for #! (shebang)
AC_SYS_INTERPRETER

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(getopt.h)

dnl add /usr/local
CFLAGS="$CFLAGS -I/usr/local/include"
LDFLAGS="$LDFLAGS -L/usr/local/lib"

dnl checking for options
AC_MSG_CHECKING(if multibyte support should be enabled)
AC_ARG_ENABLE(multibyte,
  AS_HELP_STRING(--enable-multibyte,Enable multibyte support [default=yes].),
  [use_multibyte=$enableval], [use_multibyte=yes])
AC_MSG_RESULT($use_multibyte)

if test "$use_multibyte" = "yes"; then
        AC_DEFINE(MULTIBYTE, 1, "Enable multibyte support (e.g. unicode, shift_jis)")
fi

dnl Checks for libraries.
if test "$use_multibyte" = "yes"; then
AC_CHECK_HEADERS(iconv.h,,
			AC_MSG_ERROR(Need libiconv or iconv.h for multibyte support))

AC_CHECK_LIB(iconv, iconv,,
			AC_MSG_WARN([Generic iconv found but not libiconv. See INSTALL for details]))
AC_CHECK_HEADERS([wchar.h])
AC_CHECK_TYPE([wchar_t],,[ AC_MSG_ERROR([Type wchar_t not found.]) ])
# check for all needed wide char function
# even if not all defines are used in code
AC_CHECK_FUNCS(mbstowcs wcstombs fgetwc ungetwc wcscasecmp wcscat wcscmp wcscpy wcslen wcsncat wcsncmp wcsstr wcstol swprintf)

dnl setting INTERNAL_LOCALE
AC_MSG_CHECKING(for unicode locale)
# prefer en_US.UTF-8 locale
DEFAULT_LOCALE=`locale -a | grep -i 'en_US.utf.*8$' | sed -e '2,$D'`
if test -z "$DEFAULT_LOCALE"; then
 # prefer any english UTF-8 locale
 DEFAULT_LOCALE=`locale -a | grep -i 'en_.*.utf.*8$' | sed -e '2,$D'`
fi
if test -z "$DEFAULT_LOCALE"; then
 # take any UTF-8 locale
 DEFAULT_LOCALE=`locale -a | grep -i 'utf.*8$' | sed -e '2,$D'`
fi
AC_MSG_RESULT("$DEFAULT_LOCALE")
AC_MSG_CHECKING(if overwriting automatically set unicode locale)
AC_ARG_WITH(unicode-locale,
	[  --with-unicode-locale=LOCALE  use this LOCALE instead of $DEFAULT_LOCALE],
	AC_MSG_RESULT("$with_unicode_locale") ;
		INTERNAL_LOCALE="$with_unicode_locale" ;
		AC_DEFINE_UNQUOTED(INTERNAL_LOCALE, "$with_unicode_locale", [preferred Unicode set]),
	AC_MSG_RESULT(no)  ;
		INTERNAL_LOCALE="$DEFAULT_LOCALE" ;
		AC_DEFINE_UNQUOTED(INTERNAL_LOCALE, "$DEFAULT_LOCALE", [preferred Unicode set]))

if test "$INTERNAL_LOCALE" = ""; then
		AC_MSG_ERROR([Couldn't find utf-8 encoding. Use --with-unicode-locale=LOCALE.])
fi
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_CHECK_FUNCS(strstr)
AC_CHECK_FUNCS(strcasecmp)

dnl create testsuite
dnl AC_CONFIG_TESTDIR([tests])
dnl AC_CONFIG_FILES([tests/Makefile tests/atlocal])
dnl AM_MISSING_PROG([AUTOM4TE], [autom4te])

AC_CONFIG_FILES([doc/Makefile
                 src/Makefile
                 tests/Makefile
                 Makefile])
AC_OUTPUT

