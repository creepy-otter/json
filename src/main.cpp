#include <iostream>
#include <typeinfo>

#include "json.hpp"

int main() {
  otter::json j1 = "Hello World!";
  j1.debug_print_str();
  otter::json j2 = (int64_t)42;
  j2.debug_print_int();
  /* TODO(creepy-otter): Add compatible template functions using
   * std::enable_if to support arbitary assginment */
  return 0;
}