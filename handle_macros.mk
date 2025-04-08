## trace ##
%/common.o: trace_macro := $(if $(TRACE), -D TRACE)

## word ##
%/Word.o: word_macros := $(call buildmacros, DISABLE_, $(DISABLE_WORDS))
%/Word.o: word_macros += $(if $(DISABLE_POSTFIXES), -D DISABLE_POSTFIXES)
%/Word.o: word_macros += $(if $(DISABLE_ASSOCS), -D DISABLE_ASSOCS)
%/Word.o: word_macros += $(if $(DISABLE_SPECIAL_ATOMS), -D DISABLE_SPECIAL_ATOMS)
%/Word.o: word_macros += $(if $(DISABLE_DOLLARS_CBG), -D DISABLE_DOLLARS_CBG)
%/Word.o: word_macros += $(if $(DISABLE_PSBG_SUFFICES), -D DISABLE_PSBG_SUFFICES)

macros = $(strip $(trace_macro) $(word_macros))
