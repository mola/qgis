#############################################################################
# Makefile for building: qgis
# Generated by qmake (1.04a) (Qt 3.1.1) on: Sun Aug 31 16:57:57 2003
# Project:  qgis.pro
# Template: subdirs
# Command: $(QMAKE) -o Makefile qgis.pro
#############################################################################

MAKEFILE =	Makefile
QMAKE    =	qmake
DEL_FILE =    rm -f
SUBTARGETS =	 \
		sub-src \
		sub-plugins

first: all

all: Makefile $(SUBTARGETS)

src/$(MAKEFILE): 
	cd src && $(QMAKE)  -o $(MAKEFILE)
sub-src: src/$(MAKEFILE) FORCE
	cd src && $(MAKE) -f $(MAKEFILE)

plugins/$(MAKEFILE): 
	cd plugins && $(QMAKE)  -o $(MAKEFILE)
sub-plugins: plugins/$(MAKEFILE) FORCE
	cd plugins && $(MAKE) -f $(MAKEFILE)

Makefile: qgis.pro  $(QTDIR)/mkspecs/default/qmake.conf 
	$(QMAKE) -o Makefile qgis.pro
qmake: qmake_all
	@$(QMAKE) -o Makefile qgis.pro

all: $(SUBTARGETS)
qmake_all: src/$(MAKEFILE) plugins/$(MAKEFILE)
	 ( [ -d src ] && cd src ; grep "^qmake_all:" $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) qmake_all; ) || true
	 ( [ -d plugins ] && cd plugins ; grep "^qmake_all:" $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) qmake_all; ) || true
clean uninstall install uiclean mocclean lexclean yaccclean: qmake_all FORCE
	 ( [ -d src ] && cd src ; $(MAKE) -f $(MAKEFILE) $@; ) || true
	 ( [ -d plugins ] && cd plugins ; $(MAKE) -f $(MAKEFILE) $@; ) || true
distclean: qmake_all FORCE
	 ( [ -d src ] && cd src ; $(MAKE) -f $(MAKEFILE) $@; $(DEL_FILE) $(MAKEFILE); ) || true
	 ( [ -d plugins ] && cd plugins ; $(MAKE) -f $(MAKEFILE) $@; $(DEL_FILE) $(MAKEFILE); ) || true

FORCE:

