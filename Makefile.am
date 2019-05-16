# Top level Makefile for the CppEphem repository

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = $(BUILDSUBDIRS)

install-data-local:
	$(MKDIR_P) $(cecorrfilepath)

uninstall-local:
	$(cleanup_corrfilepath)
    