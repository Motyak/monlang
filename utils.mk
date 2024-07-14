# not: logical not
# $(1): input str
# returns: true if input str is empty, the empty str otherwise
define not
$(if $(strip $(1)),,true)
endef

# checkmakeflags: check if any make flags are set
# $(1): make flags to check (separated by space)
# returns: the flags that are set, the empty str if none
define checkmakeflags
$(strip $(foreach flag,$(1),$(findstring $(flag),$(firstword -$(MAKEFLAGS)))))
endef

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
	$(eval .BUILDMAKESTATUS := $(.SHELLSTATUS)) \
)
endef

# shouldrebuild: check if target is missing or outdated based on dependencies
# $(1): target
# $(2): dependencies
# returns true if target needs to be rebuilt, empty string otherwise
define shouldrebuild
$(shell \
	[ ! -e $(1) ] && { echo true; exit 0; }; \
	for lib in $(2); do \
		[ $$lib -nt $(1) ] && { echo true; exit 0; }; \
	done)
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
