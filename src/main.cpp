#include <iostream>
#include <typeinfo>

#include "json.hpp"

int main() {
  otter::json j1 = "Hello World!";
  j1.debug_print_str();
  otter::json j2 = 42;
  j2.debug_print_int();
  otter::json j3 = 0.1;
  j3.debug_print();
  otter::json j4 = false;
  j4.debug_print();
  j1 = nullptr;
  j1.debug_print();
  /* TODO(creepy-otter): Add compatible template functions using
   * std::enable_if to support arbitary assginment */
  return 0;
}