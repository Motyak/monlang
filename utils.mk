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

# askmake: invoke make -q for a target dir
# $(1): target dir
# returns: true if the target needs to be rebuild, false otherwise
define askmake
$(shell \
	if ! $(MAKE) -qsC $(1); then \
		echo true; \
	fi)
endef

# buildmake: generate the commands to make the target
# $(1): target dir
# returns: the commands to make the target
define buildmake
$(MAKE) -C $(1); exitcode=$$?; \
if [ $$exitcode -ne 0 ]; then exit $$exitcode; fi
endef

# shouldrebuild: check if target is missing or outdated based on dependencies
# $(1): target
# $(2): dependencies
# returns true if target needs to be rebuilt, empty string otherwise
define shouldrebuild
$(shell \
	if [ ! -e $(1) ]; then { echo true; exit 0; }; fi; \
	for lib in $(2); do \
		if [ ! -e $$lib ]; then { echo true; exit 0; }; fi; \
		if [ $$lib -nt $(1) ]; then { echo true; exit 0; }; fi; \
	done)
endef

# clean: run 'clean' target recipe in a verbose shell, support dry run as well
# $(1): shell command
# returns: nothing
define clean
$(if $(filter clean,$(MAKECMDGOALS)), \
	$(if $(findstring n, $(firstword -$(MAKEFLAGS))), \
		$(shell $(SHELL) -vnc '$(strip $(1))'), \
		$(shell $(SHELL) -vc '$(strip $(1))')
	)
)
endef
