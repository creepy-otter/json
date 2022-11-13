#pragma once

#include "value_type.h"
/* Build json value from other types */

namespace internal {

template <json_type>
struct external_constructor;

template <>
struct external_constructor<json_type::JSON_STRING> {
  template <typename JsonType>
  static void construct(JsonType& j,
                        const typename JsonType::json_string& str) {
    j.value_.release(j.type_);
    j.type_ = json_type::JSON_STRING;
    j.value_ = str;
  }

  template <typename JsonType>
  static void construct(JsonType& j, typename JsonType::json_string&& str) {
    j.value_.release(j.type_);
    j.type_ = json_type::JSON_STRING;
    j.value_ = std::move(str);
  }

  template <
      typename JsonType, typename StringType,
      std::enable_if_t<
          !std::is_same_v<StringType, typename JsonType::json_string>, int> = 0>
  static void construct(JsonType& j, const StringType& str) {
    j.value_.release(j.type_);
    j.type_ = json_type::JSON_STRING;
    j.value_.str = j.template create<typename JsonType::json_string>(str);
  }
};

template <typename JsonType, typename StringType,
          std::enable_if_t<std::is_constructible_v<
                               typename JsonType::json_string, StringType>,
                           int> = 0>
void init_json(JsonType& j, const StringType& str) {
  std::cout << typeid(str).name() << std::endl;
  external_constructor<json_type::JSON_STRING>::construct(j, str);
}
}  // namespace internal
