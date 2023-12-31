#ifndef VARIANT_UTIL_H
#define VARIANT_UTIL_H

#include <variant>

/* overload pattern */
template <class... Fs> struct overload : Fs... { using Fs::operator()...; };
template <class... Fs> overload(Fs...) -> overload<Fs...>;

template <class... Args>
struct variant_cast_proxy
{
    std::variant<Args...> v;

    template <class... ToArgs>
    operator std::variant<ToArgs...>() const
    {
        return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return arg ; },
                          v);
    }
};

template <class... Args>
auto variant_cast(const std::variant<Args...>& v) -> variant_cast_proxy<Args...>
{
    return {v};
}

#endif // VARIANT_UTIL_H
