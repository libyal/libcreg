if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCREG_DLL_IMPORT@

pyexec_LTLIBRARIES = pycreg.la

pycreg_la_SOURCES = \
	pycreg.c pycreg.h \
	pycreg_codepage.c pycreg_codepage.h \
	pycreg_error.c pycreg_error.h \
	pycreg_file.c pycreg_file.h \
	pycreg_file_object_io_handle.c pycreg_file_object_io_handle.h \
	pycreg_integer.c pycreg_integer.h \
	pycreg_key.c pycreg_key.h \
	pycreg_keys.c pycreg_keys.h \
	pycreg_libbfio.h \
	pycreg_libcerror.h \
	pycreg_libclocale.h \
	pycreg_libcreg.h \
	pycreg_python.h \
	pycreg_unused.h \
	pycreg_value.c pycreg_value.h \
	pycreg_value_types.c pycreg_value_types.h \
	pycreg_values.c pycreg_values.h

pycreg_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pycreg_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pycreg_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

