#!/bin/bash
shopt -s nullglob globstar
set -o errexit -o pipefail

## we expect at least one header file and one obj file ##
(shopt -s failglob; : include/monlang/**/*.h)
(shopt -s failglob; : obj/*.o)

exec 9> /tmp/monlang_trace.txt
export BASH_XTRACEFD=9
trap '{ echo RELEASE FAILED, trace was saved in /tmp/monlang_trace.txt; } 9>/dev/null' ERR
set -o xtrace

[ "$1" == "-B" ] && rm -rf .release
mkdir -p dist/monlang-LV1 .release/monlang 

## package objects (in background job) ##
if [ "$1" == "-B" ]; then
    ar rcsvD dist/monlang-LV1.a obj/**/*.o & package_proc_id=$!
else
    ar rcsvD dist/monlang-LV1.a ${@:-obj/**/*.o} & package_proc_id=$!
fi

## copy public header files ##
updated_headers="$(rsync -rc --out-format='%f' include/monlang/ .release/monlang/ | { grep '.h$' || true; })"
for header in $updated_headers; do
    target_file=${header/include\/monlang/dist\/monlang-LV1}
    mkdir -p ${target_file%/*}
    cp $header $target_file
    perl -i -pe 's/<monlang\//<monlang-LV1\//g' $target_file
done

## add LV1:: namespace for each entity in public ast/ headers ##
cd dist/monlang-LV1/ast
for header in [A-Z]*.h; do
    grep -q "^include/monlang/ast/${header}$" <<< "$updated_headers" || continue
    camelcase_entity_name=${header%.h*}
    uppercase_entity_name=$(
        cat <<< $camelcase_entity_name \
        | perl -pe 's/([A-Z])/_$1/g' \
        | perl -pe 'tr/[a-z]/[A-Z]/' \
        | tail -c+2 # remove leading underscore
    )
    CC=$camelcase_entity_name UPP=$uppercase_entity_name \
        perl -i -pe 's/(#endif \/\/ AST_$ENV{UPP}_H)/namespace LV1 { using $ENV{CC} = ::$ENV{CC}; }\n\n$1/' $header
done
cd - > /dev/null

## add LV1:: namespace for ProgramWord in public ast/Word.h ##
word_h=dist/monlang-LV1/ast/Word.h
added_code=$(
cat <<'EOF'
namespace LV1 { using ProgramWord = ::ProgramWord; }
EOF
)
grep -q "^${word_h/dist\/monlang-LV1/include\/monlang}$" <<< "$updated_headers" && {
    perl -i -pe 's/(#endif \/\/ AST_WORD_H)/'"$added_code"'\n\n$1/' $word_h
}

## add LV1:: namespace for Ast in public ast/visitor.h ##
visitor_h=dist/monlang-LV1/ast/visitors/visitor.h
added_code=$(
cat <<'EOF'
namespace LV1 { using Ast = ::Ast; }
EOF
)
grep -q "^${visitor_h/dist\/monlang-LV1/include\/monlang}$" <<< "$updated_headers" && {
    perl -i -pe 's/(#endif \/\/ AST_VISITOR_H)/'"$added_code"'\n\n$1/' $visitor_h
}

## add LV1:: namespace for Ast_ in public visitor.h ##
visitor_h=dist/monlang-LV1/visitors/visitor.h
added_code=$(
cat <<'EOF'
namespace LV1 { using Ast_ = ::Ast_; }
EOF
)
grep -q "^${visitor_h/dist\/monlang-LV1/include\/monlang}$" <<< "$updated_headers" && {
    perl -i -pe 's/(#endif \/\/ VISITOR_H)/'"$added_code"'\n\n$1/' $visitor_h
}

wait $package_proc_id

echo -e "\nupdated headers:\n${updated_headers:-<none>}\n"

echo RELEASE DONE
