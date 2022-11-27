#include <iostream>
#include <typeinfo>

#include "json.hpp"

using namespace otter;
int main() {
  json j6{"Hello", {"World", 1}, {false, {nullptr, {2.71, {true}}}}};
  json::debug_print(j6);
  /* TODO(creepy-otter): Support object type assignment */
  return 0;
}