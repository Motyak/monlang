#include <iostream>

#define ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << ": " << message << std::endl; \
        std::terminate(); \
    }

#define SHOULD_NOT_HAPPEN() \
    std::cerr << "`Should not happen` raised in " << __FILE__ \
            << " line " << __LINE__ << std::endl; \
    std::terminate();
