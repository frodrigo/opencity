#==============================================================================
# OpenCity's main automake file
#==============================================================================

SUBDIRS = \
	src viewer


EXTRA_DIST = \
	graphism model sound texture COPYRIGHT


dist_pkgdata_DATA = \
	opencity.desktop opencity.png


nobase_dist_pkgdata_DATA = \
	autopackage/opencity.apspec


#==============================================================================
# "config" directory
#==============================================================================
nobase_dist_pkgdata_DATA += \
	config/graphism.conf config/opencity.xml


#==============================================================================
# "docs" directory
#==============================================================================
nobase_dist_pkgdata_DATA += \
	docs/FAQ.txt docs/FAQ_it.txt docs/README_it.txt


#==============================================================================
# Install all the EXTRA_DIST directories
#==============================================================================
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir);
	for data in $(EXTRA_DIST); do \
		if [ -d $(srcdir)/$$data ]; then \
			$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$$data && \
			cp -r $(srcdir)/$$data/* $(DESTDIR)$(pkgdatadir)/$$data/; \
		fi; \
	done;
#	$(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/;
#	$(INSTALL_DATA) $(srcdir)/opencity.png $(DESTDIR)$(datadir)/pixmaps/;
#	$(mkinstalldirs) $(DESTDIR)$(datadir)/applications/;
#	$(INSTALL_DATA) $(srcdir)/opencity.desktop $(DESTDIR)$(datadir)/applications/;

	@echo -e "\n\r";
	@echo "========================================================================";
	@echo "   OpenCity has been installed successfully in '$(DESTDIR)$(prefix)'";
	@echo "   Have fun !";
	@echo "========================================================================";
	@echo -e "\n\r";


#==============================================================================
# Uninstall = Delete everything
#==============================================================================
uninstall-local:
#	$(RM) -f $(DESTDIR)$(datadir)/pixmaps/opencity.png
#	$(RM) -f $(DESTDIR)$(datadir)/applications/opencity.desktop
	$(RM) -f $(DESTDIR)$(bindir)/opencity
	$(RM) -rf $(DESTDIR)$(pkgdatadir)


#==============================================================================
# Remove SVN directories from the distribution file
#==============================================================================
dist-hook:
	for extra in $(EXTRA_DIST); do \
		$(RM) -rf `find $(distdir)/$$extra -name "\.svn" -type d`; \
	done;


