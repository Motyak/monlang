## trace ##
%/common.o: trace_macro := $(if $(TRACE), -D TRACE)

## word ##
%/Word.o: word_macros := $(call buildmacros, DISABLE_, $(DISABLE_WORDS))
%/Word.o: word_macros += $(if $(DISABLE_SPECIAL_ATOMS), -D DISABLE_SPECIAL_ATOMS)

## postfix ##
%/Atom.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_ATOM)
%/ParenthesesGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_PG)
%/SquareBracketsGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_SBG)
%/CurlyBracketsGroup.o: postfix_macros := $(call buildmacros, DISABLE_P, $(DISABLE_POSTFIXES), %_IN_CBG)

## assoc ##
%/Atom.o: assoc_macro := $(call buildmacros, DISABLE_ASSOC_IN_, $(DISABLE_ASSOCS), ATOM)

macros = $(strip $(trace_macro) $(word_macros) $(postfix_macros) $(assoc_macro))
