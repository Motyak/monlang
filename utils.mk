# buildmake: function that builds a target dir Makefile if necessary
# $(1): path of target dir containing the Makefile to build
# returns: true if a rebuild is necessary, empty string otherwise. ..
# .. assign make exit status to variable .BUILDMAKESTATUS
define buildmake
	$(shell \
		if ! $(MAKE) -qsC $(1); then \
			echo -n true; \
			>&2 $(MAKE) -C $(1); \
		fi \
	)
	$(eval .BUILDMAKESTATUS := $(.SHELLSTATUS))
endef
