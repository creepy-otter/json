#pragma once

enum class json_type {
  JSON_OBJECT = 0,
  JSON_ARRAY = 1,
  JSON_STRING = 2,
  JSON_NUMBER_INT = 3,
  JSON_NUMBER_FLOAT = 4,
  JSON_BOOLEAN = 5,
  JSON_NULL = 6
};