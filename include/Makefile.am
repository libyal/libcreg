include_HEADERS = \
	libcreg.h

pkginclude_HEADERS = \
	libcreg/codepage.h \
	libcreg/definitions.h \
	libcreg/error.h \
	libcreg/extern.h \
	libcreg/features.h \
	libcreg/types.h

EXTRA_DIST = \
	libcreg.h.in \
	libcreg/definitions.h.in \
	libcreg/features.h.in \
	libcreg/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libcreg.h
	/bin/rm -f libcreg/definitions.h
	/bin/rm -f libcreg/features.h
	/bin/rm -f libcreg/types.h
	/bin/rm -f Makefile

