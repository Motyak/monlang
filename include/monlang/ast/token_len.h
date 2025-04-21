/*
    standalone header, no .cpp

    This header should be included when one..
    ..needs to use token_len on ALL definitions..
    ..of LV1 ast/

    If you only need token_len for a couple of entities..
    .., then simply include monlang/ast/common.h and..
    ..the corresponding entity headers
*/

#ifndef AST_TOKEN_LEN_H
#define AST_TOKEN_LEN_H

#include <monlang/ast/common.h> // token_len() set_token_len() templates

/* all LV1 ast/ entities */
#include <monlang/ast/Atom.h>
#include <monlang/ast/Quotation.h>
#include <monlang/ast/SquareBracketsTerm.h>
#include <monlang/ast/ParenthesesGroup.h>
#include <monlang/ast/SquareBracketsGroup.h>
#include <monlang/ast/MultilineSquareBracketsGroup.h>
#include <monlang/ast/CurlyBracketsGroup.h>
#include <monlang/ast/PostfixParenthesesGroup.h>
#include <monlang/ast/PostfixSquareBracketsGroup.h>
#include <monlang/ast/Path.h>
#include <monlang/ast/Association.h>

#endif // AST_TOKEN_LEN_H
