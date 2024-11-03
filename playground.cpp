
#include <monlang/Term.h>
#include <monlang/Atom.h>
#include <monlang/ParenthesesGroup.h>

#include <monlang/visitors/Unparse.h>

#include <utils/str-utils.h>
#include <utils/assert-utils.h>
#include <utils/vec-utils.h>

#include <cstdint>

#define UINT uint32_t
#define INT int32_t

// TERM CTOR //////////////////////////////////////////////

MayFail<Word> Atom_(std::string val) {
    return mayfail_convert<Word>(MayFail<Atom>(Atom{val}));
}

// explicit cast
Atom Atom_(MayFail<Word> word) {
    return *std::get<Atom*>(*word);
}

Term Term_(std::string operation) {
    auto str_vec = split(operation, " ");
    std::vector<MayFail<Word>> words;
    for (auto token: str_vec) {
        words.push_back(Atom_(token));
    }
    return Term{words};
}

// TERM SELECTORS /////////////////////////////////////////

struct Operator {
    std::string val;
    UINT pos;
};

std::string stringify(MayFail<Word> word) {
    std::ostringstream oss;
    visitAst(Unparse(oss), word);
    return oss.str();
}

Operator optr(Term term, INT nth) {
    ASSERT (nth != 0);
    ASSERT (abs(nth) * 2 <= term.words.size());

    UINT pos;
    if (nth > 0) {
        pos = UINT(nth) * 2 - 1;
    }
    else /* if nth < 0 */ {
        pos = term.words.size() - 2 * abs(nth);
    }

    return Operator{Atom_(term.words.at(pos)).value, pos};
}

std::string left_opnd(Term term, Operator op) {
    // word is either an Atom or a ParenthesesGroup
    return stringify(term.words.at(op.pos - 1));
}

std::string right_opnd(Term term, Operator op) {
    // word is either an Atom or a ParenthesesGroup
    return stringify(term.words.at(op.pos + 1));
}

std::optional<Operator> prev_optr(Term term, Operator op) {
    if (op.pos < 2) {
        return {};
    }
    UINT pos = op.pos - 2;
    return Operator{Atom_(term.words.at(pos)).value, pos};
}

std::optional<Operator> next_optr(Term term, Operator op) {
    if (op.pos >= term.words.size() - 2) {
        return {};
    }
    UINT pos = op.pos + 2;
    return Operator{Atom_(term.words.at(pos)).value, pos};
}

// TERM MUTATOR ///////////////////////////////////////////

Term Term_(Term term, Operator op) {
    std::vector<MayFail<Word>> words;

    words.push_back(Atom_(left_opnd(term, op)));
    words.push_back(Atom_(op.val));
    words.push_back(Atom_(right_opnd(term, op)));

    return Term{words};
}

ParenthesesGroup ParenthesesGroup_(Term term, Operator op) {
    return ParenthesesGroup{{Term_(term, op)}};
}

// explicit cast
MayFail<Word> Word_(ParenthesesGroup pg) {
    return mayfail_convert<Word>(MayFail<ParenthesesGroup>(pg));
}

void parenthesize_optn(Term* term, Operator op) {
    auto operation = ParenthesesGroup_(*term, op);

    // copy all words BEFORE operation
    auto left_words = std::vector<MayFail<Word>>(term->words.begin(), term->words.begin() + op.pos - 1);

    // copy all words AFTER operation
    auto right_words = std::vector<MayFail<Word>>(term->words.begin() + op.pos + 2, term->words.end());

    // concat both ends with the new parenthesized operation
    std::vector<MayFail<Word>> new_words = vec_concat({
        left_words,
        {Word_(operation)},
        right_words,
    });

    // update &term with new value
    term->words = new_words;
}

std::ostream& operator<<(std::ostream& os, Term term) {
    /* first wrap term into parentheses group, to convert it into an AST (Word) */
    auto pg = ParenthesesGroup{{MayFail<Term>(term)}};
    auto word = mayfail_convert<Word>(MayFail<ParenthesesGroup>(pg));

    visitAst(Unparse(os), word);
    return os;
}

#ifdef TEST_OPTR
// g++ -D TEST_OPTR -o playground.elf playground.cpp --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
int main()
{
    Term input = Term_("1 + 2 * 2 ^ 3 ^ 2 + 91 ^ 1");

    {
        auto op = optr(input, 1);
        std::cout << "#1 <=> [" << op.pos << "]: " << op.val << std::endl;
    }

    {
        auto op = optr(input, -6);
        std::cout << "#-6 <=> [" << op.pos << "]: " << op.val << std::endl;
    }

    {
        auto op = optr(input, -1);
        std::cout << "#-1 <=> [" << op.pos << "]: " << op.val << std::endl;
    }

    {
        auto op = optr(input, 6);
        std::cout << "#6 <=> [" << op.pos << "]: " << op.val << std::endl;
    }
}
#endif // TEST_OPTR

