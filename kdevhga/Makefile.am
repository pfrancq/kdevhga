####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kdevhga
kdevhga_SOURCES = gawords.cpp qlistwords.cpp khgaprjview.cpp khgaheuristicview.cpp khgagaview.cpp kappoptions.ui kdevhgaview.cpp kdevhgadoc.cpp kdevhga.cpp main.cpp 
kdevhga_LDADD   = ./r/rga/librga.a ./r/rhga/librhga.a ./r/rio/librio.a ./r/rmath/librmath.a ./r/rstd/librstd.a ./r/rxml/librxml.a ./qtwidgets/rga/librga.a ./r/rxml/librxml.a ./r/rio/librio.a ./r/rstd/librstd.a ./r/rmath/librmath.a ./r/rga/librga.a ./r/rhga/librhga.a ./qtwidgets/rga/librga.a $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = r qtwidgets 

EXTRA_DIST = main.cpp kdevhga.cpp kdevhga.h kdevhgadoc.cpp kdevhgadoc.h kdevhgaview.cpp kdevhgaview.h kdevhgaui.rc kdevhga.desktop lo32-app-kdevhga.png lo16-app-kdevhga.png kappoptions.ui khgagaview.cpp khgagaview.h khgaheuristicview.cpp khgaheuristicview.h khgaprjview.cpp khgaprjview.h qlistwords.cpp qlistwords.h gawords.h gawords.cpp 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kdevhga.desktop $(kde_appsdir)/Applications/kdevhga.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kdevhga.png $(kde_icondir)/locolor/32x32/apps/kdevhga.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kdevhga.png $(kde_icondir)/locolor/16x16/apps/kdevhga.png

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/kdevhga.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/kdevhga.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/kdevhga.png

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
kdevhga_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/kdevhga
rc_DATA = kdevhgaui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kdevhga.pot; \
	fi

