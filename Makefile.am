#==============================================================================
# OpenCity's main automake file
#==============================================================================
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = config docs map sound src viewer


#==============================================================================
# EXTRA_DIST is used to package the files we need
# See the install-data-local target for more information
#==============================================================================
EXTRA_DIST = \
	graphism \
	opencity.desktop opencity.png \
	autopackage/opencity.apspec


#==============================================================================
# Icon and desktop file installation
#==============================================================================
desktopdir = $(datadir)/applications
desktop_DATA = opencity.desktop

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = opencity.png


#==============================================================================
# Install all the graphism directory
#==============================================================================
install-data-local:
	test -z "$(pkgdatadir)" || $(mkdir_p) "$(DESTDIR)$(pkgdatadir)"
	for data in $(EXTRA_DIST); do \
		if [ -d "$(srcdir)/$$data" ]; then \
			cp -r "$(srcdir)/$$data" "$(DESTDIR)$(pkgdatadir)"/; \
		fi; \
	done;

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
	$(RM) -f $(DESTDIR)$(bindir)/opencity
	$(RM) -rf $(DESTDIR)$(pkgdatadir)

	@echo -e "\n\r";
	@echo "========================================================================";
	@echo "             OpenCity has been uninstalled successfully                 ";
	@echo "                       Thank you for trying.                            ";
	@echo "========================================================================";
	@echo -e "\n\r";


#==============================================================================
# Remove SVN directories from the distribution file
#==============================================================================
dist-hook:
	for extra in $(EXTRA_DIST); do \
		$(RM) -rf `find $(distdir)/$$extra -name "\.svn" -type d`; \
	done;


