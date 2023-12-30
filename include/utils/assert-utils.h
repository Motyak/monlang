
#define ASSERT(condition) \
    if (!(condition)) { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << std::endl; \
        std::terminate(); \
    }

#define SHOULD_NOT_HAPPEN() \
    std::cerr << "`Should not happen` raised in " << __FILE__ \
            << " line " << __LINE__ << std::endl; \
    std::terminate();

#define DEBUG(n) \
    std::cout << "[" << n << "] DEBUG: `" << peekRemainingCharacters(input) << "`" << std::endl;
