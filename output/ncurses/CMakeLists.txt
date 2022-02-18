# Add the library
add_library(wordle_ncurses ncurses.cpp)
# It needs to be linked with ncurses
target_link_libraries(wordle_ncurses ncurses)
