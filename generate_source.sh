#!/bin/bash

## ParenthesesGroup.h ##
sed -e 's/SQUARE_BRACKETS/PARENTHESES/g' \
    -e 's/SquareBrackets/Parentheses/g' \
    include/monlang/SquareBracketsGroup.h > include/monlang/ParenthesesGroup.h
echo "Generated ParenthesesGroup.h"

## ParenthesesGroup.cpp ##
perl -pe 's/SquareBrackets/Parentheses/g' src/SquareBracketsGroup.cpp \
    | perl -pe "s/\QINITIATOR_SEQUENCE = {'['};\E/INITIATOR_SEQUENCE = {'('};/" \
    | perl -pe "s/\QTERMINATOR_SEQUENCE = {']'};\E/TERMINATOR_SEQUENCE = {')'};/" \
    | perl -pe 's/43/42/g' \
    | perl -pe 's/403/402/g' \
    > src/ParenthesesGroup.cpp
echo "Generated ParenthesesGroup.cpp"