#ifdef TEST_OPND
// g++ -D TEST_OPND -o playground.elf playground.cpp --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
int main()
{
    Term input = Term_("1 + 2 * 2 ^ 3 ^ 2 + 91 ^ 1");

    {
        auto op = optr(input, 1);
        std::cout << "#1: " << left_opnd(input, op) << " " << op.val << " " << right_opnd(input, op) << std::endl;
    }

    {
        auto op = optr(input, -1);
        std::cout << "#-1: " << left_opnd(input, op) << " " << op.val << " " << right_opnd(input, op) << std::endl;
    }
}
#endif // TEST_OPND

#ifdef TEST_PREV_NEXT_OPTR
// g++ -D TEST_PREV_NEXT_OPTR -o playground.elf playground.cpp --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
int main()
{
    Term input = Term_("1 + 2 * 2 ^ 3 ^ 2 + 91 ^ 1");

    {
        auto op = optr(input, 2);
        std::cout << "#2: " << op.val << std::endl;
        std::cout << "<prev>: " << prev_optr(input, op).val << std::endl;
        std::cout << "---" << std::endl;
    }

    {
        auto op = optr(input, 2);
        std::cout << "#2: " << op.val << std::endl;
        std::cout << "<next>: " << next_optr(input, op).val << std::endl;
        std::cout << "---" << std::endl;
    }

    // // ERR
    // {
    //     auto op = optr(input, 1);
    //     std::cout << "#1: " << op.val << std::endl;
    //     std::cout << "<prev>: " << prev_optr(input, op).val << std::endl;
    // }

    // // ERR
    // {
    //     auto op = optr(input, -1);
    //     std::cout << "#-1: " << op.val << std::endl;
    //     std::cout << "<next>: " << next_optr(input, op).val << std::endl;
    // }
}
#endif // TEST_PREV_NEXT_OPTR

#ifdef TEST_PARENTHESIZE
// g++ -D TEST_PARENTHESIZE -o playground.elf playground.cpp src/visitors/Unparse.cpp src/visitors/ConvenientVisitor.cpp obj/*.o --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
int main()
{
    Term input = Term_("1 + 2 * 2 ^ 3 ^ 2 + 91 ^ 1");
    std::cout << "Term: " << input << std::endl;

    {
        auto op = optr(input, 1);
        parenthesize_optn(&input, op);
    }

    std::cout << "Term: " << input << std::endl;

    {
        auto op = optr(input, 1);
        parenthesize_optn(&input, op);
    }

    std::cout << "Term: " << input << std::endl;

}
#endif // TEST_PARENTHESIZE

#ifdef MAIN

enum Associativity {
    LEFT_ASSOCIATIVE,
    RIGHT_ASSOCIATIVE,
};

std::vector<std::pair<std::string, Associativity>> prec_table = {
    /* ordered from highest precedence to lowest */

    {"^", RIGHT_ASSOCIATIVE},

    {"*", LEFT_ASSOCIATIVE},
    {"/", LEFT_ASSOCIATIVE},

    {"+", LEFT_ASSOCIATIVE},
    {"-", LEFT_ASSOCIATIVE},
};

enum Direction {
    ITERATE_LEFT_TO_RIGHT,
    ITERATE_RIGHT_TO_LEFT,
};

// returns false if no op found, true otherwise
bool parenthesizeFirstEncounteredOp(Term* term, std::string opval, Direction direction) {
    std::optional<Operator> op;
    std::optional<Operator> (*next)(Term, Operator);

    if (term->words.size() <= 2) {
        return false; // no operation can be found
    }

    if (direction == ITERATE_LEFT_TO_RIGHT) {
        op = optr(*term, 1);
        next = next_optr;
    }

    if (direction == ITERATE_RIGHT_TO_LEFT) {
        op = optr(*term, -1);
        next = prev_optr;
    }

    while (op) {
        if (op->val == opval) {
            parenthesize_optn(/*OUT*/term, *op);
            return true;
        }
        op = next(*term, *op);
    }

    return false; // no op found
}

// g++ -D MAIN -o playground.elf playground.cpp src/visitors/Unparse.cpp src/visitors/ConvenientVisitor.cpp obj/*.o --std=c++23 -Wall -Wextra -Og -ggdb3 -I include
int main()
{
    Term input = Term_("1 + 2 * 2 ^ 3 ^ 2 + 91 ^ 1");
    std::cout << "Term: " << input << std::endl;

    for (auto [optr, assoc]: prec_table) {
        auto direction = assoc == LEFT_ASSOCIATIVE? ITERATE_LEFT_TO_RIGHT : ITERATE_RIGHT_TO_LEFT;
        while (parenthesizeFirstEncounteredOp(&input, optr, direction))
            ; // until no more operator found
    }

    std::cout << "Term: " << input << std::endl;
}
#endif // MAIN
