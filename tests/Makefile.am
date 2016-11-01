AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBCREG_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYCREG = \
	test_python_functions.sh \
	test_pycreg_set_ascii_codepage.sh
endif

TESTS = \
	test_api_functions.sh \
	test_creginfo.sh \
	test_cregexport.sh \
	test_set_ascii_codepage.sh \
	$(TESTS_PYCREG)

check_SCRIPTS = \
	pycreg_test_get_version.py \
	pycreg_test_open_close.py \
	pycreg_test_set_ascii_codepage.py \
	test_api_functions.sh \
	test_cregexport.sh \
	test_creginfo.sh \
	test_pycreg_set_ascii_codepage.sh \
	test_python_functions.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	creg_test_error \
	creg_test_open_close \
	creg_test_set_ascii_codepage \
	creg_test_support

creg_test_error_SOURCES = \
	creg_test_error.c \
	creg_test_libcreg.h \
	creg_test_libcstring.h \
	creg_test_macros.h \
	creg_test_unused.h

creg_test_error_LDADD = \
	../libcreg/libcreg.la \
	@LIBCSTRING_LIBADD@

creg_test_open_close_SOURCES = \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_libcstring.h \
	creg_test_libcsystem.h \
	creg_test_open_close.c

creg_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

creg_test_set_ascii_codepage_SOURCES = \
	creg_test_libcerror.h \
	creg_test_libclocale.h \
	creg_test_libcreg.h \
	creg_test_libcstring.h \
	creg_test_set_ascii_codepage.c

creg_test_set_ascii_codepage_LDADD = \
	../libcreg/libcreg.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

creg_test_support_SOURCES = \
	creg_test_libcreg.h \
	creg_test_libcstring.h \
	creg_test_macros.h \
	creg_test_support.c \
	creg_test_unused.h

creg_test_support_LDADD = \
	../libcreg/libcreg.la \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

