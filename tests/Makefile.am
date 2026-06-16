AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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

check_PYTHONdir = $(abs_top_builddir)

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_PYTHON_SCRIPTS = \
	pycreg_test_file.py \
	pycreg_test_support.py

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
	creg_test_tools_mount_path_string \
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

creg_test_tools_mount_path_string_SOURCES = \
	../cregtools/mount_path_string.c ../cregtools/mount_path_string.h \
	creg_test_libcerror.h \
	creg_test_libcpath.h \
	creg_test_macros.h \
	creg_test_tools_mount_path_string.c \
	creg_test_unused.h

creg_test_tools_mount_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
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
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libcreg.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libcreg

test_inputs_pycreg.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pycreg

test_inputs_creginfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) creginfo

test_inputs_creginfo_hierarchy.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) creginfo_hierarchy

test_inputs_cregexport.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) cregexport

test_library: \
	package.m4 \
	test_inputs_libcreg.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pycreg.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_cregexport.at \
	test_inputs_creginfo.at \
	test_inputs_creginfo_hierarchy.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

