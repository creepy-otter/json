#pragma once

#include "value_type.h"
/* Build json value from other types */

namespace internal {

template <json_type>
struct external_constructor;

/* String constructor */
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

/* Integer constructor */
template <>
struct external_constructor<json_type::JSON_NUMBER_INT> {
  template <typename JsonType, typename IntegerType>
  static void construct(JsonType& j, IntegerType num) {
    j.value_.release(j.type_);
    j.type_ = json_type::JSON_NUMBER_INT;
    j.value_ = num;
  }
};

/* Float constructor */
template <>
struct external_constructor<json_type::JSON_NUMBER_FLOAT> {
  template <typename JsonType, typename FloatType>
  static void construct(JsonType& j, FloatType num) {
    j.value_.release(j.type_);
    j.type_ = json_type::JSON_NUMBER_FLOAT;
    j.value_ = num;
  }
};

/* Entries */
template <typename JsonType, typename StringType,
          std::enable_if_t<std::is_constructible_v<
                               typename JsonType::json_string, StringType>,
                           int> = 0>
void init_json(JsonType& j, const StringType& str) {
  external_constructor<json_type::JSON_STRING>::construct(j, str);
}

template <typename JsonType, typename StringType,
          std::enable_if_t<std::is_constructible_v<
                               typename JsonType::json_string, StringType>,
                           int> = 0>
void init_json(JsonType& j, StringType&& str) {
  external_constructor<json_type::JSON_STRING>::construct(j, std::move(str));
}

template <typename JsonType, typename IntegerType,
          std::enable_if_t<std::is_integral_v<IntegerType> &&
                               !std::is_same_v<IntegerType, bool> &&
                               !std::is_floating_point_v<IntegerType>,
                           int> = 0>
void init_json(JsonType& j, IntegerType num) {
  external_constructor<json_type::JSON_NUMBER_INT>::construct(
      j, static_cast<typename JsonType::json_int>(num));
}

template <typename JsonType, typename FloatType,
          std::enable_if_t<std::is_floating_point_v<FloatType>, int> = 0>
void init_json(JsonType& j, FloatType num) {
  external_constructor<json_type::JSON_NUMBER_FLOAT>::construct(
      j, static_cast<typename JsonType::json_float>(num));
}
}  // namespace internal
