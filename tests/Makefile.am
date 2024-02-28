AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBCREG_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYCREG = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_creginfo.sh \
	test_cregexport.sh \
	$(TESTS_PYCREG)

check_SCRIPTS = \
	pycreg_test_file.py \
	pycreg_test_support.py \
	test_cregexport.sh \
	test_creginfo.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	creg_test_data_block \
	creg_test_data_type \
	creg_test_error \
	creg_test_file \
	creg_test_io_handle \
	creg_test_key \
	creg_test_key_descriptor \
	creg_test_key_hierarchy_entry \
	creg_test_key_name_entry \
	creg_test_key_navigation \
	creg_test_notify \
	creg_test_support \
	creg_test_tools_info_handle \
	creg_test_tools_output \
	creg_test_tools_signal \
	creg_test_value \
	creg_test_value_entry

creg_test_data_block_SOURCES = \
	creg_test_data_block.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_data_block_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_data_type_SOURCES = \
	creg_test_data_type.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_data_type_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_error_SOURCES = \
	creg_test_error.c \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_unused.h

creg_test_error_LDADD = \
	../libcreg/libcreg.la

creg_test_file_SOURCES = \
	creg_test_file.c \
	creg_test_functions.c creg_test_functions.h \
	creg_test_getopt.c creg_test_getopt.h \
	creg_test_libbfio.h \
	creg_test_libcerror.h \
	creg_test_libclocale.h \
	creg_test_libcnotify.h \
	creg_test_libcreg.h \
	creg_test_libuna.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

creg_test_io_handle_SOURCES = \
	creg_test_io_handle.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_io_handle_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_key_SOURCES = \
	creg_test_key.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_key_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_key_descriptor_SOURCES = \
	creg_test_key_descriptor.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_key_descriptor_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_key_hierarchy_entry_SOURCES = \
	creg_test_key_hierarchy_entry.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_key_hierarchy_entry_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_key_name_entry_SOURCES = \
	creg_test_key_name_entry.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_key_name_entry_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_key_navigation_SOURCES = \
	creg_test_key_navigation.c \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h

creg_test_key_navigation_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_notify_SOURCES = \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_notify.c \
	creg_test_unused.h

creg_test_notify_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_support_SOURCES = \
	creg_test_functions.c creg_test_functions.h \
	creg_test_getopt.c creg_test_getopt.h \
	creg_test_libbfio.h \
	creg_test_libcerror.h \
	creg_test_libclocale.h \
	creg_test_libcnotify.h \
	creg_test_libcreg.h \
	creg_test_libuna.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_support.c \
	creg_test_unused.h

creg_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_tools_info_handle_SOURCES = \
	../cregtools/info_handle.c ../cregtools/info_handle.h \
	creg_test_libcerror.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_tools_info_handle.c \
	creg_test_unused.h

creg_test_tools_info_handle_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_tools_output_SOURCES = \
	../cregtools/cregtools_output.c ../cregtools/cregtools_output.h \
	creg_test_libcerror.h \
	creg_test_macros.h \
	creg_test_tools_output.c \
	creg_test_unused.h

creg_test_tools_output_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_tools_signal_SOURCES = \
	../cregtools/cregtools_signal.c ../cregtools/cregtools_signal.h \
	creg_test_libcerror.h \
	creg_test_macros.h \
	creg_test_tools_signal.c \
	creg_test_unused.h

creg_test_tools_signal_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_value_SOURCES = \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h \
	creg_test_value.c

creg_test_value_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

creg_test_value_entry_SOURCES = \
	creg_test_libcerror.h \
	creg_test_libcreg.h \
	creg_test_macros.h \
	creg_test_memory.c creg_test_memory.h \
	creg_test_unused.h \
	creg_test_value_entry.c

creg_test_value_entry_LDADD = \
	../libcreg/libcreg.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

