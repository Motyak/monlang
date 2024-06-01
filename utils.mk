# buildmake: function that builds a target dir Makefile if necessary
# $(1): path of target dir containing the Makefile to build
# returns: true if a rebuild is necessary, empty string otherwise. ..
# .. assign make exit status to variable .BUILDMAKESTATUS
define buildmake
$(shell \
	if ! $(MAKE) -qsC $(1); then \
		echo -n true; \
		>&2 $(MAKE) -$(MAKEFLAGS) -C $(1); \
	fi \
)
$(eval .BUILDMAKESTATUS := $(.SHELLSTATUS))
endef

# fileexists: function that check whether or not a file is missing
# $(1): path of the file to check
# returns: true if the file is missing, empty string otherwise
define missingfile
$(shell \
	if [ ! -e $(1) ]; then \
		echo -n true; \
	fi \
)
endef
