#include <iostream>

#include "json.hpp"

int main() {
  otter::json j1{"Hello World"};
  /* TODO(creepy-otter): Add compatible template functions using std::enable_if
   * to support arbitary assginment */
  // otter::json j2 = "Hello";

  j1.debug_print();
  // j2.debug_print();
  return 0;
}