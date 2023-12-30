#include <Atom.h>
// #include <Program.h>
#include <Term.h>
// #include <Association.h>
// #include <PostfixParenthesesGroup.h>
// #include <PostfixSquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
// #include <CurlyBracketsGroup.h>
#include <common.h>
#include <utils/vec-utils.h>
#include <utils/variant-utils.h>

#include <iostream>
#include <algorithm>

static MayFail<std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(const Atom&, std::istringstream&);

MayFail<std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> consumeAtom(std::istringstream& input) {
    auto atom = consumeAtomStrictly(input);

    return atom.and_then([&input](auto atom){return lookBehind(atom, input);});
}

static MayFail<std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(const Atom& atom, std::istringstream& input) {
    using PostfixLeftPart = std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = atom;

    // BEGIN:
    // if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
    //     if (!*postfixParenthesesGroup) {
    //         return std::unexpected(Error{102, new Error{postfixParenthesesGroup->error()}});
    //     }
    //     accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), **postfixParenthesesGroup};
    //     goto BEGIN;
    // }

    // if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
    //     if (!*postfixSquareBracketsGroup) {
    //         return std::unexpected(Error{103, new Error{postfixSquareBracketsGroup->error()}});
    //     }
    //     accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), **postfixSquareBracketsGroup};
    //     goto BEGIN;
    // }

    // if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
    //     ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
    //     input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
    //     if (auto rightPart = consumeProgramWord(input); !rightPart) {
    //         return std::unexpected(Error{104, new Error{rightPart.error()}});
    //     } else {
    //         return new Association{leftPart, *rightPart};
    //     }
    // }

    return variant_cast(accumulatedPostfixLeftPart);
}

MayFail<Atom> consumeAtomStrictly(std::istringstream& input) {
    static const std::vector<char> TERMINATOR_CHARACTERS = vec_union({
        // Program::RESERVED_CHARACTERS,
        Term::RESERVED_CHARACTERS,
        // Association::RESERVED_CHARACTERS,
        ParenthesesGroup::RESERVED_CHARACTERS,
        SquareBracketsGroup::RESERVED_CHARACTERS,
        // CurlyBracketsGroup::RESERVED_CHARACTERS,
        // Quotation::RESERVED_CHARACTERS,
    });

    if (input.peek() == EOF) {
        return std::unexpected(Error{117});
    }

    std::string value;
    char currentChar;
    while (input.peek() != EOF && std::all_of(
            TERMINATOR_CHARACTERS.begin(),
            TERMINATOR_CHARACTERS.end(),
            [&input](auto terminatorChar){return input.peek() != terminatorChar;})) {
        input.get(currentChar);
        value += currentChar;
    }

    if (value.size() == 0) {
        return std::unexpected(Error{118});
    }

    return Atom{value};
}
