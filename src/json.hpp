#pragma once

#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "external_construct.hpp"
#include "meta_helper.hpp"
#include "value_type.h"

namespace otter {
template <typename T>
bool is_lvalue(T&) {
  return true;
}
template <typename T>
bool is_lvalue(T&&) {
  return false;
}

class json {
 public:
  template <typename T, typename... Args>
  static T* create(Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    return ptr;
  }
  union json_value;
  using json_object = std::unordered_map<std::string, json>;
  using json_array = std::vector<json>;
  using json_string = std::string;
  using json_int = int64_t;
  using json_float = float;
  using json_bool = bool;

  union json_value {
    /* data */
    json_object* obj;
    json_array* arr;
    json_string* str;
    json_int num_int;
    json_float num_float;
    json_bool boolean;

    // default constructor (for null values)
    json_value() { obj = nullptr; }
    json_value(json_int val) { num_int = val; }
    json_value(json_float val) { num_float = val; }
    json_value(json_bool val) { boolean = val; }
    json_value(const json_object& val) : obj(create<json_object>(val)) {}
    json_value(json_object&& val) : obj(create<json_object>(std::move(val))) {}
    json_value(const json_array& val) : arr(create<json_array>(val)) {}
    json_value(json_array&& val) : arr(create<json_array>(std::move(val))) {}
    json_value(const json_string& val) : str(create<json_string>(val)) {}
    json_value(json_string&& val) noexcept
        : str(create<json_string>(std::move(val))) {}

   public:
    void release(json_type t) {
      switch (t) {
        case json_type::JSON_OBJECT:
          /* TODO(creepy-otter): a temporary delete for json_object,
           * do this recursively or flatten the object */
          if (obj != nullptr) {
            delete obj;
            obj = nullptr;
          }
          break;
        case json_type::JSON_ARRAY:
          if (arr != nullptr) {
            delete arr;
            arr = nullptr;
          }
          break;
        case json_type::JSON_STRING:
          if (str != nullptr) {
            delete str;
            str = nullptr;
          }
        default:
          break;
      }
    }
  };

  /* data */
  json_value value_;
  json_type type_ = json_type::JSON_NULL;

 public:
  ~json() { value_.release(type_); }

  json(std::nullptr_t = nullptr) : type_(json_type::JSON_NULL) {
    value_.obj = nullptr;
  }

  json(const json& other) : type_(other.type_) {
    switch (type_) {
      case json_type::JSON_OBJECT:
        value_ = *other.value_.obj;
        break;
      case json_type::JSON_ARRAY:
        value_ = *other.value_.arr;
        break;
      case json_type::JSON_STRING:
        value_ = *other.value_.str;
        break;
      case json_type::JSON_NUMBER_INT:
        value_ = other.value_.num_int;
        break;
      case json_type::JSON_NUMBER_FLOAT:
        value_ = other.value_.num_float;
        break;
      case json_type::JSON_BOOLEAN:
        value_ = other.value_.boolean;
        break;
      case json_type::JSON_NULL:
      default:
        break;
    }
  }

  json(json&& other) noexcept
      : type_(std::move(other.type_)), value_(std::move(other.value_)) {
    other.type_ = json_type::JSON_NULL;
  }

  template <typename CompatibleType>
  json(CompatibleType&& val) : type_(json_type::JSON_NULL) {
    internal::init_json(*this, std::forward<CompatibleType>(val));
  }

  json(std::initializer_list<json> init_list) : type_(json_type::JSON_NULL) {
    type_ = json_type::JSON_ARRAY;
    value_.arr = create<json_array>(init_list.begin(), init_list.end());
  }

  void debug_print_int() { std::cout << value_.num_int << std::endl; }
  void debug_print_float() { std::cout << value_.num_float << std::endl; }
  static void debug_print_array(const json_array& j_arr) {
    std::cout << "[";
    for (auto it = j_arr.begin(); it != j_arr.end(); it++) {
      debug_print(*it, '\0');
      if (it != j_arr.end() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]";
  }

  static void debug_print(const json& j, char delim = '\n') {
    switch (j.type_) {
      case json_type::JSON_NUMBER_INT:
        std::cout << j.value_.num_int << delim;
        break;
      case json_type::JSON_NUMBER_FLOAT:
        std::cout << j.value_.num_float << delim;
        break;
      case json_type::JSON_STRING:
        std::cout << '\"' << *j.value_.str << '\"' << delim;
        break;
      case json_type::JSON_BOOLEAN:
        std::cout << std::boolalpha << j.value_.boolean << delim;
        break;
      case json_type::JSON_NULL:
        std::cout << "null" << std::endl;
        break;
      case json_type::JSON_ARRAY:
        debug_print_array(*j.value_.arr);
        break;
      case json_type::JSON_OBJECT:
        std::cout << "Object Type" << std::endl;
        break;
      default:
        std::cout << (int)(j.type_) << std::endl;
        break;
    }
  }
};

}  // namespace otter
