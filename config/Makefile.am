#==============================================================================
# The configuration files
#==============================================================================
pkgsysconfdir = $(sysconfdir)/$(PACKAGE)/config
dist_pkgsysconf_DATA = \
	graphism.conf opencity.xml graphism.xml

#==============================================================================
# Uninstall = delete directories
#==============================================================================
uninstall-hook:
	rmdir $(pkgsysconfdir)
	rmdir $(sysconfdir)/$(PACKAGE)
