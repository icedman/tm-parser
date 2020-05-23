#include "scope.h"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace scope {

scope_t::scope_t(){};
scope_t::scope_t(char const *scope) { name = scope; };
scope_t::scope_t(std::string const &scope) { name = scope; };
scope_t::~scope_t(){};

scope_t::scope_t(scope_t &&rhs) { name = rhs.name; };
scope_t::scope_t(scope_t const &rhs) { name = rhs.name; };

scope_t& scope_t::operator=(scope_t &&rhs) {
  return *this;
};

scope_t& scope_t::operator=(scope_t const &rhs) {
  return *this;
}

bool scope_t::has_prefix(scope_t const &rhs) const { return false; }

void scope_t::push_scope(std::string const &atom){};
void scope_t::pop_scope(){};
std::string const &scope_t::back() const { return name; }
size_t scope_t::size() const { return 0; }
bool scope_t::empty() const { return !name.length(); }
size_t scope_t::hash() const { return 0; }

bool scope_t::operator==(scope_t const &rhs) const { return name == rhs.name; }
bool scope_t::operator!=(scope_t const &rhs) const { return name != rhs.name; }
bool scope_t::operator<(scope_t const &rhs) const { return name < rhs.name; }

// explicit operator scope_t::bool() const { return name.length(); }
// explicit operator scope_t::std::string() const { return name; }

} // namespace scope
