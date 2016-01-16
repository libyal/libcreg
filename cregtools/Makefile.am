AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFWSI_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBCREG_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	cregexport \
	creginfo \
	cregmount

cregexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	cregexport.c \
	cregoutput.c cregoutput.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcreg.h \
	cregtools_libcstring.h \
	cregtools_libcsystem.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libfdatetime.h \
	cregtools_libfguid.h \
	cregtools_libfwnt.h \
	cregtools_libfwsi.h \
	cregtools_libuna.h

cregexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

creginfo_SOURCES = \
	info_handle.c info_handle.h \
	creginfo.c \
	cregoutput.c cregoutput.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcreg.h \
	cregtools_libcstring.h \
	cregtools_libcsystem.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libfdatetime.h \
	cregtools_libfguid.h \
	cregtools_libfwnt.h \
	cregtools_libfwsi.h \
	cregtools_libuna.h

creginfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

cregmount_SOURCES = \
	mount_handle.c mount_handle.h \
	cregmount.c \
	cregoutput.c cregoutput.h \
	cregtools_libbfio.h \
	cregtools_libcerror.h \
	cregtools_libclocale.h \
	cregtools_libcnotify.h \
	cregtools_libcreg.h \
	cregtools_libcstring.h \
	cregtools_libcsystem.h \
	cregtools_libfcache.h \
	cregtools_libfdata.h \
	cregtools_libfdatetime.h \
	cregtools_libfguid.h \
	cregtools_libfwnt.h \
	cregtools_libfwsi.h \
	cregtools_libuna.h

cregmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on cregexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(cregexport_SOURCES)
	@echo "Running splint on creginfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(creginfo_SOURCES)
	@echo "Running splint on cregmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(cregmount_SOURCES)

