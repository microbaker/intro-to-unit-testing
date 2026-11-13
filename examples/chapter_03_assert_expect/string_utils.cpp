#include "string_utils.h"

std::string safe_substr(const std::string& s, size_t pos, size_t len) {
  if (pos > s.size()) return {};
  return s.substr(pos, len);
}
