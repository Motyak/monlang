
#define until(x) while(!(x))

/* works with while,for loops, as well as do..while */
/* works with multiple loops sequentially */
/* works with nested loops as well */
inline thread_local bool __first_it;
#define LOOP __first_it = true;
#define ENDLOOP __first_it = false;
