AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBCREG_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	cregexport \
	creginfo \
	cregmount

cregexport_SOURCES = \
	cregexport.c \
	cregtools_getopt.c cregtools_getopt.h \
	cregtools_i18n.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcreg.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libuna.h \
	cregtools_output.c cregtools_output.h \
	cregtools_signal.c cregtools_signal.h \
	cregtools_unused.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h

cregexport_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

creginfo_SOURCES = \
	creginfo.c \
	cregtools_getopt.c cregtools_getopt.h \
	cregtools_i18n.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcreg.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libuna.h \
	cregtools_output.c cregtools_output.h \
	cregtools_signal.c cregtools_signal.h \
	cregtools_unused.h \
	info_handle.c info_handle.h

creginfo_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

cregmount_SOURCES = \
	cregmount.c \
	cregtools_getopt.c cregtools_getopt.h \
	cregtools_i18n.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcpath.h \
	cregtools_libcreg.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libuna.h \
	cregtools_output.c cregtools_output.h \
	cregtools_signal.c cregtools_signal.h \
	cregtools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

cregmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

splint-local:
	@echo "Running splint on cregexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(cregexport_SOURCES)
	@echo "Running splint on creginfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(creginfo_SOURCES)
	@echo "Running splint on cregmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(cregmount_SOURCES)

