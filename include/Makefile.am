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
	-rm -f libcreg.h
	-rm -f libcreg/definitions.h
	-rm -f libcreg/features.h
	-rm -f libcreg/types.h
	-rm -f Makefile

