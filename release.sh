#!/bin/bash
shopt -s nullglob globstar
set -o errexit

## copy public header files ##
rm -rf dist/monlang-parser; mkdir -p $_
cp -r include/monlang/* -t dist/monlang-parser/
for header in dist/monlang-parser/**/*.h; do
    perl -i -pe 's/<monlang\//<monlang-parser\//g' $header;
done

## package release objects ##
ar rcs dist/monlang-parser.a obj/release/**/*.o
