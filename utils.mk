comma := ,
empty :=
space := $(empty) $(empty)

# shell_onrun: same as $(shell) but doesn't evaluate during make autocompletion
# $(1): code to evaluate
# returns: evaluated code on run, the empty str otherwise
define shell_onrun
$(if $(filter .DEFAULT,$(MAKECMDGOALS)),,$(shell $(strip $(1))))
endef

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
# NOTE: outer shell expression is only used to split in separate line..
#       ..multiple `$(..)` expressions in a defined function.
#       The later oneline would have been equivalent, but is uglier:
#       $(shell ! $(MAKE) -qsC $(1) && echo true)$(if $(.SHELLSTATUS:1=),$(eval __SHOULDREBUILD += $(1)))
define askmake
$(shell echo \
	$(shell ! $(MAKE) -qsC $(1) && echo true) \
	$(if $(.SHELLSTATUS:1=),$(eval __SHOULDREBUILD += $(1))))
endef

# buildmake: (allows us to hide sub-make commands in dry run mode)
# $(1): target dir
define buildmake
$(MAKE) -C $(strip $(1))
endef

# shouldrebuild: check if target is missing or outdated based on dependencies
# $(1): target
# $(2): dependencies
# returns true if target needs to be rebuilt, empty string otherwise
# NOTE: outer shell expression is only used to split in separate line..
#       ..multiple `$(..)` expressions in a defined function.
define shouldrebuild
$(shell echo \
	$(shell
		if [ ! -e $(1) ]; then { echo true; exit 0; }; fi; \
		for lib in $(2); do \
			if [ ! -e $$lib ]; then { echo true; exit 0; }; fi; \
			if [ $$lib -nt $(1) ]; then { echo true; exit 0; }; fi; \
		done) \
	$(__SHOULDREBUILD) # required in dry run mode
)
endef

# clean: run 'clean' target recipe in a verbose shell, support dry run as well
# $(1): shell command
# returns: nothing
define clean
$(if $(filter clean,$(MAKECMDGOALS)),
	$(if $(findstring n, $(firstword -$(MAKEFLAGS))),
		$(shell $(SHELL) -vnc '$(strip $(1))'),
		$(shell $(SHELL) -vc '$(strip $(1))')
	)
)
endef
