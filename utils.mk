# buildmake: function that builds a target dir Makefile if necessary
# $(1): path of target dir containing the Makefile to build
# returns: true if a rebuild is necessary, empty string otherwise. ..
# .. assign make exit status to variable .BUILDMAKESTATUS
define buildmake
$(shell \
	if ! $(MAKE) -qsC $(1); then \
		echo -n true; \
		>&2 $(MAKE) -C $(1) -$(MAKEFLAGS); \
	fi \
)
$(eval .BUILDMAKESTATUS := $(.SHELLSTATUS))
endef

# missingfile: function that check whether or not a file is missing
# $(1): path of the file to check
# returns: true if the file is missing, empty string otherwise
define missingfile
$(shell \
	if [ ! -e $(1) ]; then \
		echo -n true; \
	fi \
)
endef

# ifnotmakeflag: similar as $(if) but checks for a make flag absence
# $(1): make flag to check
# $(2): output str
# returns: the output str if the make flag is NOT set, the empty str if it is set
define ifnotmakeflag
$(if $(findstring $(strip $(1)), $(firstword -$(MAKEFLAGS))),, $(2))
endef

# clean: run 'clean' target recipe in a verbose shell
# $(1): shell command
# returns: nothing
define clean
$(if $(filter clean,$(MAKECMDGOALS)), $(shell $(SHELL) -vc '$(1)'))
endef
