#pragma once

#include "types.h"

namespace scope {

namespace parse {
char const *selector(char const *first, char const *last,
                     scope::types::selector_t &selector);

} // namespace parse

} // namespace scope
