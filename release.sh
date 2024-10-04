#!/bin/bash
shopt -s nullglob globstar
set -o errexit

## copy public header files ##
rm -rf dist/monlang-parser; mkdir -p $_
cp -r include/monlang/* -t dist/monlang-parser/
for header in dist/monlang-parser/**/*.h; do
    perl -i -pe 's/<monlang\//<monlang-parser\//g' $header;
done

## add LV1:: namespace for each entity in public headers ##
cd dist/monlang-parser
for header in [A-Z]*.h; do
    camelcase_entity_name=${header%.h*}
    uppercase_entity_name=$(
        cat <<< $camelcase_entity_name \
        | perl -pe 's/([A-Z])/_$1/g' \
        | perl -pe 'tr/[a-z]/[A-Z]/' \
        | tail -c+2 # remove heading underscore
    )
    CC=$camelcase_entity_name UPP=$uppercase_entity_name \
        perl -i -pe 's/(#endif \/\/ $ENV{UPP}_H)/namespace LV1 { using $ENV{CC} = ::$ENV{CC}; }\n\n$1/' $header
done
cd - > /dev/null

## add LV1:: namespace for Ast and AstVisitor in public visitor.h ##
visitor_h=dist/monlang-parser/visitors/visitor.h
added_code=$(
cat <<'EOF'
namespace LV1 {
    using Ast = ::Ast;
    using AstVisitor = ::AstVisitor;
}
EOF
)
perl -i -pe 's/(#endif \/\/ VISITOR_H)/'"$added_code"'\n\n$1/' $visitor_h

## package release objects ##
ar rcs dist/monlang-parser.a obj/release/**/*.o
