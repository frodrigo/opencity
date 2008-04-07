#==============================================================================
# Documentation
#==============================================================================
EXTRA_DIST = $(docs_DATA) $(man_MANS)

docsdir = $(datadir)/doc/$(PACKAGE)
docs_DATA = \
	../COPYRIGHT ../README ../AUTHORS ../INSTALL FAQ.txt \
	README_es.txt \
	FAQ_it.txt INSTALL_it.txt README_it.txt


man_MANS = opencity.6

#==============================================================================
# Uninstall = delete directory
#==============================================================================
uninstall-hook:
	rmdir $(docsdir)
