# buildmake: function that builds a target dir Makefile if necessary
# $(1): path of target dir containing the Makefile to build
# returns: true if a rebuild is necessary, empty string otherwise. ..
# .. assign make exit status to variable .BUILDMAKESTATUS
define buildmake
$(shell \
	if ! $(MAKE) -qsC $(1); then \
		echo true; \
		>&2 $(MAKE) -C $(1); \
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
		echo true; \
	fi \
)
endef

# ifnotmakeflag: similar as $(if) but checks for a make flag absence
# $(1): make flags to check (separated by space)
# $(2): output str
# returns: the output str if all make flags aren't set, the empty str if any is
define ifnotmakeflag
$(if $(strip $(foreach flag,$(1),$(findstring $(flag),$(firstword -$(MAKEFLAGS))))),,$(2))
endef

# clean: run 'clean' target recipe in a verbose shell, support dry run as well
# $(1): shell command
# returns: nothing
define clean
$(if $(filter clean,$(MAKECMDGOALS)), \
	$(if $(findstring n, $(firstword -$(MAKEFLAGS))), \
		$(shell $(SHELL) -vnc '$(1)'), \
		$(shell $(SHELL) -vc '$(1)')
	)
)
endef
