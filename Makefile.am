SUBDIRS = \
	src viewer

EXTRA_DIST = \
	config docs graphism model sound texture \
	OpenCity.desktop OpenCity.png \
	autopackage

#----------------------------------- Install all the EXTRA_DIST directories
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir);
	for data in $(EXTRA_DIST); do \
		if [ -d $(srcdir)/$$data ]; then \
			$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/$$data && \
			cp -r $(srcdir)/$$data/* $(DESTDIR)$(pkgdatadir)/$$data/; \
		fi; \
	done;
#	$(INSTALL_DATA) config/opencity.conf $(DESTDIR)$(pkgdatadir)/config/;
	$(INSTALL_DATA) sound/sound.m3u $(DESTDIR)$(pkgdatadir)/sound/;
	$(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/;
	$(INSTALL_DATA) $(srcdir)/OpenCity.png $(DESTDIR)$(datadir)/pixmaps/;
	$(mkinstalldirs) $(DESTDIR)$(datadir)/applications/;
	$(INSTALL_DATA) $(srcdir)/OpenCity.desktop $(DESTDIR)$(datadir)/applications/;

	@echo -e "\n\r";
	@echo "========================================================================";
	@echo "   OpenCity has been installed successfully in '$(DESTDIR)$(prefix)'";
	@echo "   Have fun !";
	@echo "========================================================================";
	@echo -e "\n\r";

#----------------------------------- Uninstall = Delete everything
uninstall-local:
	$(RM) -f $(DESTDIR)$(datadir)/pixmaps/OpenCity.png
	$(RM) -f $(DESTDIR)$(datadir)/applications/OpenCity.desktop
	$(RM) -f $(DESTDIR)$(bindir)/opencity
	$(RM) -rf $(DESTDIR)$(pkgdatadir)

#----------------------------------- Remove CVS directories
dist-hook:
	for extra in $(EXTRA_DIST); do \
		$(RM) -rf `find $(distdir)/$$extra -name "CVS" -type d`; \
	done;


