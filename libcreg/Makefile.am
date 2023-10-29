AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBCREG_DLL_EXPORT@

lib_LTLIBRARIES = libcreg.la

libcreg_la_SOURCES = \
	creg_data_block.h \
	creg_file_header.h \
	creg_key_navigation.h \
	libcreg.c \
	libcreg_codepage.h \
	libcreg_data_block.c libcreg_data_block.h \
	libcreg_data_type.c libcreg_data_type.h \
	libcreg_debug.c libcreg_debug.h \
	libcreg_definitions.h \
	libcreg_error.c libcreg_error.h \
	libcreg_extern.h \
	libcreg_file.c libcreg_file.h \
	libcreg_io_handle.c libcreg_io_handle.h \
	libcreg_key.c libcreg_key.h \
	libcreg_key_descriptor.c libcreg_key_descriptor.h \
	libcreg_key_hierarchy_entry.c libcreg_key_hierarchy_entry.h \
	libcreg_key_item.c libcreg_key_item.h \
	libcreg_key_name_entry.c libcreg_key_name_entry.h \
	libcreg_key_navigation.c libcreg_key_navigation.h \
	libcreg_key_tree.c libcreg_key_tree.h \
	libcreg_libbfio.h \
	libcreg_libcdata.h \
	libcreg_libcerror.h \
	libcreg_libclocale.h \
	libcreg_libcnotify.h \
	libcreg_libcthreads.h \
	libcreg_libfcache.h \
	libcreg_libfdata.h \
	libcreg_libuna.h \
	libcreg_notify.c libcreg_notify.h \
	libcreg_support.c libcreg_support.h \
	libcreg_types.h \
	libcreg_unused.h \
	libcreg_value.c libcreg_value.h \
	libcreg_value_entry.c libcreg_value_entry.h \
	libcreg_value_type.c libcreg_value_type.h

libcreg_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@PTHREAD_LIBADD@

libcreg_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libcreg_definitions.h.in \
	libcreg.rc \
	libcreg.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libcreg_definitions.h
	-rm -f libcreg.rc
	-rm -f Makefile

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libcreg ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcreg_la_SOURCES)

