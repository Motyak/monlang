#ifndef AST_COMMON_H
#define AST_COMMON_H

#include <vector>
#include <sstream>
#include <optional>
#include <variant>

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

Sequence operator *(int ntimes, const Sequence&);
std::ostream& operator<<(std::ostream&, const Sequence&);

std::optional<char> sequenceFirstChar(const Sequence&);
size_t sequenceLen(const Sequence&);

template <typename... Targs>
size_t token_len(const std::variant<Targs...>& entityVariant) {
    return std::visit(
        [](auto* entity){return entity->_tokenLen;},
        entityVariant
    );
}

template <typename T>
size_t token_len(const T& entity) {
    return entity._tokenLen;
}

template <typename... Targs>
void set_token_len(const std::variant<Targs...>& entityVariant, size_t tokenLen) {
    std::visit(
        [tokenLen](auto* entity){entity->_tokenLen = tokenLen;},
        entityVariant
    );
}

#endif // AST_COMMON_H
