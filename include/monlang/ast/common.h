#ifndef AST_COMMON_H
#define AST_COMMON_H

#include <vector>
#include <sstream>

struct Quantifier {
  private:
    int n;

  public:
    explicit Quantifier(int);
    operator int() const; // accessor
};
Quantifier operator "" _(unsigned long long int);

struct CharacterAppearance {
    char c;
    Quantifier ntimes;
    CharacterAppearance(char c, Quantifier ntimes = 1_);
};

using Sequence = std::vector<CharacterAppearance>;

Sequence operator *(int ntimes, Sequence);
std::ostream& operator<<(std::ostream&, Sequence);

#endif // AST_COMMON_H
