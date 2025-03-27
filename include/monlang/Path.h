#ifndef PATH_H
#define PATH_H

#include <monlang/ast/Path.h>
#include <monlang/Atom.h>

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<Path> {
    PathLeftPart leftPart; // never Malformed, by design
    MayFail<Atom> rightPart;

    size_t _tokenLen = 0;
    explicit MayFail_(const PathLeftPart&, const MayFail<Atom>&);

    explicit MayFail_(const Path&);
    explicit operator Path() const;
};

Malformed<Malformed_<Path>>
consumePath(PostfixLeftPart& accumulatedPostfixLeftPart, const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // PATH_H
