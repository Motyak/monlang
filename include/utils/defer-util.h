// stackoverflow dot com /questions/32432450/what-is-standard-defer-finalizer-implementation-in-c

#ifndef DEFER_UTIL_H

template <class F>
struct __Deferrer {
    F f;
    ~__Deferrer() {
        f();
    }
};

template <class F>
__Deferrer<F> operator*(int, F f) {
    return {f};
}

#define __DEFER_(x) __defer##x
#define __DEFER(x) __DEFER_(x)
#define defer auto __DEFER(__LINE__) = 0 * [&]()

#endif // DEFER_UTIL_H
