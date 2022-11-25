#include <iostream>
#include <typeinfo>

#include "json.hpp"

using namespace otter;
int main() {
  // otter::json j5{1, 2, 3};
  // otter::json::debug_print(j5);
  otter::json j6 = {1, {2}};
  std::cout << j6.value_.arr->size() << std::endl;
  std::cout << (j6.value_.arr)[0].size() << std::endl;
  std::cout << (j6.value_.arr)[1].size() << std::endl;
  /* TODO(creepy-otter): Add compatible template functions using
   * std::enable_if to support arbitary assginment */
  return 0;
}