// #include <Atom.h>
// #include <Program.h>
// #include <Term.h>
// #include <Association.h>
// #include <PostfixParenthesesGroup.h>
// #include <PostfixSquareBracketsGroup.h>
// #include <ParenthesesGroup.h>
// #include <SquareBracketsGroup.h>
// #include <CurlyBracketsGroup.h>
// #include <common.h>
// #include <utils/vec-utils.h>
// #include <utils/variant-utils.h>

// #include <iostream>
// #include <algorithm>

// std::variant<Atom, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*> consumeAtom(std::istringstream& input) {
//     auto atom = consumeAtomStrictly(input);

//     using PostfixLeftPart = std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
//     PostfixLeftPart accumulatedPostfixLeftPart = atom;

//     BEGIN:
//     if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
//         accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), *postfixParenthesesGroup};
//         goto BEGIN;
//     }

//     if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
//         accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), *postfixSquareBracketsGroup};
//         goto BEGIN;
//     }

//     if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
//         ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
//         input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
//         ProgramWord rightPart = consumeProgramWord(input);
//         return new Association{leftPart, rightPart};
//     }

//     return variant_cast(accumulatedPostfixLeftPart);
// }


// Atom consumeAtomStrictly(std::istringstream& input) {
//     static const std::vector<char> TERMINATOR_CHARACTERS = vec_union({
//         Program::RESERVED_CHARACTERS,
//         Term::RESERVED_CHARACTERS,
//         Association::RESERVED_CHARACTERS,
//         ParenthesesGroup::RESERVED_CHARACTERS,
//         SquareBracketsGroup::RESERVED_CHARACTERS,
//         CurlyBracketsGroup::RESERVED_CHARACTERS,
//         Quotation::RESERVED_CHARACTERS,
//     });

//     if (input.peek() == EOF) {
//         std::cerr << "unexpected EOF while about to parse an atom value" << std::endl;
//         throw std::runtime_error("user exception");
//     }

//     std::string value;
//     char currentChar;
//     while (input.peek() != EOF && std::all_of(
//                 TERMINATOR_CHARACTERS.begin(),
//                 TERMINATOR_CHARACTERS.end(),
//                 [&input](auto terminatorChar){return input.peek() != terminatorChar;})) {
//         input.get(currentChar);
//         value += currentChar;
//     }

//     if (value.size() == 0) {
//         std::cerr << "expected an atom value but found nothing" << std::endl;
//         throw std::runtime_error("user exception");
//     }

//     return Atom{value};
// }
