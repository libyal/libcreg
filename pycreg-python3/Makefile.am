# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir3)"
pyexecdir = $(pyexecdir3)

if HAVE_PYTHON3
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
	@LIBBFIO_CPPFLAGS@

am_pycreg_la_rpath = -rpath $(pyexecdir3)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pycreg.la

nodist_pycreg_la_SOURCES = $(BUILT_SOURCES)

pycreg_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libcreg/libcreg.la
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@

pycreg_la_CPPFLAGS = $(PYTHON3_CPPFLAGS)
pycreg_la_LDFLAGS  = -module -avoid-version $(PYTHON3_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pycreg/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pycreg*.[ch]

distclean: clean
	/bin/rm -f Makefile

