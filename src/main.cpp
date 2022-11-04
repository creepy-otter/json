#include <iostream>

#include "json.hpp"

int main() {
  otter::json j1{std::string{"Hello World"}};
  /* TODO(creepy-otter): Add compatible template functions using
   * std::enable_if to support arbitary assginment */
  std::unordered_map<std::string, int> m;
  m["Hello"] = 1;
  otter::json j2 = m;

  j1.debug_print();
  j2.debug_print();
  return 0;
}