add_executable(is_valid is_valid.cpp)
target_link_libraries(is_valid wordle_core)
add_test(NAME is_valid COMMAND is_valid)

add_executable(set_get_words set_get_words.cpp)
target_link_libraries(set_get_words wordle_core)
add_test(NAME set_get_words COMMAND set_get_words ${CMAKE_CURRENT_SOURCE_DIR}/stress-test-wordlist.txt)
